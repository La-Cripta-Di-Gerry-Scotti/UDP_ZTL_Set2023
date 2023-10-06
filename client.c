#include <stdio.h>

#include <stdbool.h>
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
    uint16_t i16_port_number = DEF_PORTNUMBER;
    uint32_t i32_timestamp;
    int n;

    short sh_i = 0;

    struct srct_Richiesta struct_Richiesta[] = {DEF_INGRESSO, "AB123CD", 3, DEF_USCITA, "EF456GH", 16};
    struct srct_Risposta struct_Risposta[] = {0};
    
    
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

    i32_ip_address = IP_to_bin(argv[1]);

    while(struct_Richiesta[sh_i].i8_comando == DEF_INGRESSO || struct_Richiesta[sh_i].i8_comando == DEF_USCITA)
    {
        switch (struct_Richiesta[sh_i].i8_comando)
        {
            case DEF_USCITA:
                UDP_send(i32_ip_address, i16_port_number, (uint8_t *)&struct_Richiesta[sh_i], sizeof(struct_Richiesta[sh_i]));

                if(n = (UDP_receive(&i32_ip_address, &i16_port_number, (uint8_t *)&struct_Risposta[sh_i], sizeof(struct_Risposta[sh_i]))) > 0)
                {
                    if(struct_Risposta[sh_i].i8_FlagErrore == 1)
                    {
                        if(struct_Risposta[sh_i].b_multa == true)
                        {
                            printf("%s ha preso una multa\r\n", struct_Richiesta[sh_i].arr16ch_targa);
                        }
                        else
                        {
                            printf("%s non ha preso la multa\r\n",struct_Richiesta[sh_i].arr16ch_targa);
                        }
                    }
                    else
                    {
                        printf("C'è stato un errore\r\n");
                    }
                }

            case DEF_INGRESSO:
                UDP_send(i32_ip_address, i16_port_number, (uint8_t *)&struct_Richiesta[sh_i], sizeof(struct_Richiesta[sh_i]));

                if(n = (UDP_receive(&i32_ip_address, &i16_port_number, (uint8_t *)&struct_Risposta[sh_i], sizeof(struct_Risposta[sh_i]))) > 0)
                {
                    if(struct_Risposta[sh_i].i8_FlagErrore == 1)
                    {
                        printf("OK\r\n");
                    }

                    else
                    {
                        printf("Errore\r\n");
                    }
                }
        }

        sh_i++;
    }


    UDP_close();
    return 0;
}
