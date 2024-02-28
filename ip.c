#include "ip.h"


void printBinNum(int x){
    for(int i = 8; i > 0; i--){
        printf("%d", (x & (1 << (i - 1))) ? 1 : 0);
    }
}

void printIpDec(int* byte){
    for(int i = 0; i < 4; i++){
        printf("%03d", byte[i]);
        if(i != 3){
            printf(".");
        }
    }
}

void printIpBin(int* byte){
    for(int i = 0; i < 4; i++){
        printBinNum(byte[i]);
        if(i != 3){
            printf(".");
        }
    }
}

void _convertIp(int* a, char* ip){
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
        _convertIp(&byte[i], ip);
    }
    _convertIp(netmask, ip);
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

void outputIp(int* byte, int netmask){
    if(byte[0] < 248){
        printf("(Class %c)\n\n", ((byte[0] < 128) ? 'A' : ((byte[0] < 192) ? 'B' : ((byte[0] < 224) ? 'C' : ((byte[0] < 240) ? 'D' : 'E')))));
    }
    else{
        printf("(Undefined Class)\n\n");
    }

    printf("Adress:   ");
    printIpDec(byte);
    printf("           ");
    printIpBin(byte);
    printf("\n");
    
    int netmaskByte[4] = {255, 255, 255, 255};
    calculateNetmaskAndNetwork(netmaskByte, netmask);
    
    printf("Netmask:  ");
    printIpDec(netmaskByte);
    printf(" = %d      ", netmask);
    printIpBin(netmaskByte);
    printf("\n");

    for(int i = 0; i < 4; i++){
        netmaskByte[i] = netmaskByte[i] ^ 0b11111111;
    }
    
    printf("WildCard: ");
    printIpDec(netmaskByte);
    printf("           ");
    printIpBin(netmaskByte);
    printf("\n\n");

    int networkByte[4] = {byte[0], byte[1], byte[2], byte[3]};
    calculateNetmaskAndNetwork(networkByte, netmask);
    
    printf("Network:  ");
    printIpDec(networkByte);
    printf("           ");
    printIpBin(networkByte);
    printf("\n");

    int brodcastByte[4] = {byte[0], byte[1], byte[2], byte[3]};
    calculateBrodcast(brodcastByte, netmask);
    
    printf("Brodcast: ");
    printIpDec(brodcastByte);
    printf("           ");
    printIpBin(brodcastByte);
    printf("\n");

    calculateHostMin(networkByte, netmask);

    printf("HostMin:  ");
    printIpDec(networkByte);
    printf("           ");
    printIpBin(networkByte);
    printf("\n");

    calculateHostMax(brodcastByte, netmask);
    
    printf("HostMax:  ");
    printIpDec(brodcastByte);
    printf("           ");
    printIpBin(brodcastByte);
    printf("\n\n");

    printf("Hosts/Net: %d\n", calculateHost(netmask));
}
