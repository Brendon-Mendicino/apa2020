
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
int STinsert( ST st, char *nome);

/* Selezione l'elemeto di rango r nella tabella.  */
int STselect( ST st, int r);

/* Ritorna il valore associato ad un nome.  */
int STsearch( ST st, char *nome);

/* Cerca la chiave.  */
char *STsearchByValue( ST st, int value);


#endif  // SYMBOL_T_H
