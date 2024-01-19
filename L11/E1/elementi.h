
#ifndef ELEMENTI_H
#define ELEMENTI_H

#include <stdio.h>

#define MAX_NAME 100
#define MAX_ELEM 5


// Campo di bit per i tipi di acrobazia
#define  E_TRANS	1
#define  E_FRONT	2
#define  E_BACK		4

typedef enum {
	trans = 0,
	acr_front = 1,
	acr_back = 2
} type_e;



typedef struct {
	char nome[MAX_NAME];
	int type;
	int dir_entr;
	int dir_exit;
	int req_preced;
	int finale;
	float score;
	int diff;
}elemento_t;

typedef struct {
	elemento_t *elem;
	int n_elem;
	int max_elem;
} elemList_t;

typedef struct diag_t *Diag;

struct diag_t{
	elemList_t *list;
	int diff;
};


/* Creatore.  */
elemList_t *listInit( int max_elem);

/* Creatore.  */
Diag *diagInit( int n_diag);

/* Distruttore.  */
void listFree( elemList_t *list);

/* Distruttore.  */
void diagFree( Diag *diag, int n_diag);



/* Stampa la lista.  */
void listPrint( FILE *output, elemList_t *list);

/* Leggi da file.  */
void listReadFromFile( FILE *input, elemList_t *list);

void printDiag( FILE *output, Diag diag);



/* Funzioni di controllo.  */

/* Controllo elemeto acrobatico. Si usano i campi di bit per l'argomento 'acrob_type.  */
int checkAcrobatico( elemList_t *list, int *inedex, int n_index);

/* Controllo se un tipo avanti e' presente.  */
int checkAcrobFront( elemList_t *list, int *index, int n_index);

/* Controllo se un tipo indietro e' presente.  */
int checkAcrobBack( elemList_t *list, int *index, int n_index);

/* Controllo due elementi consecutivi.  */
int checkAcrobConsecutive( elemList_t *list, int *index, int n_index);

/* Controllo sul primo elemento, deve essre frontale.  */
int isFirstValid( elemList_t *list, int index);

/* Controllo sulla validita' dell'elemento in base al predecessore. */
int isNextValid( elemList_t *list, int last, int prev);

/* Controllo se l'elemento puo essere suguito da altri.  */
int isLastElement( elemList_t *list, int index);

/* Controlle se l'elemento ha bisogno di un elemento di precedenza.  */
int isReqDiPreced( elemList_t *list, int index);

/* Controlla se un elemento con difficalta' '8' e' prente in una lista.  */
int isEightPresent( elemList_t *list, int *index, int n_index);

/* Prendi lo 'score' totale da una lista.  */
float getScore( elemList_t *list, int *index, int n_index);

/* Ritorna la differenza totale.  */
int listGetDiff( elemList_t *list, int *index, int n_index);


/* Copia una lista di elementi dentro una diagonale.  */
void diagCopy( Diag diag, int *index, int n_index, elemList_t *list);

#endif  // ELEMENTI_H
