#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


int main(){
    int16_t* memory;
    
    memory = (int16_t *) calloc(UINT16_MAX,sizeof(int16_t));
    
    FILE* file;
    file = fopen("test.bin","wb");
   
    memory[0] = (1<<8);
    printf("%i\n",memory[0]);
    printf("%i\n",memory[UINT16_MAX]);
    printf("%i\n",memory[1]);
    fwrite(memory,sizeof(uint16_t),UINT16_MAX,file);
 
    fclose(file);
    free(memory);
    
    return(0);
}