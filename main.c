#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void printBinNum(int x){
    for(int i = 8; i > 0; i--){
        printf("%d", (x & (1 << (i - 1))) ? 1 : 0);
    }
}

void printIpDec(int* byte, char* type){
    for(int i = 0; i < 4; i++){
        printf("%03d", byte[i]);
        if(i != 3){
            printf(".");
        }
    }
}

void printIpBin(int* byte, char* type){
    for(int i = 0; i < 4; i++){
        printBinNum(byte[i]);
        if(i != 3){
            printf(".");
        }
    }
}

void convertIpHelper(int* a, char* ip){
    char strA[5] = "";
    static int i = 0;
    int j = 0;
    for(; (ip[i] != '.' && ip[i] != '/') && ip[i] != '\0'; i++){
        strA[j] = ip[i];
        j++;
    }
    i++;
    strA[j] = '\0';
    *a = atoi(strA);
}

void convertIp(int* byte, int* netmask, char* ip){
    for(int i = 0; i < 4; i++){
        convertIpHelper(&byte[i], ip);
    }
    convertIpHelper(netmask, ip);
}

void calculateNetmaskAndNetwork(int* byte, int netmask){
    netmask = 32 - netmask;

    for(int i = 3; i >= 0; i--){
        if(netmask >= 8){
            netmask -= 8;
            byte[i] = 0;
        }
        else if(netmask > 0){
            byte[i] = byte[i] >> netmask;
            byte[i] = byte[i] << netmask;
            netmask = 0;
            return;
        }
        else{
            return;
        }
    }
}

void calculateBrodcast(int* byte, int netmask){
    netmask = 32 - netmask;

    for(int i = 3; i >= 0; i--){
        if(netmask >= 8){
            netmask -= 8;
            byte[i] = 255;
        }
        else if(netmask > 0){
            int x = (1 << netmask) - 1;
            byte[i] = byte[i] | x;
            netmask = 0;
            return;
        }
        else{
            return;
        }
    }
}

void calculateHostMin(int* byte, int netmask){
    if(netmask == 32){
        return;
    }
    byte[3] += 1;
}

void calculateHostMax(int* byte, int netmask){
    if(netmask == 32){
        return;
    }
    byte[3] -= 1;
}

int calculateHost(int netmask){
    return ((pow(2, 32 - netmask) - 2) + ((netmask == 32) ? 2 : 0));
}

int main(int argc, char *argv[]){

    if(argc != 2){
        printf("spatny pocet argumentu\n");
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

    if(byte[0] < 248){
        printf("(Class %c)\n\n", ((byte[0] < 128) ? 'A' : ((byte[0] < 192) ? 'B' : ((byte[0] < 224) ? 'C' : ((byte[0] < 240) ? 'D' : 'E')))));
    }
    else{
        printf("(Undefined Class)\n\n");
    }

    printf("Adress:   ");
    printIpDec(byte, "Adress");
    printf("           ");
    printIpBin(byte, "Adress");
    printf("\n");
    
    int netmaskByte[4] = {255, 255, 255, 255};
    calculateNetmaskAndNetwork(netmaskByte, netmask);
    
    printf("Netmask:  ");
    printIpDec(netmaskByte, "Netmask");
    printf(" = %d      ", netmask);
    printIpBin(netmaskByte, "Netmask");
    printf("\n");

    for(int i = 0; i < 4; i++){
        netmaskByte[i] = netmaskByte[i] ^ 0b11111111;
    }
    
    printf("WildCard: ");
    printIpDec(netmaskByte, "Wildcard");
    printf("           ");
    printIpBin(netmaskByte, "Wildcard");
    printf("\n\n");

    int networkByte[4] = {byte[0], byte[1], byte[2], byte[3]};
    calculateNetmaskAndNetwork(networkByte, netmask);
    
    printf("Network:  ");
    printIpDec(networkByte, "Network");
    printf("           ");
    printIpBin(networkByte, "Network");
    printf("\n");

    int brodcastByte[4] = {byte[0], byte[1], byte[2], byte[3]};
    calculateBrodcast(brodcastByte, netmask);
    
    printf("Brodcast: ");
    printIpDec(brodcastByte, "Brodcast");
    printf("           ");
    printIpBin(brodcastByte, "Brodcast");
    printf("\n");

    calculateHostMin(networkByte, netmask);

    printf("HostMin:  ");
    printIpDec(networkByte, "HostMin");
    printf("           ");
    printIpBin(networkByte, "HostMin");
    printf("\n");

    calculateHostMax(brodcastByte, netmask);
    
    printf("HostMax:  ");
    printIpDec(brodcastByte, "HostMax");
    printf("           ");
    printIpBin(brodcastByte, "HostMax");
    printf("\n\n");

    printf("Hosts/Net: %d\n", calculateHost(netmask));

    return 0;
}
