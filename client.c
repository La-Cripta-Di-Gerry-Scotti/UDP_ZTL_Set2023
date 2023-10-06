#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "UDP.h"
#include "ZTL.h"

#define PORT_NUMBER 12345
#define TIMEOUT 1000 // 1000ms = 1s

#define INGRESSO 1
#define USCITA 0

#define OK 1
#define ERRORE 0

struct RICHIESTA{
    uint8_t comando;
    char targa[16];
    uint16_t minuti;
};

struct RISPOSTA{
    uint8_t comando;
    uint8_t errore;
    bool multa;
};

int main(int argc, char* argv[]){
    uint32_t ip_address;
    uint16_t port_number;
    uint32_t timestamp;
    int n;
    struct RISPOSTA risp1;
    struct RISPOSTA risp2;
    struct RICHIESTA r1 = { INGRESSO, "AB123CD", 3};
    struct RICHIESTA r2 = { USCITA, "EF456GH", 16};

    if(argc < 2) {
        printf("Errore argc\r\n");
        return 0;
    }
    ip_address = atoi(argv[1]);

    if(UDP_init(PORT_NUMBER) != 0){
        printf("Errore inizializzzione soxcket");
        return -1;
    }

    UDP_send(ip_address, port_number, (uint8_t *)&r1, sizeof(struct RICHIESTA));

    if(n = (UDP_receive(&ip_address, &port_number, (uint8_t *)&risp1, sizeof(struct RISPOSTA))) > 0){
        if(risp1.errore == 1){
            printf("OK\r\n");
        } else {
            printf("Errore\r\n");
        }
    }

    UDP_send(ip_address, port_number, (uint8_t *)&r2, sizeof(struct RICHIESTA));

    if(n = (UDP_receive(&ip_address, &port_number, (uint8_t *)&risp2, sizeof(struct RISPOSTA) )) > 0){
        if(risp2.errore == 1){
            if(risp2.multa == true){
                printf("%s ha preso una multa\r\n", r2.targa);
            } else{
                printf("%s non ha preso la multa\r\n", r2.targa);
            }
        } else {
            printf("C'è stato un errore\r\n");
        }
    }

    UDP_close();
    return 0;
}