#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#include "quotazione.h"


typedef struct node *link;

struct node {
	quotazione_t quot;
	struct node *l;
	struct node *r;
	int n;   // Numero di nodi del sotto-albero compreso questo
};

struct quot_bst {
	link root;
	link z;
	int n_quot;
};






static void memCheck( void *p)
{
	if( p == NULL) {
		fprintf( stderr, "ERROR: out of memory!\n");
		exit(EXIT_FAILURE);
	}
}


static link NewNode( quotazione_t quot, link l, link r, int n)
{
	link new;
	new = (link)malloc( sizeof(struct node));
	memCheck( new);
	new->quot = quot;
	new->l = l;
	new->r = r;
	new->n = n;
	return new;
}





quotazione_t QuotNull( void)
{
	quotazione_t quot;
	quot.time = TIMEnull();
	quot.value = 0.0;
	quot.scambi = 0;
	return quot;
}

int QuotCheckNull( quotazione_t quot)
{
	if( quot.value != 0.0)
		return 0;
	if( TIMEcompare( quot.time, TIMEnull()) != 0)
		return 0;
	return 1;
}

int QuotCompare( quotazione_t a, quotazione_t b)
{
	if( a.value < b.value)
		return -1;
	if( a.value > b.value)
		return 1;
	return TIMEdatecmp( a.time, b.time);
}

void QuotPrint( FILE *output, quotazione_t quot)
{
	fprintf( output, "Data: ");
	TIMEdatePrint( output, quot.time);
	fprintf( output, "; %.2f\n", quot.value);
}




Qbst BstInit( void)
{
	Qbst bst;
	bst = (Qbst)malloc(sizeof(*bst));
	bst->root = (bst->z = NewNode( QuotNull(), NULL, NULL, 0));
	bst->n_quot = 0;
	return bst;
}


static void recursiveFree( link h, link z)
{
	if( h == z)
		return;
	
	recursiveFree( h->l, z);
	recursiveFree( h->r, z);
	free( h);
}

void BstFree( Qbst bst)
{
	recursiveFree( bst->root, bst->z);
	free( bst->z);
	free( bst);
}


int BstGetNodes( Qbst bst)
{
	return bst->n_quot;
}

// Solo per la funzione sottostante
static int is_subst_quot_insert = 0;

static link insert( quotazione_t quot, link h, link z)
{
	// Se ho aggiornato un valore di una quotazione in una
	// data gia' esistente, allora risalgo le ricorsioni
	if( is_subst_quot_insert == 1)
		return h;

	if( h == z) {
		return NewNode( quot, z, z, 1);
	}
	// Se la quot inserita e' nello stesso giorno allora
	// viene aggiornata al nuovo valore
	if( TIMEdatecmp( quot.time, h->quot.time) == 0) {
		// Applico:
		// ~~> (Q1*n1 + Q2*n2)/(n1+n2) = Q_fin
		h->quot.value = (h->quot.value * h->quot.scambi + quot.value * quot.scambi);
		h->quot.value =  h->quot.value / (float)(h->quot.scambi + quot.scambi);
		h->quot.scambi += quot.scambi;
		is_subst_quot_insert = 1;
		return h;
	}

	if( TIMEdatecmp( quot.time, h->quot.time) < 0) {
		h->l = insert( quot, h->l, z);
	}
	else {
		h->r = insert( quot, h->r, z);
	}
	if( !is_subst_quot_insert)
		h->n++;
	return h;
}

void BstInsert( Qbst bst, quotazione_t quot)
{
	// Se due quotazioni sono le stesse, la quot di quel
	// giorno viene aggiornata al nuovo valore
	if( bst == NULL)
		return;
	bst->root = insert( quot, bst->root, bst->z);
	// Se non si e' aggiornato nessun valore, non aumentare
	// il numero di nodi
	if( is_subst_quot_insert == 0)
		bst->n_quot++;
	is_subst_quot_insert = 0;
}


static quotazione_t searchByDate( mytime_t date, link h, link z)
{
	if( h == z)
		return QuotNull();
	if( TIMEdatecmp( date, h->quot.time) == 0)
		return h->quot;
	if( TIMEdatecmp( date, h->quot.time) < 0)
		return searchByDate( date, h->l, z);
	return searchByDate( date, h->r, z);
}

quotazione_t BstSearchByDate( Qbst bst, mytime_t date)
{
	return searchByDate( date, bst->root, bst->z);
}

static quotazione_t recursiveSelect( int *steps, link h, link z, int n)
{
	quotazione_t quot;
	if( h == z)
		return QuotNull();

	quot = recursiveSelect( steps, h->l, z, n);
	if( !QuotCheckNull( quot))
		return quot;

	if( n == *steps)
		return h->quot;
	(*steps)++;

	quot = recursiveSelect( steps, h->r, z, n);
	return quot;
}

quotazione_t BstSelect( Qbst bst, int n)
{
	int steps = 0;
	return recursiveSelect( &steps, bst->root, bst->z, n);
}




static void searchMinBetweenDate( link h, link z, mytime_t data1, mytime_t data2, quotazione_t *quot_min)
{
	if( h == z)
		return;

	searchMinBetweenDate( h->l, z, data1, data2, quot_min);
	if( TIMEdatecmp( data1, h->quot.time) < 0  &&
		TIMEdatecmp( data2, h->quot.time) > 0) {
		if( h->quot.value < quot_min->value)
			*quot_min = h->quot;
	}
	searchMinBetweenDate( h->r, z, data1, data2, quot_min);
}

quotazione_t BstSearchMinBetweenDate( Qbst bst, mytime_t data1, mytime_t data2)
{
	quotazione_t quot_min;
	quot_min.value = FLT_MAX;
	if( bst == NULL)
		return QuotNull();
	searchMinBetweenDate( bst->root, bst->z, data1, data2, &quot_min);
	return quot_min;
}


static void searchMaxBetweenDate( link h, link z, mytime_t data1, mytime_t data2, quotazione_t *quot_max)
{
	if( h == z)
		return;

	searchMaxBetweenDate( h->l, z, data1, data2, quot_max);
	if( TIMEdatecmp( data1, h->quot.time) < 0  &&
		TIMEdatecmp( data2, h->quot.time) > 0) {
		if( h->quot.value > quot_max->value)
			*quot_max = h->quot;
	}
	searchMaxBetweenDate( h->r, z, data1, data2, quot_max);
}

quotazione_t BstSearchMaxBetweenDate( Qbst bst, mytime_t data1, mytime_t data2)
{
	quotazione_t quot_max;
	quot_max.value = 0.0;
	if( bst == NULL)
		return QuotNull();
	searchMaxBetweenDate( bst->root, bst->z, data1, data2, &quot_max);
	return quot_max;
}



static int find_max_height( link h, link z)
{
	int max_l, max_r;
	if( h == z)
		return -1;
	max_l = find_max_height( h->l, z);
	max_r = find_max_height( h->r, z);
	if( max_l >= max_r)
		return max_l + 1;
	return max_r + 1;
}

static int find_min_height( int pos, link h, link z)
{
	// Considero come foglia il nodo z
	int min_l, min_r;
	if( h == z)
		return pos;
	min_l = find_min_height( pos+1, h->l, z);
	min_r = find_min_height( pos+1, h->r, z);
	if( min_l <= min_r)
		return min_l;
	return min_r;
}


static link rotate_left( link root)
{
	link temp;
	temp = root->r;
	root->r = temp->l;
	temp->l = root;

	temp->n = root->n;
	root->n -= temp->r->n;
	root->n--;

	return temp;
}

static link rotate_right( link root)
{
	link temp;
	temp = root->l;
	root->l = temp->r;
	temp->r = root;

	temp->n = root->n;
	root->n -= temp->l->n;
	root->n--;

	return temp;
}

static link partition( link h, int r)
{
	// r = rango
	int t = h->l->n;
	if( t > r) {
		h->l = partition( h->l, r);
		h = rotate_right( h);
	}
	if( t < r) {
		h->r = partition( h->r, r-t-1);
		h = rotate_left( h);
	}
	return h;
}

static link balance_tree( link root, link z)
{
	int mediana;
	if( root == z)
		return z;
	mediana = (root->n+1)/2 -1;
	root = partition( root, mediana);
	root->l = balance_tree( root->l, z);
	root->r = balance_tree( root->r, z);
	return root;
}

void BstBalance( Qbst bst, int soglia)
{
	int max, min;
	// Devo trovare la differenza tra l'altezza minima e massima delle foglie
	max = find_max_height( bst->root, bst->z);
	min = find_min_height( 0, bst->root, bst->z);
	if( max - min > soglia)
		bst->root = balance_tree( bst->root, bst->z);
}





static void treePrint( FILE *output, link h, link z, int order)
{
	if( h == z)
		return;

	if( order == PRE_ORDER)
		QuotPrint( output, h->quot);
	treePrint( output, h->l, z, order);
	if( order == IN_ORDER)
		QuotPrint( output, h->quot);
	treePrint( output, h->r, z, order);
	if( order == POST_ORDER)
		QuotPrint( output, h->quot);
}

void BstPrint( FILE *output, Qbst bst, int order)
{
	treePrint( output, bst->root, bst->z, order);
}
