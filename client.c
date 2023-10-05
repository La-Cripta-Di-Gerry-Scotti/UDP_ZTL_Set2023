#include <stdio.h>


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

int main(int argc, char *argv[])
{
    uint32_t i32_ip_address;
    uint16_t i16_port_number;
    uint32_t i32_timestamp;
    int n;

    struct srct_Richiesta struct_Richiesta1 = {DEF_INGRESSO, "AB123CD", 3};
    struct srct_Richiesta struct_Richiesta2 = {DEF_USCITA, "EF456GH", 16};
    struct srct_Risposta struct_Risposta1;
    struct srct_Risposta struct_Risposta2;
    
    
    if (argc!= 2)
    {
        printf("ERR 201: argc non valido\n");
        return -201;
    }

    if(UDP_init(DEF_PORTNUMBER) != 0)
    {
        printf("Errore inizializzazione socket\n");
        return -202;
    }

    UDP_send(i32_ip_address, i16_port_number, (uint8_t *)&struct_Richiesta1, sizeof(struct_Richiesta1));

    if(n = (UDP_receive(&i32_ip_address, &i16_port_number, (uint8_t *)&struct_Risposta1, sizeof(struct_Risposta1))) > 0)
    {
        if(struct_Risposta1.i8_FlagErrore == 1)
        {
            printf("OK\r\n");
        }

        else
        {
            printf("Errore\r\n");
        }
    }

    UDP_send(i32_ip_address, i16_port_number, (uint8_t *)&struct_Richiesta2, sizeof(struct_Richiesta2));

    if(n = (UDP_receive(&i32_ip_address, &i16_port_number, (uint8_t *)&struct_Risposta2, sizeof(struct_Risposta2))) > 0)
    {
        if(struct_Risposta2.i8_FlagErrore == 1)
        {
            if(struct_Risposta2.b_multa == true)
            {
                printf("%s ha preso una multa\r\n", struct_Richiesta2.arr16ch_targa);
            }
            else
            {
                printf("%s non ha preso la multa\r\n", struct_Richiesta2.arr16ch_targa);
            }
        }
        else
        {
            printf("C'è stato un errore\r\n");
        }
    }

    UDP_close();
    return 0;
}
