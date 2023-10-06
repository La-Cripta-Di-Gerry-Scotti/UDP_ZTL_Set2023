#include <stdlib.h>
#include "ZTL.h"

// Return = 1 OK
// Return = 0 ERR
int ingressoVeicolo(char targa[], unsigned long timestamp){
    return 1;
}

// Return = 1 OK
// Return = 0 ERR
int uscitaVeicolo(char targa[], unsigned long timestamp, bool* multa)
{
    if(rand()%350 >= 15)
        *multa = true;
    else
        *multa = false;
    return 1;
}