# UDP_ZTL_Set2023


L’accesso alla ZTL di una città è controllato con telecamere in grado di“leggere” la targa dei veicoli che transitano dai varchi di ingresso e di uscita della zona a traffico limitato: il sistema di gestione prevede che siano multati i veicoli non autorizzati che entrano nella ZTL e vi permangono più di 15 minuti, a questo scopo i varchi di uscita integrano un segnalatore luminoso che si illumina di verde nel caso di uscita senza multa, di rosso altrimenti.

Le telecamere sono connesse ad una rete MAN che comprende un server di gestione del controllo degli accessi e si comportano come client che impiegano il protocollo di trasporto UDP. L’azienda ztl.net ha già realizzato per conto della polizia municipale un software di gestione per S.O. Linux che espone la seguente API in linguaggio C/C++ (ciascuna funzione restituisce un valore negativo in caso di errore, 0 in caso di successo):



int ingressoVeicolo(char targa[], unsigned long timestamp);


int uscitaVeicolo(char targa[], unsigned long timestamp, bool* multa);



Siete incaricati di definire il protocollo applicativo di rete e di implementare in linguaggio C/C++ il server UDP che si interfaccia con il software di gestione; dovete inoltre realizzare un client di test che simuli il comportamento della telecamera di un varco di uscita.
