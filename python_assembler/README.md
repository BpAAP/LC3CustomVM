# Python based assembler

## Writing assembly programs:
Assembly programs are simple text files with the extension `.assm`
Each instruction is on a new line. Empty lines are ignored.

Instructions to the processor start with the 3 letter names for the operations that are available in the instruction set. Followed by the appropriate number of `source:data` pairs, seperated by spaces. A `source` is one of `A`, `D` or `R` denoting that the following data is a/an address, data or register respectively. Data is a 16 bit integer.

Examples:
|Instruction    |Description                                            |
|---            |---                                                    |
|`LDT R:0 A:100`|Load data at address 100 into register 0 (accumulator) |
|`ADD D:20`     |Add the data 20 to the accumulator. (accumulator + 20) |
|`CMP D:22`     |Compare the data 22 to the accumulator.                |
|`HLT`          |Halt execution                                         |

## Assembler instructions
`@SET A D`   Set value `D` in memory at address `A`.
`@SET LABEL X` Associate the address of the next instruction with the lable `X`
`@GET LABEL X` Get the address that the lable `X` is referring to.
`#` Precedes a comment. Do not use `#` after a `@` on the same line as these interfere with each other.



