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
    HLT=0,ADD,SUB,AND,NOT,OR,CMP,JMP,JEQ,JGT,JLT,SDT,LDT,MOV,
        PSH,POP,PSA,POA,JSR,RSR,SSB,SRT,JOF,JUF,FRT,BSR,BSL
};

//Creating Flags
enum {
    fl_gt=0,fl_lt=0,fl_eq=0,fl_uf=0,fl_of=0
};

int16_t readAddress(int16_t address){
    return memory[address];
};

int readImage(const char* path){
    FILE* file = fopen(path,"rb");
    if (!file){
        return 0;
    }else{
        //pointer to current address in memory
        fread(memory,sizeof(uint16_t),UINT16_MAX,file);
        fclose(file);
    }
    return 1;
}

int main(int argc,const char* argv[]){

    //Check if there are enough arguments
    if (argc < 1){
        printf("Failed to load image");
        exit(2);
    }

    //Try to read image file
    for (int i=1;i<argc;i++){
        if(!readImage(argv[i])){
            printf("Could not load %s\n",(argv[i]));
            exit(1);
        }
    }

    //Address 0 is reset vector, set PC to its value.

    reg[PC] = 0;

    int active = 1;
    while (active){
        uint16_t current_instruction = readAddress(reg[PC]);
        uint16_t opcode = current_instruction >> 8;
        
        uint16_t opargs = (current_instruction << 8) >> 8;
        printf("opcode: %i, oparg: %i\n",opcode,opargs);
        printf("%i\n",current_instruction);
        switch(opcode){
            case HLT:
                active = 0;
                printf("HLT");
                break;
            case ADD:
                //ADD CODE
                printf("ADD");
                break;
            case SUB:
                //SUB CODE
                printf("SUB");
                break;
            case AND:
                //AND CODE
                printf("AND");
                break;
            case NOT:
                //NOT CODE
                printf("NOT");
                break;
            case OR:
                //OR CODE
                printf("OR");
                break;
            case CMP:
                printf("CMP");
                //CMP CODE
                break;
            case JMP:
                printf("JMP");
                //JMP CODE
                break;
            case JEQ:
                printf("JEQ");
                //JEQ CODE
                break;
            case JGT:
                printf("JGT");
                //JGT CODE
                break;
            case JLT:
                printf("JLT");
                //JLT CODE
                break;
            case SDT:
                printf("SDT");
                //SDT CODE
                break;
            case LDT:
                printf("LDT");
                //LDT CODE
                break;
            case MOV:
                printf("MOV");
                //MOV CODE
                break;
            case PSH:
                printf("PSH");
                //PSH CODE
                break;
            case POP:
                printf("POP");
                //POP CODE
                break;
            case PSA:
                printf("PSA");
                //PSA CODE
                break;
            case POA:
                printf("POA");
                //POA CODE
                break;
            case JSR:
                printf("JSR");
                //JSR CODE
                break;
            case RSR:
                printf("RSR");
                //RSR CODE
                break;
            case SSB:
                printf("SSB");
                //SSB CODE
                break;
            case SRT:
                printf("SRT");
                //SRT CODE
                break;
            case JOF:
                printf("JOF");
                //JOF CODE
                break;
            case JUF:
                printf("JUF");
                //JUF CODE
                break;
            case FRT:
                printf("FRT");
                //FRT CODE
                break;
            case BSR:
                printf("BSR");
                //BSR CODE
                break;
            case BSL:
                printf("BSL");
                //BSL CODE
                break;

            default:
                active = 0;
                printf("Got Default");
                break;

        }
    }
}