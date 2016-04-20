//compile with: gcc PowerOutageEventListener.c rs232.c -Wall -Wextra -o2 -o PowerOutageEventListener

#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>

int main() {
    unsigned long powerOffDurationMs = 5000;

    int cport_nr=5;  /* /dev/ttyS0 (COM1 on windows) */
    int bdrate=9600; /* 9600 baud */
    char mode[]={'8','N','1',0};
    
    unsigned char powerOnMsg = 49;
    unsigned char powerOffMsg = 48;
    unsigned char startCode = 50;
    
    unsigned long powerOffTimeMs = 0;
    unsigned char serialMsg;
    
    
    printf("Opening comport...\n");
    
    if(RS232_OpenComport(cport_nr, bdrate, mode)) {
        printf("Can not open comport\n");
        return(0);
    }
    
    Sleep(1500);
    printf("Okay, let's go!\n");
    
    RS232_SendByte(cport_nr, startCode);
    printf("Waiting for Arduino to respond...\n");
    for(;;) {
        if (RS232_PollComport(cport_nr, &serialMsg, 1)) {
            if (serialMsg == powerOnMsg || serialMsg == powerOffMsg)
                break;
        }
    }
    printf("Arduino responded!\n");
    
    for(;;) {
        if (RS232_PollComport(cport_nr, &serialMsg, 1)) {
            if (serialMsg == powerOnMsg) {
                printf("Power is on!\n");
                powerOffTimeMs = 0;
            } else if (serialMsg == powerOffMsg) {
                if (powerOffTimeMs == 0) {
                    powerOffTimeMs = GetTickCount();
                    printf("Power is off!\n");
                } else if (GetTickCount() > powerOffTimeMs+powerOffDurationMs) {
                    printf("POWER OUTAGE!!!");
                    system("default.cmd");
                    Sleep(5000);
                } else {
                    printf("Power is off!\n");
                }
            } else {
                printf("Received garbage!\n");
            }
            Sleep(100);
        }
    }
    
    return(0);
}