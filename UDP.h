#include <stdint.h>
#include <stdint.h>
#include <stdbool.h>

// converte un indirizzo IP nella notazione ASCII decimale usuale (esempio:
// "192.168.1.1") in un valore numerico a 32 bit
uint32_t IP_to_bin(char ip_address[]);

// inizializza il socket impostando il numero di porta locale
// restituisce -1 in caso di errore, 0 in caso di successo
int UDP_init(uint16_t port_number);

// trasmette al socket identificato dall'indirizzo IP e dal numero di porta UDP un
// numero byte di dati contenuti nel buffer data
// restituisce -1 in caso di errore, altrimenti il numero di byte trasmessi
int UDP_send( uint32_t ip_address, uint16_t port_number, uint8_t data[], int byte);

// riceve un datagram nel buffer data di dimensione size
// restituisce -1 in caso di errore o di dati non disponibili,
// il numero di byte ricevuti in caso di successo
// indirizzo IP e numero di porta UDP del mittente sono restituiti come parametri
int UDP_receive( uint32_t *ip_address, uint16_t *port_number, uint8_t data[], int size);

// attende la disponibilita' di dati ricevuti fino a un tempo massimo espresso in
// millisecondi
// restituisce -1 in caso di errore, 0 in caso di timeout,
// 1 se sono stati ricevuti dati
int UDP_wait(unsigned long millisecond);

// chiude il socket
void UDP_close(void );
