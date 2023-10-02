//
// Created by Giacomo Cunardi on 02/10/23.
//

#include <stdbool.h>
#include <stdlib.h>


int ingressoVeicolo(char targa[], unsigned long timestamp)
{
    return 0;
}

int uscitaVeicolo(char targa[], unsigned long timestamp, bool* multa)
{
    // Gestire 15 min
    if(rand()%1440 >= 15){
        *multa = true;
    }
    else{
        *multa = false;
    }


}
