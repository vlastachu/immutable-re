/**
 * Copyright (c) 2017 - present Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

open Immutable;
open ReUnit;
open ReUnit.Test;

let test = describe "Reducer" [
  describe "S" [
    it "count" (fun () => {
      IntRange.create start::0 count::5
        |> IntRange.Reducer.count
        |> Expect.toBeEqualToInt 5;
    }),
    it "every" (fun () => {
      IntRange.Reducer.every (fun _ => false) IntRange.empty |> Expect.toBeEqualToTrue;

      IntRange.create start::0 count::5
        |> IntRange.Reducer.every (fun i => i >= 0)
        |> Expect.toBeEqualToTrue;

      IntRange.create start::0 count::5
        |> IntRange.Reducer.every (fun i => i < 3)
        |> Expect.toBeEqualToFalse;
    }),
    it "find" (fun () => {
      IntRange.create start::0 count::5
        |> IntRange.Reducer.find (fun i => i ===2)
        |> Expect.toBeEqualToSomeOfInt 2;

      IntRange.empty
        |> IntRange.Reducer.find (fun i => i ===2)
        |> Expect.toBeEqualToNoneOfInt;

      IntRange.create start::0 count::5
        |> IntRange.Reducer.find (fun i => i ===5)
        |> Expect.toBeEqualToNoneOfInt;
    }),
    it "findOrRaise" (fun () => {
      IntRange.create start::0 count::5
        |> IntRange.Reducer.findOrRaise (fun i => i ===2)
        |> Expect.toBeEqualToInt 2;

      (fun () => IntRange.empty |> IntRange.Reducer.findOrRaise (fun i => i ===2))
        |> Expect.shouldRaise;
    }),
    it "first" (fun () => {
      IntRange.create start::0 count::5
        |> IntRange.Reducer.first
        |> Expect.toBeEqualToSomeOfInt 0;

      IntRange.empty
        |> IntRange.Reducer.first
        |> Expect.toBeEqualToNoneOfInt;
    }),
    it "firstOrRaise" (fun () => {
      IntRange.create start::0 count::5
        |> IntRange.Reducer.firstOrRaise
        |> Expect.toBeEqualToInt 0;

      (fun () => IntRange.empty |> IntRange.Reducer.firstOrRaise) |> Expect.shouldRaise;
    }),
    it "forEach" (fun () => {
      let last = ref 0;
      IntRange.create start::0 count::5
        |> IntRange.Reducer.forEach while_::(fun i => i < 3) (fun i => { last := i });
      !last |> Expect.toBeEqualToInt 2;
    }),
    it "none" (fun () => {
      IntRange.Reducer.none (fun _ => false) IntRange.empty |> Expect.toBeEqualToTrue;

      IntRange.create start::0 count::5
        |> IntRange.Reducer.none (fun i => i >= 2)
        |> Expect.toBeEqualToFalse;

      IntRange.create start::0 count::5
        |> IntRange.Reducer.none (fun i => i < 0)
        |> Expect.toBeEqualToTrue;
    }),
    it "some" (fun () => {
      IntRange.Reducer.some (fun _ => false) IntRange.empty |> Expect.toBeEqualToFalse;

      IntRange.create start::0 count::5
        |> IntRange.Reducer.some (fun i => i >= 2)
        |> Expect.toBeEqualToTrue;

      IntRange.create start::0 count::5
        |> IntRange.Reducer.some (fun i => i < 0)
        |> Expect.toBeEqualToFalse;
    }),
  ],
  describe "S1" [
    it "count" (fun () => {
      IntRange.create start::0 count::5
        |> IntRange.toIterator
        |> Iterator.Reducer.count
        |> Expect.toBeEqualToInt 5;
    }),
    it "every" (fun () => {
      Iterator.Reducer.every (fun _ => false) (Iterator.empty ()) |> Expect.toBeEqualToTrue;

      IntRange.create start::0 count::5
        |> IntRange.toIterator
        |> Iterator.Reducer.every (fun i => i >= 0)
        |> Expect.toBeEqualToTrue;

      IntRange.create start::0 count::5
        |> IntRange.toIterator
        |> Iterator.Reducer.every (fun i => i < 3)
        |> Expect.toBeEqualToFalse;
    }),
    it "find" (fun () => {
      IntRange.create start::0 count::5
        |> IntRange.toIterator
        |> Iterator.Reducer.find (fun i => i ===2)
        |> Expect.toBeEqualToSomeOfInt 2;

      (Iterator.empty ())
        |> Iterator.Reducer.find (fun i => i ===2)
        |> Expect.toBeEqualToNoneOfInt;

      IntRange.create start::0 count::5
        |> IntRange.toIterator
        |> Iterator.Reducer.find (fun i => i ===5)
        |> Expect.toBeEqualToNoneOfInt;
    }),
    it "findOrRaise" (fun () => {
      IntRange.create start::0 count::5
        |> IntRange.toIterator
        |> Iterator.Reducer.findOrRaise (fun i => i ===2)
        |> Expect.toBeEqualToInt 2;

      (fun () => (Iterator.empty ()) |> Iterator.Reducer.findOrRaise (fun i => i ===2))
        |> Expect.shouldRaise;
    }),
    it "first" (fun () => {
      IntRange.create start::0 count::5
        |> IntRange.toIterator
        |> Iterator.Reducer.first
        |> Expect.toBeEqualToSomeOfInt 0;

      IntRange.empty
        |> IntRange.toIterator
        |> Iterator.Reducer.first
        |> Expect.toBeEqualToNoneOfInt;
    }),
    it "firstOrRaise" (fun () => {
      IntRange.create start::0 count::5
        |> IntRange.toIterator
        |> Iterator.Reducer.firstOrRaise
        |> Expect.toBeEqualToInt 0;

      (fun () => IntRange.empty
        |> IntRange.toIterator
        |> Iterator.Reducer.firstOrRaise
      ) |> Expect.shouldRaise;
    }),
    it "forEach" (fun () => {
      let last = ref 0;
      IntRange.create start::0 count::5
        |> IntRange.toIterator
        |> Iterator.Reducer.forEach while_::(fun i => i < 3) (fun i => { last := i });
      !last |> Expect.toBeEqualToInt 2;
    }),
    it "none" (fun () => {
      Iterator.Reducer.none (fun _ => false) (Iterator.empty ()) |> Expect.toBeEqualToTrue;

      IntRange.create start::0 count::5
        |> IntRange.toIterator
        |> Iterator.Reducer.none (fun i => i >= 2)
        |> Expect.toBeEqualToFalse;

      IntRange.create start::0 count::5
        |> IntRange.toIterator
        |> Iterator.Reducer.none (fun i => i < 0)
        |> Expect.toBeEqualToTrue;
    }),
    it "some" (fun () => {
      Iterator.Reducer.some (fun _ => false) (Iterator.empty ()) |> Expect.toBeEqualToFalse;

      IntRange.create start::0 count::5
        |> IntRange.toIterator
        |> Iterator.Reducer.some (fun i => i >= 2)
        |> Expect.toBeEqualToTrue;

      IntRange.create start::0 count::5
        |> IntRange.toIterator
        |> Iterator.Reducer.some (fun i => i < 0)
        |> Expect.toBeEqualToFalse;
    }),
  ],
];
