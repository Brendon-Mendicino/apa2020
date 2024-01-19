#include "pgList.h"
#include "pg.h"



struct pgNode {
	pg_t pg;
	struct pgNode *next;
};

typedef struct pgNode pgNode_t;

struct pgList_s {
	struct pgNode *head;
	struct pgNode *tail;
	int n_pg;
};



/* Nuovo nodo della lista.  */
pgNode_t *newNode( pg_t pg, pgNode_t *next)
{
	pgNode_t *x;
	x = (pgNode_t *)malloc(sizeof(pgNode_t));
	x->pg = pg;
	x->next = next;
	return x;
}

/* Distruttore nodo.  */
void freeNode( pgNode_t *x)
{
	equipArray_free( x->pg.equip);
	free( x);
}



/* Inserisce nodo nella lista; ordinamento in base al codice.  */
int pgList_SortInsertByCode( pg_t pg, pgList_t list)
{
    // Caso con 'head==NULL' deve essere gia getsito
    pgNode_t *x, *prev;

    for( x = list->head, prev = NULL; x != NULL; prev = x, x = x->next) {
		if( strcmp( x->pg.cod, pg.cod) > 0)
			break;
		if( strcmp( x->pg.cod, pg.cod) == 0)
			return 0;
	}

    if( prev == NULL) {
        list->head = newNode( pg, x);
	}
    else {
        prev->next = newNode( pg, x);
	}
    if( x == NULL) list->tail = prev->next;

	return 1;
}

/* Estrai un nodo dalla Lista, chiave = 'code'.  */
int pgList_DeleteByCode( char *key, pgList_t list)
{
    pgNode_t **xp, *t, *tail_flag;
    if( list->head == NULL) {
        return 0;
    }

    for( xp = &list->head, tail_flag = NULL; *xp != NULL; tail_flag = *xp, xp = &((*xp)->next)) {
		// La lista e' ordinata, e' inutile continuare
		if( strcmp( (*xp)->pg.cod, key) > 0)
			break;
        if( strcmp( (*xp)->pg.cod, key) == 0) {
            // Il ciclo e' arrivato in coda, dunque 'tail' va cambiata con il nodo precedente
            if( *xp == list->tail) {
                // 'tail_flag' punta al predecessore di '*xp'
                // Nel caso in cui il ciclo arriva ad estrarre l'elemento in coda
                // devo cambiare il volore di 'tail' a quello precedente
                list->tail = tail_flag;
            }

            t = *xp;
            *xp = (*xp)->next;
            freeNode( t);
            return 1;
        }
    }
	return 0;
}



/* creatore */
pgList_t pgList_init( void)
{
	pgList_t list;
	list = (pgList_t)malloc(sizeof(*list));

	list->head = NULL;
	list->tail = NULL;
	list->n_pg = 0;
	return list;
}

void freeList( pgNode_t *x)
{
	if( x == NULL)
		return;
	freeList( x->next);
	equipArray_free( x->pg.equip);
	free( x);
}

/* distruttore */
void pgList_free(pgList_t pgList)
{
	freeList( pgList->head);
}




/* lettura da file */
void pgList_read(FILE *fp, pgList_t pgList)
{
	pg_t pg;

	while( pg_read( fp, &pg)) {
		if( pgList->head == NULL) {
			pgList->head = newNode( pg, NULL);
			pgList->tail = pgList->head;
			pgList->n_pg++;
		}
		else {
			pgList_SortInsertByCode( pg, pgList);
			pgList->n_pg++;
		}
	}
}
/* scrittura su file */
void pgList_print(FILE *fp, pgList_t pgList, invArray_t invArray)
{
	pgNode_t *x = pgList->head;
	for( int i = 0; i < pgList->n_pg; i++) {
		fprintf( fp, "%2d) ", i+1);
		pg_print( fp, &x->pg, invArray);
		fprintf( fp, "\n");
		x = x->next;
	}
}





/* inserimento di un nuovo personaggio */
void pgList_insert(pgList_t pgList, pg_t pg)
{
	if( pgList->head == NULL) {
		pgList->head = newNode( pg, NULL);
		pgList->tail = pgList->head;
		pgList->n_pg++;
	}
	else if( pgList_SortInsertByCode( pg, pgList)) {
		printf( "PG inserito correttamente.\n");
		pgList->n_pg++;
	}
	else {
		fprintf( stderr, "Impossibile inserire il PG!\n");
	}
}

/* cancellazione con rimozione */
void pgList_remove(pgList_t pgList, char* cod)
{
	if( pgList->n_pg == 0) {
		fprintf( stderr, "Nessuno PG presente!\n");
		return;
	}
	if( pgList_DeleteByCode( cod, pgList)) {
		printf( "PG rimosso con successo!\n");
		pgList->n_pg--;
	}
	else {
		fprintf( stderr, "Nessuna corrispondeza trovata!\n");
	}
}

/* ricerca per codice, ritornando il puntatore */
pg_t *pgList_searchByCode(pgList_t pgList, char* cod)
{
	pgNode_t *x;

	for( x = pgList->head;
		 x != NULL  &&  strcmp( x->pg.cod, cod) < 0;
		 x = x->next);

	if( x != NULL && strcmp( x->pg.cod, cod) == 0)
		return ( &x->pg);

	return NULL;
}
