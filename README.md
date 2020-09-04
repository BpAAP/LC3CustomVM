# Little Computer 3 Inspired Virtual Machine in C - V1
A custom virtual machine written in C. With a python assembler and example programs.

Project originally inspired by `https://justinmeiners.github.io/lc3-vm/index.html#1:12`. Although in practice the two projects ended up working quite differently and completly incompatible. This main readme file includes the specification for the custom architecture and instruction set.

A python assembler to create executable binary files can be found in the `/python-assembler` folder which has its own readme file with further details.

You will also find examples both in assembly and their compiled binary files in the `/examples` folder. This folder also has its own readme with further details.

This is just version 1. Version 2 will include a simple video interface, as well as keyboard interaction.

For more details check my website at `www.benedekpapp.page`.

Running the vm with the option `-Debug`, will enable debug printing.

Running the vm with the option `-PrintEndState` will print the last value of the accumulator.

## Summary
A vm in C working on a custom instruction set. 

## Quick Start
1. See `README.md` in the `python_assembler` for how to write in assembly.
2. To assemble:
    * `make _yourfile_.bin` from this directory, `_yourfile_.assm` must also be in this directory.
    * Generally: run `python_assembler/assembler.py` and pass the path of `_yourfile_.assm`. `_yourfile_.bin` will be created in `python_assembler`
3. Run on vm with `./vm _yourfile_.bin` add `-Debug` for debug print statements.

## Architecture
16-bit address space
16-bit operators and operands
16-bit registers

Flags:
* flag_gt: Flag set by CMP when accumulator > operand. Used by JGT.
* flag_lt: Flag set by CMP when accumulator < operand. Used by JLT.
* flag_eq: Flag set by CMP when accumulator = operand. Used by JEQ.
* flag_uf: Set if arithmetic results in an underflow.
* flag_of:  Set when arithmetic results in an overflow.
***ADD TABLE***

Register
|Register name  |Register id|Included in PSA|
|---            |---        |---            |
|accumulator    |0000       |Yes            |
|general_1      |0001       |Yes            |
|general_2      |0010       |Yes            |
|general_3      |0011       |Yes            |
|general_4      |0100       |Yes            |
|general_5      |0101       |Yes            |
|general_6      |0110       |Yes            |
|general_7      |0111       |Yes            |
|general_8      |1000       |Yes            |
|general_9      |1001       |Yes            |
|general_10     |1010       |Yes            |
|general_11     |1011       |Yes            |
|general_12     |1100       |Yes            |
|general_13     |1101       |Yes            |
|general_14     |1110       |Yes            |
|general_15     |1111       |Yes            |
|stack_base     |N/A*       |No             |
|stack_pointer  |N/A*       |No             |
|Program Counter|N/A*       |No             |

*These registers have dedicated instruction for interacting with them. They are not designed for general purpose use.

## Instruction set

8-bit Op codes followed by 8-bit op arguments, for specifics see sections
Note: DNM is used to denote "Does not matter", these sections are not used by the instruction.

|Op name|Op Code        |Description            |Format             |
|---    |---            |---                    |---                |
|ADD    |       00000001|Addition               |(op-code)-(op-args)|
|SUB    |       00000010|Subtraction            |(op-code)-(op-args)|
|AND    |       00000011|Bitwise AND            |(op-code)-(op-args)|
|NOT    |       00000100|Bitwise NOT            |(op-code)          |
|IOR    |       00000101|Bitwise OR             |(op-code)-(op-args)|
|CMP    |       00000110|Comparison             |(op-code)-(op-args)|
|JMP    |       00000111|Jump (Always)          |(op-code)-(op-args)|
|JEQ    |       00001000|Jump If Equal          |(op-code)-(op-args)|
|JGT    |       00001001|Jump if greater        |(op-code)-(op-args)|
|JLT    |       00001010|Jump if less           |(op-code)-(op-args)|
|SDT    |       00001011|Save to Memory         |(op-code)-(op-args)|
|LDT    |       00001100|Load form Memory       |(op-code)-(op-args)|
|MOV    |       00001101|Move data between regs |(op-code)-(op-args)|
|PSH    |       00001111|Push to stack          |(op-code)-(op-args)|
|POP    |       00010000|Pop from stack         |(op-code)-(op-args)|
|PSA    |       00010001|Push all registers     |(op-code)          |
|POA    |       00010010|Pop all registers      |(op-code)          |
|JSR    |       00010011|Jump to Subroutine     |(op-code)-(op-args)|
|RSR    |       00010100|Return from subroutine |(op-code)          |
|SSB    |       00010101|Set stack base pointer |(op-code)-(op-args)|
|SRT    |       00010110|Reset stack pointer    |(op-code)-(op-args)|
|JOF    |       00010111|Jump if overflow       |(op-code)-(op-args)|
|JUF    |       00011000|Jump if underflow      |(op-code)-(op-args)|
|FRT    |       00011001|Reset flags            |(op-code)          |
|BSR    |       00011010|Bit shift right        |(op-code)          |
|BSL    |       00011011|Bit shift left         |(op-code)          |
|HLT    |       00000000|Halt                   |(op-code)          |
|NOP    |       00011100|No-op                  |(op-code)          |

### ADD | Addition
Carries out binary addition of the accumulator and another provided value, stores result in accumulator.

Types:
Add register: Uses the given register as the second operand.
|PC:x                           |
|---                            |
|(00000001)-(0000)-(register-id)|
The register ID is just the number of the register in binary: register 3 would be 0011.

Add from address: Second operand is the data stored at the address specified at the next address.
|PC:x                           |PC:x+1             |PC:n   |
|---                            |---                |---    |
|(00000001)-(0001)-(DNM)        |(address of data:n)|(data) |

Add directly: Uses value at next address as second operand.
|PC:x                           |PC:x+1             |
|---                            |---                |
|(00000001)-(0010)-(DNM)        |(data)             |

### SUB | Subtraction
Carries out binary subtraction, (Accumulator - specified-data), stores result in accumulator.

Types:
Sub register: Uses the given register as the second operand.
|PC:x                           |
|---                            |
|(00000010)-(0000)-(register-id)|
The register ID is just the number of the register in binary: register 3 would be 0011.

Add from address: Second operand is the data stored at the address specified at the next address.
|PC:x                           |PC:x+1             |PC:n   |
|---                            |---                |---    |
|(00000010)-(0001)-(DNM)        |(address of data:n)|(data) |

Add directly: Uses value at next address as second operand.
|PC:x                           |PC:x+1             |
|---                            |---                |
|(00000010)-(0010)-(DNM)        |(data)             |

### AND | Bitwise Logical AND
Identical pattern to Addition, except opcode is 00000011

### NOT | Bitwise Logical NOT
Not operation only possible on accumulator, result is stored in accumulator:
|PC:x               | 
|---                |
|(00000100)-(DNM)   |


### IOR  | Bitwise Logical OR (inclusive)
Identical pattern to Addition, except opcode is 00000101

### CMP | Compare with accumulator
Compares value in accumulator with given operand, accumulator value is unchanged, sets appropriate flag: flag_gt, flag_lt, flag_eq.

Ex: if accumulator is greater than operand. flag_gt is set.

Identical pattern to Addition, except opcode is 00000110.

### JMP | Unconditional Jump
Sets the program counter to the value given in the next address:
|PC:x               |PC:x+1                 |
|---                |---                    |
|(00000111)-(DNM)   |(address-for-jump)     |

### JEQ | Jump if equal
Should be used after a CMP instruction. Sets PC to data at next address if flag_eq is set. flag_eq is reset after call.
|PC:x               |                       |
|---                |---                    |
|(00001000)-(DNM)   |(address-for-jump)     |

### JGT | Jump if greater than
Similar to Jump if equal, except opcode is 00001001 and flag referenced is flag_gt.

### JLT | Jump if less than
Similar to Jump if equal, except opcode is 00001010 and flag referenced is flag_lt.



### SDT | Save data to memory
Save data from a register to an address in memory. (Accumulator also included)
Address is the data stored at the next address.
|PC:x                           |PC:x+1                     |
|---                            |---                        |
|(00001011)-(0000)-(register-id)|(address-to-save-to)       |

See register table at the top of the document for register id.

### LDT | Load data from memory
Load data from an address in memory to a register. (Accumulator also included)
Address is the data stored at the next address.
|PC:x                           |PC:x+1                     |
|---                            |---                        |
|(00001100)-(0000)-(register-id)|(address-to-load-from)     |

See register table at the top of the document for register id.

### MOV | Move data between registers
Moves data from an origin register to a destination register. Contents of destination register are overwritten, origin register is unchanged.
|PC:x                                   |
|---                                    |
|(00001101)-(origin-id)-(destination-id)|

See register table at the top of the document for register id.

### PSH | Push data to stack
Provided data is pushed to the stack, stack pointer is altered accordingly.

Types:
Push register: Push specified register to stack.
|PC:x                           |
|---                            |
|(00001111)-(0000)-(register-id)|

Push from address: Push value at address provided at next address.
|PC:x                           |PC:x+1             |PC:n   |
|---                            |---                |---    |
|(00001111)-(0001)-(DNM)        |(address-of-data:n)|(data) |

Push directly: Push value at next address.
|PC:x                           |PC:x+1             |
|---                            |---                |
|(00001111)-(0010)-(DNM)        |(data)             |

See register table at the top of the document for register id.

### POP | Pop data from stack
Last pushed data is popped from stack and transferred to specified location, stack pointer is altered accordingly. Data is not actively removed from stack, though is now tecnically outside, presistence is not guaranteed.

Types:
Pop to register
|PC:x                           |
|---                            |
|(00010000)-(0000)-(register-id)|

Pop to address
|PC:x                           |PC:x+1             |PC:n           |
|---                            |---                |---            |   
|(00010000)-(0001)-(DNM)        |(address-for-pop:n)|(popped-data)  |

See register table at the top of the document for register id.

### PSA | Push all register to stack
All registers are pushed to the stack in deterministic order, instruction designed to be used in conjunction with POA for saving and restoring machine state. (For example when jumping between subroutines). (PC exculded)

The state of the registers is unchanged.

|PC:x                           |
|---                            |
|(00010001)-(DNM)               |

### POA | POP and restore all registers
All registers are popped from the stack and restored. Designed for use in conjunction with PSA. (PC excluded)

|PC:x                           |
|---                            |
|(00010010)-(DNM)               |

Example use with subroutines:
[Push all registers]
[Push all arguments for calling the function]
[Push number of arguments]
JSR instruction will manipulate stack in background to place the return address before all the arguments using the number of arguments give.
In subroutine arguments are popped.

Returns are handled by writing to an address given as an argument.
After subroutine return:
[Pull all registers]
[Check the return value that was written to the specified address]

### JSR | Jump to subroutine 
Pushes address after JSR to stack, then sets PC to the given address. Designed to be used in conjunction with RSR.

|PC:x                           |PC:x+1                     |
|---                            |---                        |
|(00010011)-(DNM)               |(address-of-subroutine)    |

### RSR | Return from subroutine
Pops last stack entry and sets as PC.  Designed to be used in conjunction with JSR.

|PC:x                           |
|---                            |
|(00010100)-(DNM)               |

### SSB | Set stack_base register
Set the location for the base of the stack. Note: stack will grow by decreasing address. Data in the way will be overwritten.

Types:
Set from address:
|PC:x                           |PX:x+1                     |PC:n           |
|---                            |---                        |---            |
|(00010101)-(0000)-(DNM)        |(address-of-value:n)       |(data)         |

Set from value:
|PC:x                           |PX:x+1                     |
|---                            |---                        |
|(00010101)-(0001)-(DNM)        |(data)                     |

### SRT | Reset stack pointer
Sets stack_pointer to stack_base. No data is actively deleted, though persistence is not guaranteed.

|PC:x                           |
|---                            |
|00010110-(DNM)                 |

### JOF | Jump if overflow
Similar to Jump if equal. Except opcode is 00010111. Flag is reset.

### JUF | Jump if underflow
Similar to Jump if equal. Except opcode is 00011000. Flag is reset.

### FRT | Reset all flags
Resets all flags.
|PC:x            |
|---             |
|(00011001)-(DNM)|

### BSR | Bit shift accumulator right
|PC:x            |
|---             |
|(00011010)-(DNM)|

### BSL | Bit shift accumulator left
|PC:x            |
|---             |
|(00011011)-(DNM)|

### HLT | Halt
Stops execution irreversibly.
|PC:x            |
|---             |
|(0000)-(DNM)|

### NOP |No-op
No operation happens.
|PC:x            |
|---             |
|(00011100)-(DNM)|