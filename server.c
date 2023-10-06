//
// Created by Giacomo Cunardi on 02/10/23.
//

#include <stdio.h>
#include <time.h>

#include "UDP.h"
#include "ZTL.h"

#define DEF_PORTNUMBER 12345
#define DEF_TIMEOUT 1000 //1000ms = 1s

#define DEF_INGRESSO 1
#define DEF_USCITA 0
#define DEF_CNTRL_OK 1
#define DEF_CNTRL_ERR 0

struct srct_Richiesta{
    u_int8_t i8_comando;
    char arr16ch_targa[16];
    u_int16_t i16_minuti;
};

struct srct_Risposta{
    uint8_t i8_comando;
    u_int8_t i8_FlagErrore;
    bool b_multa;
};

int main() {
    uint32_t i32_IpAddress;
    uint16_t i16_PortNumber = DEF_PORTNUMBER;
    uint32_t i32_TimeStamp;
    int i_ControlloCiclo;

    struct srct_Richiesta struct_Richiesta;
    struct srct_Risposta struct_Risposta;

    // inizzializzo il socket UDP

    if (UDP_init(DEF_PORTNUMBER) != 0) {
        printf("Errore inizializzazione socket UDP\n");
        return -101;
    }
    else {
        printf("Socket UDP inizializzato con successo\n");
    }

    while(1)
    {
        // attesa di ricezione di un datagram

        switch(UDP_wait(DEF_TIMEOUT))
        {
            case -1:
                printf("Errore\n");
                UDP_close();
                return -102;
                
            case 0:
                printf("Timeout\n");
                UDP_close();
                return -103;
                
            case 1:
                printf("Ricevuto datagramma\n");
                
                if ((i_ControlloCiclo = UDP_receive(&i32_IpAddress, &i16_PortNumber, (uint8_t*)&struct_Richiesta, sizeof(struct_Richiesta)) == sizeof(struct_Richiesta)) > 0)
                {
                    i32_TimeStamp = time(NULL);
                    
                    struct_Risposta.i8_comando = struct_Richiesta.i8_comando;
                    
                    if(struct_Richiesta.i8_comando == DEF_INGRESSO)
                    {
                        printf("INGRESSO %s\n\r", struct_Richiesta.arr16ch_targa);
                        
                        if(ingressoVeicolo(struct_Richiesta.arr16ch_targa, i32_TimeStamp) == 0)
                        {
                            struct_Risposta.i8_FlagErrore = DEF_CNTRL_OK;
                        }
                        
                        else
                        {
                            struct_Risposta.i8_FlagErrore = DEF_CNTRL_ERR;
                        }
                    }
                    
                    else if(struct_Richiesta.i8_comando == DEF_USCITA) 
                    {
                        printf("USCITA %s\n\r", struct_Richiesta.arr16ch_targa);
                        
                        if(uscitaVeicolo(struct_Richiesta.arr16ch_targa, i32_TimeStamp, &struct_Risposta.b_multa) == 0)
                        {
                            struct_Risposta.i8_FlagErrore = DEF_CNTRL_OK;
                        }
                        else
                        {
                            struct_Risposta.i8_FlagErrore = DEF_CNTRL_ERR;
                        }
                    }
                    
                    UDP_send(i32_IpAddress, i16_PortNumber, (uint8_t*) &struct_Risposta, sizeof(struct_Risposta));
                }
                break;
        }
    }
    UDP_close();
    return 0;
}