#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void printBinNum(int x){
    unsigned i;
    for(i = 1 << 7; i > 0; i = i / 2){
        (x & i) ? printf("1") : printf("0");
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
    char strNetmask[2] = "";
    convertIpHelper(&byte[0], ip);
    convertIpHelper(&byte[1], ip);
    convertIpHelper(&byte[2], ip);
    convertIpHelper(&byte[3], ip);
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
            printf("worong value on adress on byte no.%d (indexed form 0)\n", i);
            return 2;
        }
    }

    if(netmask < 1 || netmask > 32){
        printf("worong value on netmask\n");
        return 3;
    }
    
    printf("Adress: %d.%d.%d.%d ____ ", byte[0], byte[1], byte[2], byte[3]);
    printf("Adress in bin: ");
    //%08b.%08b.%08b.%08b\n", byte[0], byte[1], byte[2], byte[3]);
    printBinNum(byte[0]);
    printf(".");
    printBinNum(byte[1]);
    printf(".");
    printBinNum(byte[2]);
    printf(".");
    printBinNum(byte[3]);
    if(byte[0] < 248){
        printf("      Class %c\n", ((byte[0] < 128) ? 'A' : ((byte[0] < 192) ? 'B' : ((byte[0] < 224) ? 'C' : ((byte[0] < 240) ? 'D' : 'E')))));
    }
    else{
        printf("      Undefined Class\n");
    }
    
    int netmaskByte[4] = {255, 255, 255, 255};
    calculateNetmaskAndNetwork(netmaskByte, netmask);

    printf("Netmask: %d.%d.%d.%d = %d ____ ", netmaskByte[0], netmaskByte[1], netmaskByte[2], netmaskByte[3], netmask);
    printf("Netmask in bin: ");
    //%08b.%08b.%08b.%08b\n", netmaskByte[0], netmaskByte[1], netmaskByte[2], netmaskByte[3]);
    
    printBinNum(netmaskByte[0]);
    printf(".");
    printBinNum(netmaskByte[1]);
    printf(".");
    printBinNum(netmaskByte[2]);
    printf(".");
    printBinNum(netmaskByte[3]);
    printf("\n");

    for(int i = 0; i < 4; i++){
        netmaskByte[i] = netmaskByte[i] ^ 0b11111111;
    }

    printf("Wildcard: %d.%d.%d.%d ____ ", netmaskByte[0], netmaskByte[1], netmaskByte[2], netmaskByte[3]);
    printf("Wildcard in bin: ");
    //%08b.%08b.%08b.%08b\n\n", netmaskByte[0], netmaskByte[1], netmaskByte[2], netmaskByte[3]);

    printBinNum(netmaskByte[0]);
    printf(".");
    printBinNum(netmaskByte[1]);
    printf(".");
    printBinNum(netmaskByte[2]);
    printf(".");
    printBinNum(netmaskByte[3]);
    printf("\n\n");

    int networkByte[4] = {byte[0], byte[1], byte[2], byte[3]};
    calculateNetmaskAndNetwork(networkByte, netmask);

    printf("Network: %d.%d.%d.%d/%d ____ ", networkByte[0], networkByte[1], networkByte[2], networkByte[3], netmask);
    printf("Network in bin: ");
    //%08b.%08b.%08b.%08b\n", networkByte[0], networkByte[1], networkByte[2], networkByte[3]);
    
    printBinNum(networkByte[0]);
    printf(".");
    printBinNum(networkByte[1]);
    printf(".");
    printBinNum(networkByte[2]);
    printf(".");
    printBinNum(networkByte[3]);
    printf("\n");

    int brodcastByte[4] = {byte[0], byte[1], byte[2], byte[3]};
    calculateBrodcast(brodcastByte, netmask);

    printf("Brodcast: %d.%d.%d.%d ____ ", brodcastByte[0], brodcastByte[1], brodcastByte[2], brodcastByte[3]);
    printf("Brodcast in bin: ");
    //%08b.%08b.%08b.%08b\n", brodcastByte[0], brodcastByte[1], brodcastByte[2], brodcastByte[3]);
    
    printBinNum(brodcastByte[0]);
    printf(".");
    printBinNum(brodcastByte[1]);
    printf(".");
    printBinNum(brodcastByte[2]);
    printf(".");
    printBinNum(brodcastByte[3]);
    printf("\n");

    calculateHostMin(networkByte, netmask);

    printf("HostMin: %d.%d.%d.%d ____ ", networkByte[0], networkByte[1], networkByte[2], networkByte[3]);
    printf("HostMin in bin: "); 
    //%08b.%08b.%08b.%08b\n", networkByte[0], networkByte[1], networkByte[2], networkByte[3]);

    printBinNum(networkByte[0]);
    printf(".");
    printBinNum(networkByte[1]);
    printf(".");
    printBinNum(networkByte[2]);
    printf(".");
    printBinNum(networkByte[3]);
    printf("\n");

    calculateHostMax(brodcastByte, netmask);

    printf("HostMax: %d.%d.%d.%d ____ ", brodcastByte[0], brodcastByte[1], brodcastByte[2], brodcastByte[3]);
    printf("HostMax in bin: "); 
    //%08b.%08b.%08b.%08b\n\n", brodcastByte[0], brodcastByte[1], brodcastByte[2], brodcastByte[3]);

    printBinNum(brodcastByte[0]);
    printf(".");
    printBinNum(brodcastByte[1]);
    printf(".");
    printBinNum(brodcastByte[2]);
    printf(".");
    printBinNum(brodcastByte[3]);
    printf("\n\n");

    int host = calculateHost(netmask);

    printf("Hosts/Net: %d\n", host);

    return 0;
}
