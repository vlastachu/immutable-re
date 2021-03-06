/**
 * Copyright (c) 2017 - present Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

type t 'a =
  | Ascending (Vector.t 'a)
  | Descending (Vector.t 'a);

let empty (): (t 'a) => Ascending (Vector.empty ());

let addFirst (value: 'a) (deque: t 'a): (t 'a) => switch deque {
  | Ascending vector =>
      Ascending (vector |> Vector.addFirst value)
  | Descending vector =>
      Descending (vector |> Vector.addLast value);
};

let addLast (value: 'a) (deque: t 'a): (t 'a) => switch deque {
  | Ascending vector =>
      Ascending (vector |> Vector.addLast value)
  | Descending vector =>
      Descending (vector |> Vector.addFirst value);
};

let count (deque: t 'a): int => switch deque {
  | Ascending vector
  | Descending vector => Vector.count vector
};

let first (deque: t 'a): (option 'a) => switch deque {
  | Ascending vector => vector |> Vector.first
  | Descending vector => vector |> Vector.last
};

let firstOrRaise (deque: t 'a): 'a => switch deque {
  | Ascending vector => Vector.firstOrRaise vector
  | Descending vector => Vector.lastOrRaise vector
};

let isEmpty (deque: t 'a): bool =>
  deque |> count === 0;

let isNotEmpty (deque: t 'a): bool =>
  deque |> count !== 0;

let last (deque: t 'a): (option 'a) => switch deque {
  | Ascending vector => vector |> Vector.last
  | Descending vector => vector |> Vector.first
};

let lastOrRaise (deque: t 'a): 'a => switch deque {
  | Ascending vector => Vector.lastOrRaise vector
  | Descending vector => Vector.firstOrRaise vector
};

let removeFirstOrRaise (deque: t 'a): (t 'a) => switch deque {
  | Ascending vector =>
      Ascending (Vector.removeFirstOrRaise vector)
  | Descending vector =>
      Descending (Vector.removeLastOrRaise vector)
};

let removeLastOrRaise (deque: t 'a): (t 'a) => switch deque {
  | Ascending vector =>
      Ascending (Vector.removeLastOrRaise vector)
  | Descending vector =>
      Descending (Vector.removeFirstOrRaise vector)
};

let return (value: 'a): (t 'a) =>
  Ascending (Vector.return value);

let reverse (deque: t 'a): (t 'a) => switch deque {
  | Ascending vector => Descending vector
  | Descending vector => Ascending vector
};

let reduce
    while_::(predicate: 'acc => 'a => bool)=Functions.alwaysTrue2
    (f: 'acc => 'a => 'acc)
    (acc: 'acc)
    (deque: t 'a): 'acc => switch deque {
  | Ascending vector => vector |> Vector.reduce while_::predicate f acc;
  | Descending vector => vector |> Vector.reduceRight while_::predicate f acc;
};

let reduceRight
    while_::(predicate: 'acc => 'a => bool)=Functions.alwaysTrue2
    (f: 'acc => 'a => 'acc)
    (acc: 'acc)
    (deque: t 'a): 'acc => switch deque {
  | Ascending vector => vector |> Vector.reduceRight while_::predicate f acc;
  | Descending vector => vector |> Vector.reduce while_::predicate f acc;
};

let removeAll (_: t 'a): (t 'a) => empty ();

let toIterator (deque: t 'a): (Iterator.t 'a) =>
 if (isEmpty deque) (Iterator.empty ())
 else { reduce: fun predicate f acc => reduce while_::predicate f acc deque };

let toIteratorRight (deque: t 'a): (Iterator.t 'a) =>
  if (isEmpty deque) (Iterator.empty ())
  else { reduce: fun predicate f acc => reduceRight while_::predicate f acc deque };

let toSequence (deque: t 'a): (Sequence.t 'a) => switch deque {
  | Ascending vector => vector |> Vector.toSequence
  | Descending vector => vector |> Vector.toSequenceRight;
};

let toSequenceRight (deque: t 'a): (Sequence.t 'a) => switch deque {
  | Ascending vector => vector |> Vector.toSequenceRight
  | Descending vector => vector |> Vector.toSequence;
};

let module TransientDeque = {
  let module TransientVector = Vector.TransientVector;

  type deque 'a = t 'a;

  type transientDequeImpl 'a =
    | Ascending (TransientVector.t 'a)
    | Descending (TransientVector.t 'a);

  type t 'a = Transient.t (transientDequeImpl 'a);

  let mutate (deque: deque 'a): (t 'a) => switch deque {
    | Ascending vector =>
        Transient.create (Ascending (Vector.mutate vector))
    | Descending vector  =>
        Transient.create (Descending (Vector.mutate vector))
  };

  let addFirst
      (value: 'a)
      (transient: t 'a): (t 'a) => switch (Transient.get transient) {
    | Ascending vector =>
        TransientVector.addFirst value vector |> ignore;
        transient;
    | Descending vector =>
        TransientVector.addLast value vector |> ignore;
        transient;
  };

  let addFirstAll
      (iter: Iterator.t 'a)
      (transient: t 'a): (t 'a) => switch (Transient.get transient) {
    | Ascending vector =>
        vector |> TransientVector.addFirstAll iter |> ignore;
        transient;
    | Descending vector =>
        vector |> TransientVector.addLastAll iter |> ignore;
        transient;
  };

  let addLast
      (value: 'a)
      (transient: t 'a): (t 'a) => switch (Transient.get transient) {
    | Ascending vector =>
        TransientVector.addLast value vector |> ignore;
        transient;
    | Descending vector =>
        TransientVector.addFirst value vector |> ignore;
        transient;
  };

  let addLastAll
      (iter: Iterator.t 'a)
      (transient: t 'a): (t 'a) => switch (Transient.get transient) {
    | Ascending vector =>
        vector |> TransientVector.addLastAll iter |> ignore;
        transient;
    | Descending vector =>
        vector |> TransientVector.addFirstAll iter |> ignore;
        transient;
  };

  let count (transient: t 'a): int => switch (Transient.get transient) {
    | Ascending vector
    | Descending vector => TransientVector.count vector
  };

  let empty (): (t 'a) =>
    empty () |> mutate;

  let first (transient: t 'a): (option 'a) => switch (Transient.get transient) {
    | Ascending vector => vector |> TransientVector.first
    | Descending vector => vector |> TransientVector.last
  };

  let firstOrRaise (transient: t 'a): 'a => switch (Transient.get transient) {
    | Ascending vector => TransientVector.firstOrRaise vector
    | Descending vector => TransientVector.lastOrRaise vector
  };

  let isEmpty (transient: t 'a): bool =>
    transient |> count === 0;

  let isNotEmpty (transient: t 'a): bool =>
    transient |> count !== 0;

  let last (transient: t 'a): (option 'a) => switch (Transient.get transient) {
    | Ascending vector => vector |> TransientVector.last
    | Descending vector => vector |> TransientVector.first
  };

  let lastOrRaise (transient: t 'a): 'a => switch (Transient.get transient)  {
    | Ascending vector => TransientVector.lastOrRaise vector
    | Descending vector => TransientVector.firstOrRaise vector
  };

  let persist (transient: t 'a): (deque 'a) => switch (Transient.persist transient) {
    | Ascending vector => Ascending (TransientVector.persist vector)
    | Descending vector => Descending (TransientVector.persist vector)
  };

  let removeAllImpl
      (_: Transient.Owner.t)
      (_: transientDequeImpl 'a): (transientDequeImpl 'a) =>
    Ascending (TransientVector.empty ());

  let removeAll (transient: t 'a): (t 'a) =>
    transient |> Transient.update removeAllImpl;

  let removeFirstOrRaise (transient: t 'a): (t 'a) => switch (Transient.get transient) {
    | Ascending vector =>
        TransientVector.removeFirstOrRaise vector |> ignore;
        transient;
    | Descending vector =>
        TransientVector.removeLastOrRaise vector |> ignore;
        transient;
  };

  let removeLastOrRaise (transient: t 'a): (t 'a) => switch (Transient.get transient) {
    | Ascending vector =>
        TransientVector.removeLastOrRaise vector |> ignore;
        transient;
    | Descending vector =>
        TransientVector.removeFirstOrRaise vector |> ignore;
        transient;
  };

  let reverseImpl
      (_: Transient.Owner.t)
      (vector: transientDequeImpl 'a) => switch vector {
    | Ascending vector => Descending vector;
    | Descending vector => Ascending vector;
  };

  let reverse (transient: t 'a): (t 'a) =>
    transient |> Transient.update reverseImpl;
};

let mutate = TransientDeque.mutate;

let addFirstAll (iter: Iterator.t 'a) (deque: t 'a): (t 'a) => deque
  |> mutate
  |> TransientDeque.addFirstAll iter
  |> TransientDeque.persist;

let addLastAll (iter: Iterator.t 'a) (deque: t 'a): (t 'a) => deque
  |> mutate
  |> TransientDeque.addLastAll iter
  |> TransientDeque.persist;

let from (iter: Iterator.t 'a): (t 'a) =>
  empty () |> addLastAll iter;

let fromReverse (iter: Iterator.t 'a): (t 'a) =>
  empty () |> addFirstAll iter;

let map (f: 'a => 'b) (deque: t 'a): (t 'b) => switch deque {
  | Ascending vector => Ascending (Vector.map f vector)
  | Descending vector => Descending (Vector.map f vector)
};

let module ReducerRight = Reducer.Make1 {
  type nonrec t 'a = t 'a;
  let reduce = reduce;
};

let module Reducer = Reducer.Make1 {
  type nonrec t 'a = t 'a;
  let reduce = reduce;
};
