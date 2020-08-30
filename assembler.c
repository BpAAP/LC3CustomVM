#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

const char *opStrs[27];
void setOpStrs(){
    opStrs[0]="HLT";
    opStrs[1]="ADD";
    opStrs[2]="SUB";
    opStrs[3]="AND";
    opStrs[4]="NOT";
    opStrs[5]="IOR";
    opStrs[6]="CMP";
    opStrs[7]="JMP";
    opStrs[8]="JEQ";
    opStrs[9]="JGT";
    opStrs[10]="JLT";
    opStrs[11]="SDT";
    opStrs[12]="LDT";
    opStrs[13]="MOV";
    opStrs[14]="PSH";
    opStrs[15]="POP";
    opStrs[16]="PSA";
    opStrs[17]="POA";
    opStrs[18]="JSR";
    opStrs[19]="RSR";
    opStrs[20]="SSB";   
    opStrs[21]="SRT";
    opStrs[22]="JOF";
    opStrs[23]="JUF";
    opStrs[24]="FRT";
    opStrs[25]="BSR";
    opStrs[26]="BSL";
}


int parseOpcode(char line[256]){
    char opStr[3];
    opStr[0] = line[0];
    opStr[1] = line[1];
    opStr[2] = line[2];
    int opCode;

    for (int i = 0; i<27;i++){
        if(!strcmp(opStr, opStrs[i])){
            return i;
        }
    } 

    printf("Couldnt parse opcode.\n");
    exit(2);
}

int main(int argc,const char* argv[]){
    setOpStrs();

    if(argc<2){
        printf("Too few arguments.\n");
        exit(1);
    }
    int16_t* memory;
    
    memory = (int16_t *) calloc(UINT16_MAX,sizeof(int16_t));
    
    FILE* in_file;

    in_file = fopen(argv[1],"r");
    if(!in_file){
        printf("Couldn't open first file.\n");
        exit(1);
    }

    FILE* out_file;
    out_file = fopen(argv[2],"wb");
    if(!out_file){
        printf("Couldn't open second file.\n");
        printf("Might need to make an empty file.\n");
        exit(1);
    }

    char line[256];
    while (fgets(line,sizeof(line),in_file)){
        int opcode = 0;
        opcode = parseOpcode(line);
        printf("%s \n",opStrs[opcode]);
    }

    // memory[0] = (1<<8);
    // printf("%i\n",memory[0]);
    // printf("%i\n",memory[UINT16_MAX]);
    // printf("%i\n",memory[1]);
    // fwrite(memory,sizeof(uint16_t),UINT16_MAX,file);
 
    fclose(in_file);
    fclose(out_file);
    free(memory);
    
    return(0);
}