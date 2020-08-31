import sys
input_path = sys.argv[1]
output_path = sys.argv[2]

f_in = open(input_path,"r")
f_out = open(output_path,"wb")

memory = []

opcodes = ["HLT","ADD","SUB","AND","NOT","IOR","CMP",
            "JMP","JEQ","JGT","JLT","SDT","LDT","MOV","PSH","POP",
            "PSA","JSR","RSR","SSB","SRT","JOF","JUF","FRT","BSR",
            "BSL"]

assemblerCommands = []

in_lines = f_in.readlines()
for line in in_lines:
    instruction = line[:-1].split(" ")

    opcode = 0
    for i in range(len(opcodes)):
        if instruction[0] == opcodes[i]:
            opcode = i

    if instruction[0] in ["ADD","SUB","AND","IOR","CMP","PSH"]:
        #ADD, SUB, AND, IOR, CMP
        memory.append(opcode)
        if instruction[1][0] == "R":
            bits8 = 0 <<4
            bits8 += int(instruction[1][2:])
            memory.append(bits8)
        elif instruction[1][0] == "A":
            bits8 = 1 <<4
            memory.append(bits8)
            memory.append(int(instruction[1][2:])>>8)
            memory.append(int(instruction[1][2:])%256)
        elif instruction[1][0] == "D":
            bits8 = 2 <<4
            memory.append(bits8)
            memory.append(int(instruction[1][2:])>>8)
            memory.append(int(instruction[1][2:])%256)

    elif instruction[0] in ["NOT","PSA","POA","RSR","SRT","FRT","BSR","BSL","HLT"]:
        memory.append(opcode)
        memory.append(0)
    
    elif instruction[0] in ["JMP","JEQ","JGT","JLT","JSR","JOF","JUF"]:
        memory.append(opcode)
        memory.append(0)
        memory.append(int(instruction[1][2:])>>8)
        memory.append(int(instruction[1][2:])%256)

    elif instruction[0] in ["SDT","LDT"]:
        memory.append(opcode)
        bits8 = int(instruction[1][2:])
        memory.append(bits8)
        memory.append(int(instruction[2][2:])>>8)
        memory.append(int(instruction[2][2:])%256)
    
    elif instruction[0] in ["MOV"]:
        memory.append(opcode)
        bits8 = int(instruction[1][2:]) <<4
        bits8 += int(instruction[2][2:])
        memory.append(bits8)

    elif instruction[0] in ["POP"]:
        memory.append(opcode)
        if instruction[1][0] == "R":
            bits8 = int(instruction[1][2:])
            memory.append(bits8)
        elif instruction[1][0] == "A":
            memory.append(1<<4)
            memory.append(int(instruction[1][2:])>>8)
            memory.append(int(instruction[1][2:])%256)

    elif instruction[0] in ["SSB"]:
        memory.append(opcode)
        if instruction[1][0] == "A":
            memory.append(0)
            memory.append(int(instruction[1][2:])>>8)
            memory.append(int(instruction[1][2:])%256)
        elif instruction[1][0] == "D":
            memory.append(1<<4)
            memory.append(int(instruction[1][2:])>>8)
            memory.append(int(instruction[1][2:])%256)
    elif instruction[0][0] == "@":
        assemblerCommands.append(instruction)

memory += [0 for _ in range(65536-len(memory))]

for command in assemblerCommands:
    if command[0][1:] == "SET":
        address = int(command[1])*2
        value = int(command[2])
        memory[address] = value>>8
        memory[address+1] = value %256


for i in range(len(memory)):
    f_out.write((memory[i]).to_bytes(1,byteorder='big',signed=False))
    
f_in.close()
f_out.close()