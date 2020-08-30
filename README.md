# Little Computer 3 Inspired Virtual Machine in C

##
16-bit address space
16-bit operators and operands
16-bit registers

## Instruction set

8-bit Op codes followed by 8-bit op arguments, for specifics see sections

|Op name|Op Code        |Description            |Format             |
|---    |---            |---                    |---                |
|ADD    |       00000001|Addition               |(op-code)-(op-args)|
|SUB    |       00000010|Subtraction            |(op-code)-(op-args)|
|AND    |       00000011|Bitwise AND            |(op-code)-(op-args)|
|NOT    |       00000100|Bitwise NOT            |(op-code)          |
|OR     |       00000101|Bitwise OR             |(op-code)-(op-args)|
|CMP    |       00000110|Comparison             |(op-code)-(op-args)|
|JMP    |       00000111|Jump (Always)          |(op-code)-(op-args)|
|JEQ    |       00001000|Jump If Equal          |(op-code)-(op-args)|
|JGT    |       00001001|Jump if greater        |(op-code)-(op-args)|
|JLT    |       00001010|Jump if less           |(op-code)-(op-args)|
|SDT    |       00001011|Save to Memory         |(op-code)-(op-args)|
|LDT    |       00001100|Load form Memory       |(op-code)-(op-args)|
|LDA    |       00001101|Load to Accumulator    |(op-code)-(op-args)|
|PSH    |       00001111|Push to stack          |(op-code)-(op-args)|
|POP    |       00010000|Pop from stack         |(op-code)-(op-args)|
|PSA    |       00010001|Push all registers     |(op-code)          |
|POA    |       00010010|Pop all registers      |(op-code)          |
|JSR    |       00010011|Jump to Subroutine     |(op-code)-(op-args)|
|RSR    |       00010100|Return from subroutine |(op-code)          |