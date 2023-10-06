#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <stdint.h>
#include <stdbool.h>

#include "UDP.h"

int socket_id; // identificatore del socket

uint32_t IP_to_bin(char ip_address[])
{
	uint32_t bin_address = inet_network(ip_address);
	return bin_address;
}

int UDP_init(uint16_t port_number)
{
	struct sockaddr_in add; // struttura per indirizzo
	
	// apertura socket UDP non bloccante
	if ((socket_id = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, IPPROTO_UDP)) < 0)
	  return -1;
	// costruzione struttura indirizzo
	memset(&add, 0, sizeof(add)); // azzeramento struttura
	add.sin_family = AF_INET; // dominio indirizzi IP
	add.sin_addr.s_addr = 0; // indirizzo IP locale
	add.sin_port = htons(port_number); // numero di porta assegnato
	// associazione socket a numero di porta
	if (bind(socket_id, (struct sockaddr *)&add, sizeof(add)) < 0)
	{
	  close(socket_id);
	  return -1;
	}
	return 0;
}

int UDP_send( uint32_t ip_address, uint16_t port_number, uint8_t data[], int byte)
{
	struct sockaddr_in add; // struttura per indirizzo di destinazione
	int n;
	
	// costruzione struttura indirizzo
	memset(&add, 0, sizeof(add)); // azzeramento struttura
	add.sin_family = AF_INET; // dominio indirizzi IP
	add.sin_port = htons(port_number); // numero di porta UDP
	add.sin_addr.s_addr = htonl(ip_address); // indirizzo IP
	// trasmissione datagram
	if ((n = sendto(socket_id, (void *)data, byte, 0, ( struct sockaddr*)&add, sizeof(add))) < 0)
	  return -1;
	return n;
}

int UDP_receive( uint32_t *ip_address, uint16_t *port_number, uint8_t data[], int size)
{
	struct sockaddr_in add; // struttura per indirizzo mittente
	unsigned int dim = sizeof(add);
	int n;
	
	// ricezione datagram (non bloccante)
	if (( n = recvfrom(socket_id, (void *)data, size, 0, (struct sockaddr*)&add, &dim)) <= 0)
	  return -1;
	// estrazione indirizzo IP e numero di porta UDP
	*ip_address = ntohl(add.sin_addr.s_addr);
	*port_number = ntohs(add.sin_port);
	return n;
}

int UDP_wait(unsigned long millisecond)
{
	int n;
	struct timeval timeout;
	fd_set read_set;
	
	FD_ZERO(&read_set);
	FD_SET(socket_id, &read_set);
	timeout.tv_sec = millisecond/1000;
	timeout.tv_usec = (millisecond%1000)*1000;
	n = select(socket_id+1, &read_set, NULL, NULL, &timeout);
	if (n < 0)
	  return -1;
	if (n == 0)
	  return 0;
	// n > 0
	return 1;
}

void UDP_close(void)
{
	// chiusura socket
	close(socket_id);
	return;
}
