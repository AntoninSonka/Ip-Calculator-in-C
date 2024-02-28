#include "ip.h"

int main(int argc, char *argv[]){
    
    //Error handeling and setup

    if(argc != 2){
        printf("wrong number of arguments\n");
        return 1;
    }

    int byte[4];
    int netmask;

    convertIp(byte, &netmask, argv[1]);

    for(int i = 0; i < 4; i++){
        if(byte[i] < 0 || byte[i] > 255){
            printf("wrong value on adress on byte no.%d (indexed form 0)\n", i);
            return 2;
        }
    }

    if(netmask < 1 || netmask > 32){
        printf("wrong value on netmask\n");
        return 3;
    }

    //Actual code
    
    int byte1[] = {byte[0], byte[1], byte[2], byte[3]};
    
    outputIp(byte1, netmask);

    return 0;
}
