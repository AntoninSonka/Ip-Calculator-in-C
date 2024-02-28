#ifndef IP_H
#define IP_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void printBinNum(int x);
void printIpDec(int* byte);
void printIpBin(int* byte);
void convertIp(int* byte, int* netmask, char* ip);
void calculateNetmaskAndNetwork(int* byte, int netmask);
void calculateBrodcast(int* byte, int netmask);
void calculateHostMin(int* byte, int netmask);
void calculateHostMax(int* byte, int netmask);
int calculateHost(int netmask);
void outputIp(int* byte, int netmask);

#endif // !IP_H
