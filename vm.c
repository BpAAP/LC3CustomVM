#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

//Creating Memory
uint16_t memory[UINT16_MAX];

//Creating Registers
enum {acc=0,g1,g2,g3,g4,g5,g6,g7,g8,g9,g10,g11,g12,g13,g14,
            g15,stack_base,stack_pointer,PC
};

uint16_t reg[PC];

//Creating OPcodes
enum {
    HLT=0,ADD,SUB,AND,NOT,IOR,CMP,JMP,JEQ,JGT,JLT,SDT,LDT,MOV,
        PSH,POP,PSA,POA,JSR,RSR,SSB,SRT,JOF,JUF,FRT,BSR,BSL
};

//Creating Flags
enum {
    gt=0,lt,eq,uf,of
};

int flag[of];


uint16_t readAddress(uint16_t address){
    return memory[address];
};

int readImage(const char* path){
    uint16_t origin;
    
    FILE* file = fopen(path,"rb");
    
    if (!file){
        return 0;
    }else{
        
        fread(&origin,1,1,file);
        printf("%i\n",origin);
        fread(memory,sizeof(uint16_t),UINT16_MAX,file);
        fclose(file);
    }
    return 1;
}

int main(int argc,const char* argv[]){

    int debug = 0;

    for (int i = 0; i<=of;i++){
        flag[i] = 0;
    }

    for (int i = 0; i<=PC;i++){
        reg[i] = 0;
    }

    //Check if there are enough arguments
    if (argc > 0){
        //Try to read image file
        if(!readImage(argv[1])){
            printf("Could not load %s\n",(argv[1]));
            exit(1);
        }
        //readImage(argv[1]);
        printf("Reading from %s\n",argv[1]);
    }else{
        printf("No image given\n");
        exit(1);
    }

    //Check if debugging mode is on
    // printf("%s",argv[2]);
    // if (!(argv[2])){
    //     if(!strcmp(argv[2],"-Debug")){
    //         printf("Debugging messages enabled.\n");
    //         debug = 1;
    //     }
    // }
    debug = 1;

    //Address 0 is reset vector, set PC to its value.

    reg[PC] = 0;
    reg[stack_pointer] = UINT16_MAX;
    reg[stack_base] = UINT16_MAX;
    uint16_t address = 0;
    uint16_t data = 0;

    
    int active = 1;
    while (active){
        
        uint16_t current_instruction = memory[reg[PC]];
        uint16_t opcode = current_instruction >> 8;
        
        uint16_t oparg1 = (current_instruction << 8) >> 12;
        uint16_t oparg2 = (current_instruction << 12) >> 12;
        address = 0;
        data = 0;

        printf("Current instruction:%i\n",current_instruction);
        printf("Next instruction:%i\n",memory[reg[PC]+1]);
        printf("Opcode:%i\n",opcode);

        if(debug){printf("PC: %i\n",reg[PC]);}
        switch(opcode){
            case HLT:
                //HLT code
                active = 0;
                if (debug){printf("HLT\n");}                
                break;

            case ADD:
                //ADD CODE
                switch(oparg1){
                    case 0:
                        //Operand is in register
                        data = reg[oparg2];
                        
                        if(debug){printf("ADD| Read %i from register %i\n",data,oparg2);}
                        reg[PC] +=1;
                    case 1:
                        //Operand is from address
                        address = readAddress(reg[PC]+1);
                        data = readAddress(address);
                    
                        if(debug){printf("ADD| Read %i from address %i\n",data,address);}
                        reg[PC] +=2;
                    case 2:
                        //Operand is in next address
                        address = reg[PC]+1;
                        data = readAddress(address);
                    
                        if(debug){printf("ADD| Read %i from (next) address %i\n",data,address);}
                        reg[PC] +=2;
                }
                if (data+reg[acc]>UINT16_MAX){
                    flag[of] = 1;
                    reg[acc] = (reg[acc] + data)%UINT16_MAX;
                    if(debug){printf("Set overflow\n");}
                }
                reg[acc] += data;
                
                break;

            case SUB:
                //SUB CODE
                data;
                switch(oparg1){
                    case 0:
                        //Operand is in register
                        data = reg[oparg2];
                        
                        if(debug){printf("SUB| Read %i from register %i\n",data,oparg2);}
                        reg[PC] +=1;
                    case 1:
                        //Operand is from address
                        address = readAddress(reg[PC]+1);
                        data = readAddress(address);
                        
                        if(debug){printf("SUB| Read %i from address %i\n",data,address);}
                        reg[PC] +=2;
                    case 2:
                        //Operand is in next address
                        address = reg[PC]+1;
                        data = readAddress(address);
                        
                        if(debug){printf("SUB| Read %i from (next) address %i\n",data,address);}
                        reg[PC] +=2;
                }
                if(reg[acc]-data < 0){
                    reg[acc] = (reg[acc] - data);
                    flag[uf] = 1;
                    if(debug){printf("Set underflow\n");}
                }
                reg[acc] -= data;                
                break;

            case AND:
                //AND CODE
                switch(oparg1){
                    case 0:
                        //Operand is in register
                        data = reg[oparg2];
                        reg[acc] = reg[acc] & data;
                        if(debug){printf("AND| Read %i from register %i\n",data,oparg2);}
                        reg[PC] +=1;
                    case 1:
                        //Operand is from address
                        address = readAddress(reg[PC]+1);
                        data = readAddress(address);
                        reg[acc] = reg[acc] & data;
                        if(debug){printf("AND| Read %i from address %i\n",data,address);}
                        reg[PC] +=2;
                    case 2:
                        //Operand is in next address
                        address = reg[PC]+1;
                        data = readAddress(address);
                        reg[acc] = reg[acc] & data;
                        if(debug){printf("AND| Read %i from (next) address %i\n",data,address);}
                        reg[PC] +=2;
                }                
                break;

            case NOT:
                //NOT CODE
                reg[acc] = ~reg[acc];
                if(debug){printf("NOT\n");}
                reg[PC] +=1;                
                break;

            case IOR:
                //IOR CODE
                switch(oparg1){
                    case 0:
                        //Operand is in register
                        data = reg[oparg2];
                        reg[acc] = reg[acc] | data;
                        if(debug){printf("IOR| Read %i from register %i\n",data,oparg2);}
                        reg[PC] +=1;
                    case 1:
                        //Operand is from address
                        address = readAddress(reg[PC]+1);
                        data = readAddress(address);
                        reg[acc] = reg[acc] | data;
                        if(debug){printf("IOR| Read %i from address %i\n",data,address);}
                        reg[PC] +=2;
                    case 2:
                        //Operand is in next address
                        address = reg[PC]+1;
                        data = readAddress(address);
                        reg[acc] = reg[acc] | data;
                        if(debug){printf("IOR| Read %i from (next) address %i\n",data,address);}
                        reg[PC] +=2;
                }          
                break;

            case CMP:
                //CMP CODE
                switch(oparg1){
                    case 0:
                        //Operand is in register
                        data = reg[oparg2];
                        if(debug){printf("CMP| Read %i from register %i\n",data,oparg2);}
                        reg[PC] +=1;
                    case 1:
                        //Operand is from address
                        address = readAddress(reg[PC]+1);
                        data = readAddress(address);
                        
                        if(debug){printf("CMP| Read %i from address %i\n",data,address);}
                        reg[PC] +=2;
                    case 2:
                        //Operand is in next address
                        address = reg[PC]+1;
                        data = readAddress(address);
                        
                        if(debug){printf("CMP| Read %i from (next) address %i\n",data,address);}
                        reg[PC] +=2;
                }
                if(reg[acc]>data){
                    flag[gt] = 1;
                    if(debug){printf("Set greater-than\n");}
                } else if (reg[acc]<data){
                    flag[lt] = 1;
                    if(debug){printf("Set less-than\n");}
                } else if (reg[acc]=data){
                    flag[eq] = 1;
                    if(debug){printf("Set equal\n");}
                }
                break;

            case JMP:
                //JMP CODE
                address = readAddress(reg[PC]+1);
                reg[PC] = address;
                if(debug){printf("JMP| Jumped to %i\n",address);}
                break;

            case JEQ:
                //JEQ CODE
                if(flag[eq]){
                    address = readAddress(reg[PC]+1);
                    reg[PC] = address;
                    reg[eq] = 0;
                    if(debug){printf("JEQ| Jumped to %i\n",address);}
                } else{
                    reg[PC] +=2;
                    if(debug){printf("JEQ| Didn't jump\n");}
                }
                break;

            case JGT:
                //JGT CODE
                if(flag[gt]){
                    address = readAddress(reg[PC]+1);
                    reg[PC] = address;
                    reg[gt] = 0;
                    if(debug){printf("JGT| Jumped to %i\n",address);}
                } else{
                    reg[PC] +=2;
                    if(debug){printf("JGT| Didn't jump\n");}
                }
                break;

            case JLT:
                //JLT CODE
                if(flag[lt]){
                    address = readAddress(reg[PC]+1);
                    reg[PC] = address;
                    reg[lt] = 0;
                    if(debug){printf("JLT| Jumped to %i\n",address);}
                } else{
                    reg[PC] +=2;
                    if(debug){printf("JLT| Didn't jump\n");}
                }
                break;

            case SDT:
                //SDT CODE
                address = readAddress(reg[PC]+1);
                data = reg[oparg2];
                if(debug){printf("SDT| Saved %i from register %i to address %i\n",data,oparg2,address);}
                memory[address] = data;
                reg[PC] += 2;
                break;

            case LDT:
                //LDT CODE
                address = readAddress(reg[PC]+1);
                data = readAddress(address);
                if(debug){printf("LDT| Loaded %i from address %i to register %i\n",data,address,oparg2);}
                reg[oparg2] = data;
                reg[PC] += 2;
                break;

            case MOV:
                //MOV CODE
                data = reg[oparg1];
                if(debug){printf("MOV| Moved %i from register %i to register %i\n",data,oparg1,oparg2);}
                reg[oparg2] = reg[oparg1];
                reg[PC] +=1;
                break;

            case PSH:
                //PSH CODE
                data;
                switch(oparg1){
                    case 0:
                        //Operand is in register
                        data = reg[oparg2];
                        
                        if(debug){printf("PSH| Read %i from register %i\n",data,oparg2);}
                        reg[PC] +=1;
                    case 1:
                        //Operand is from address
                        address = readAddress(reg[PC]+1);
                        data = readAddress(address);
                        
                        if(debug){printf("PSH| Read %i from address %i\n",data,address);}
                        reg[PC] +=2;
                    case 2:
                        //Operand is in next address
                        address = reg[PC]+1;
                        data = readAddress(address);
                        
                        if(debug){printf("PSH| Read %i from (next) address %i\n",data,address);}
                        reg[PC] +=2;
                }
                memory[reg[stack_pointer]] = data;
                reg[stack_pointer] -= 1;
                break;

            case POP:
                //POP CODE
                switch(oparg1){
                    case 0:
                        //Operand is in register
                        data = memory[reg[stack_pointer]];
                        reg[oparg2] = data;
                        if(debug){printf("POP| Wrote %i to register %i\n",data,oparg2);}
                        reg[PC] +=1;
                    case 1:
                        //Operand is from address
                        address = readAddress(reg[PC]+1);
                        data = memory[reg[stack_pointer]];
                        memory[address] = data;
                        if(debug){printf("POP| Wrote %i to address %i\n",data,address);}
                        reg[PC] +=2;
                }
                if (!(reg[stack_pointer]==reg[stack_base])){
                    reg[stack_pointer] -= 1;
                }             
                break;

            case PSA:
                //PSA CODE
                for (int i = 0; i<=g15;i++){
                    memory[stack_pointer] = reg[i];
                    reg[stack_pointer] -= 1;
                }
                if(debug){printf("PSA| Pushed all to stack\n");}
                reg[PC] +=1;
                break;

            case POA:
                //POA CODE
                for (int i = 0; i<=g15;i++){
                    reg[i] = memory[stack_pointer];
                    reg[stack_pointer] += 1;
                }
                if(debug){printf("POA| Pulled all\n");}
                reg[PC] +=1;
                break;

            case JSR:
                //JSR CODE
                address = readAddress(reg[PC]+1);
                memory[reg[stack_pointer]] = reg[PC]+2;
                reg[stack_pointer] -= 1;
                reg[PC] = address;
                if(debug){printf("JSR| Jumped to subroutine at %i\n",address);}
                break;

            case RSR:
                //RSR CODE
                address = readAddress(reg[stack_pointer]);
                reg[stack_pointer] += 1;
                reg[PC] = address;
                if(debug){printf("RSR| Returned from subroutine to %i\n",address);}
                break;

            case SSB:
                //SSB CODE
                switch(oparg1){
                    case 0:
                        //From value at address
                        address = readAddress(reg[PC]+1);
                        data = readAddress(address);
                        reg[stack_base] = data;
                        if(debug){printf("SSB| Set stack base to %i from address %i\n",data,address);}
                    case 1:
                        //From next address
                        address = reg[PC]+1;
                        data = readAddress(address);
                        reg[stack_base] = data;
                        if(debug){printf("SSB| Set stack base to %i from (next) address %i\n",data,address);}    
                }
                reg[PC] +=2;
                break;

            case SRT:
                //SRT CODE
                reg[stack_pointer] = reg[stack_base];
                if(debug){printf("SRT| Reset stack pointer to %i\n",reg[stack_base]);}
                reg[PC] +=1;
                break;

            case JOF:
                //JOF CODE
                if(flag[of]){
                    address = readAddress(reg[PC]+1);
                    reg[PC] = address;
                    reg[of] = 0;
                    if(debug){printf("JOF| Jumped to %i\n",address);}
                } else{
                    reg[PC] +=2;
                    if(debug){printf("JOF| Didn't jump\n");}
                }
                break;

            case JUF:
                //JUF CODE
                if(flag[uf]){
                    address = readAddress(reg[PC]+1);
                    reg[PC] = address;
                    reg[uf] = 0;
                    if(debug){printf("JUF| Jumped to %i\n",address);}
                } else{
                    reg[PC] +=2;
                    if(debug){printf("JUF| Didn't jump\n");}
                }
                break;

            case FRT:
                //FRT CODE
                for (int i = 0; i<=of;i++){
                    flag[i] = 0;
                }
                reg[PC] +=1;
                if(debug){printf("FRT| reset all flags\n");}
                break;

            case BSR:
                //BSR CODE
                reg[acc] = reg[acc] >> 1;
                reg[PC] += 1;
                if(debug){printf("BSR| Bit shift right\n");}
                break;

            case BSL:
                //BSL CODE
                reg[acc] = reg[acc] << 1;
                reg[PC] += 1;
                if(debug){printf("BSL| Bit shift left\n");}
                break;

            default:
                active = 0;
                printf("ERROR: DEFAULT CASE TRIGGERED\n");
                break;

        }
    }
}