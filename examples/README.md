# Examples

## Fibonacci
Accumulator runs through the values of the Fibonnaci Sequence starting from values `0 1 ...`.
Quits once the accumulator value goes above a 200. (arbitrarily chosen, could go to 2^16 before overflow).
Run it with the `-Debug` option and see the values grow in the accumulator. (`ACC: #`).

## Multiply
Sets two integers in memory and multiplies them together. Finally placing the result in to the accumulator.
Quits when result is obtained.
Run it with the `-Debug` option and see the values grow in the accumulator. (`ACC: #`).
The final value given by `ACC: #` is the result of the multiplication.

## Subroutine
Loads some data from memory to some registers, then does some math. It then pushes all registers, jumps to a subroutine, modifies the registers, returns from subroutine, and restores pushed registers.
Then it does the opposite of the maths before and ends up with `ACC: #`.
Run it with the `-Debug` option and see the values grow in the accumulator. (`ACC: #`).

## Factorial
Uses the multiply program above inside a subroutine. 
Run it with the `-Debug` option and see the values grow in the accumulator. (`ACC: #`).
The final value given by `ACC: #` is the result of the multiplication.

## Power
Uses the multiply program above inside a subroutine. 
Run it with the `-Debug` option and see the values grow in the accumulator. (`ACC: #`).
The final value given by `ACC: #` is the result of the multiplication.

# Infinite Counter
Accumulator counts up forever.