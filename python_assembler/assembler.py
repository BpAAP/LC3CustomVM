import sys
input_path = sys.argv[1]
output_path = input_path[:-4] + "bin"

f_in = open(input_path,"r")
f_out = open(output_path,"wb")

#Empty array to store memory
#Note here each element is 8 its, but for the vm each element is 16bits
memory = []

#Opcode list for easily converting between opname and opcode
opcodes = ["HLT","ADD","SUB","AND","NOT","IOR","CMP",
            "JMP","JEQ","JGT","JLT","SDT","LDT","MOV","PSH","POP",
            "PSA","POA","JSR","RSR","SSB","SRT","JOF","JUF","FRT","BSR",
            "BSL","NOP"]

#List for string post-assembly instructions
assemblerCommands = []

#Read input file into list by lines
in_lines = f_in.readlines()

#Make dictionary for assembly-time labels
lables = {}

#Counter for calculating where the current instruction would be post-assembly
post_compile_address = 0

#List of commands that take up 2 elements (8bits)
opsLen2 = ["NOT","PSA","POA","RSR","SRT","FRT","BSR","BSL","HLT","NOP",
            "ADD R","SUB R","AND R","IOR R","CMP R","PSH R",
            "MOV","POP R"]

#List of commands that take up 4 elements (16bits)
opsLen4 = ["ADD D","SUB D","AND D","IOR D","CMP D","PSH D",
            "ADD A","SUB A","AND A","IOR A","CMP A","PSH A",
            "JMP","JEQ","JGT","JLT","JSR","JOF","JUF",
            "SDT","LDT",
            "POP A","SSB"]

#Add newline to last line if not present
if(-1!=in_lines[-1]):
    in_lines[-1] = in_lines[-1] + '\n'

#Remove comments
for i in range(len(in_lines)):
    if (-1 != in_lines[i].find("#")):
        pos = in_lines[i].find("#")
        in_lines[i] = (in_lines[i])[:pos] + '\n'

#Remove empty lines
toremove = []
for line in in_lines:
    if line == "\n":
        toremove.append(line)

for line in toremove:
    in_lines.remove(line)


#Assembler instructions to remove pre-assembly
toremove = []

#Remove newline characters from lines
for i in range(len(in_lines)):
    in_lines[i] = (in_lines[i])[:(in_lines[i].find("\n"))]

#Check if line is a lable set instruction
#If its a command adjust post_compile_address accordingly
for line in in_lines:
    #adjust post_compile_address
    for opLen2 in opsLen2:
        if opLen2 in line:
            post_compile_address +=1
            
    #adjust post_compile_address
    for opLen4 in opsLen4:
        if opLen4 in line:
            post_compile_address +=2
            
    #initialise a label
    if line[:10] == "@SET LABEL":
        lables[(line[11:])[:-1]] = post_compile_address
        toremove.append(line)

#remove the lines flagged before
for line in toremove:
    in_lines.remove(line)

#Resolve @Get Lable commands
for i in range(len(in_lines)):
    if "@GET LABEL" in in_lines[i]:
        start = in_lines[i].find("@GET LABEL")
        label = (in_lines[i])[start+11:][:-1]
        address = lables[label]
        in_lines[i] = in_lines[i][:6] + str(address)

# for line in in_lines:
#     print(line)


#Run assembly loop
for line in in_lines:
    instruction = line.split(" ")

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

    elif instruction[0] in ["NOT","PSA","POA","RSR","SRT","FRT","BSR","BSL","HLT","NOP"]:
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

memory += [0 for _ in range(65536*2-len(memory))]

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