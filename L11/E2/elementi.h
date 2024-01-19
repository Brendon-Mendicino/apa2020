
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


typedef struct elemList_t *List_t;

struct elemList_t{
	elemento_t *elem;
	int n_elem;
	int max_elem;
};

typedef struct diag_t *Diag;

struct diag_t{
	List_t list;
	int diff;
};


/* Creatore.  */
List_t listInit( int max_elem);

/* Creatore.  */
Diag *diagInit( int n_diag);

/* Distruttore.  */
void listFree( List_t list);

/* Distruttore.  */
void diagFree( Diag *diag, int n_diag);



/* Stampa la lista.  */
void listPrint( FILE *output, List_t list);

/* Leggi da file.  */
void listReadFromFile( FILE *input, List_t list);

void printDiag( FILE *output, Diag diag);



/* Funzioni di controllo.  */

/* Controllo elemeto acrobatico. Si usano i campi di bit per l'argomento 'acrob_type.  */
int checkAcrobatico( List_t list, int *inedex, int n_index);

/* Controllo se un tipo avanti e' presente.  */
int checkAcrobFront( List_t list, int *index, int n_index);

/* Controllo se un tipo indietro e' presente.  */
int checkAcrobBack( List_t list, int *index, int n_index);

/* Controllo due elementi consecutivi.  */
int checkAcrobConsecutive( List_t list, int *index, int n_index);

/* Controllo sul primo elemento, deve essre frontale.  */
int isFirstValid( List_t list, int index);

/* Controllo sulla validita' dell'elemento in base al predecessore. */
int isNextValid( List_t list, int last, int prev);

/* Controllo se l'elemento puo essere suguito da altri.  */
int isLastElement( List_t list, int index);

/* Controlle se l'elemento ha bisogno di un elemento di precedenza.  */
int isReqDiPreced( List_t list, int index);

/* Controlla se un elemento con difficalta' '8' e' prente in una lista.  */
int isEightPresent( List_t list, int *index, int n_index);

/* Prendi lo 'score' totale da una lista.  */
float getScore( List_t list, int *index, int n_index);

/* Ritorna la differenza totale.  */
int listGetDiff( List_t list, int *index, int n_index);


/* Copia una lista di elementi dentro una diagonale.  */
void diagCopy( Diag diag, int *index, int n_index, List_t list);


void listSort( List_t list);




#endif  // ELEMENTI_H
