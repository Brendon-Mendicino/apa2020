
#ifndef SYMBOL_T_H
#define SYMBOL_T_H

typedef struct symboltable_t *ST;


/* Costruttore, inizializza al massimo numero di elementi.  */
ST STinit( int max);

/* Distruttore.  */
void STfree( ST st);

/* Conta il numero di elementi nella tabella.  */
int STcount( ST st);

/* Inserisci un nuovo elemento; ritorna il valore della chiave inserita.  */
int STinsert( ST st, char *nome, int net);

/* Selezione l'elemeto r-esimo nella tabella.  */
int STselect( ST st, int r);

/* Ritorna il valore associato ad un nome.  */
int STgetValue( ST st, char *nome);

/* Cerca la chiave.  */
char *STsearchByValue( ST st, int value);

#endif  // SYMBOL_T_H
