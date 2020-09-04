//modified from https://www.geeksforgeeks.org/socket-programming-cc/ 
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 

#include <stdlib.h>
#include <stdint.h>

//these includes are only for the random bit of the demo
#include <time.h>
#include <stdlib.h>



#define PORT 25560
   
int main(int argc, char const *argv[]) 
{ 
    int sock = 0; 
    struct sockaddr_in serv_addr; 
    int16_t partA = 0;
    int16_t partB = 0;
    partA += (1<<8)+1+(1<<13)+(1<<5);  //Command for coloring pixel (1,1) (x,y)
    partB += (15);     //Make it blue      

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    }
    
    int net_partA = htons(partA);
    int net_partB = htons(partB);
    send(sock , &net_partA, sizeof(partA), 0); 
    send(sock , &net_partB, sizeof(partB), 0);
    printf("Sent colour test.\n"); 


    partA = (2<<13)+(2<<5)+(2<<8)+2;  //Command for writing character to pixel (2,2) (x,y)
    partB = (3<<10) + 65;     //Make it green and an 'A'
    net_partA = htons(partA);
    net_partB = htons(partB);
    send(sock , &net_partA, sizeof(partA), 0); 
    send(sock , &net_partB, sizeof(partB), 0);
    printf("Sent character test.\n"); 

    srand(time(NULL));   // Initialization, should only be called once.
 
    
    
    //throw some random characters around
    for (int i =0;i<10;i++){
        
        int x = rand()%32;    
        int y = rand()%32;
        int b = rand()%64;
        int c = rand()%128 + 2;
        partA = (2<<13) + (2<<5) + (x<<8) + y;
        partB = (b<<8) + c;
        net_partA = htons(partA);
        net_partB = htons(partB);
        send(sock , &net_partA, sizeof(partA), 0); 
        send(sock , &net_partB, sizeof(partB), 0);

       
                
    }
    return 0; 
} 