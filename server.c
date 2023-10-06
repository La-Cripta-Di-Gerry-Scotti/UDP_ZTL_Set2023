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
    struct RISPOSTA risposta;
    struct RICHIESTA richiesta;

    //inizializzo socket
    if(UDP_init(PORT_NUMBER) < 0){
        printf("Errore apertura socket!\r\n");
        return -1;
    }

    printf("Service Activated\r\n");

    while(1){
        //attesa di ricezione di un datagram

        if(UDP_wait(TIMEOUT) > 0){
            //VERIFICO I DATGRAM
            if(n = UDP_receive(&ip_address, &port_number, (uint8_t *)&richiesta, sizeof(struct RICHIESTA)) == sizeof(struct RICHIESTA) > 0){
                timestamp = time(NULL);
                risposta.comando = richiesta.comando;
                if(richiesta.comando == INGRESSO){
                    printf("INGRESSO %s\r\n", richiesta.targa);
                    if(ingressoVeicolo(richiesta.targa, timestamp) == 0){
                        risposta.errore =OK;
                    } else {
                        risposta.errore = ERRORE;
                    }
                } else if(richiesta.comando == USCITA){
                    printf("USCITA %s\r\n", richiesta.targa);
                    if(uscitaVeicolo(richiesta.targa, timestamp, &risposta.multa) == 0){
                        risposta.errore =OK;
                    } else {
                        risposta.errore = ERRORE;
                    }
                }
                UDP_send(ip_address, port_number,(uint8_t *)&risposta, sizeof(struct RISPOSTA));
            }
        } else {
            UDP_close();
            return 0;
        }
    }
    UDP_close();
    return 0;
}