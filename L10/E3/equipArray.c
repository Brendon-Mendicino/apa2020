#include "equipArray.h"


struct equipArray_s {
	int n_items;
	int inv_index[EQUIP_SLOT];
};


/* creatore */
equipArray_t equipArray_init( void)
{
	equipArray_t equip;
	equip = (equipArray_t)malloc(sizeof(*equip));
	if( equip == NULL) {
		fprintf( stderr, "ERROR: out of memory!\n");
		exit(EXIT_FAILURE);
	}
	equip->n_items = 0;
	return equip;
}
/* distruttore */
void equipArray_free(equipArray_t equipArray)
{
	free( equipArray);
}



/* quanti equipaggiamenti sono in uso */
int equipArray_inUse(equipArray_t equipArray)
{
	return equipArray->n_items;
}

/* scrittura su file */
void equipArray_print(FILE *fp, equipArray_t equipArray, invArray_t invArray)
{
	for( int i = 0; i < equipArray->n_items; i++) {
		fprintf( fp, "%2d) ", i+1);
		invArray_printByIndex( fp, invArray, equipArray->inv_index[i]);
	}
}

/* modifica equipaggiamento scegliendo un oggetto da inventario */
inv_t *equipArray_add(equipArray_t equipArray, invArray_t invArray)
{
	int index = 0;
	if( equipArray->n_items == EQUIP_SLOT) {
		fprintf( stderr, "Equipaggiamento pieno!\n");
		return NULL;
	}

	invArray_print( stdout, invArray);
	printf( "Scegli indice: ");
	scanf( "%d", &index);
	
	index--;
	if( invArray_checkIndex( invArray, index)) {
		equipArray->inv_index[equipArray->n_items] = index;
		equipArray->n_items++;
		return ( invArray_getByIndex( invArray, index));
	}
	return NULL;
}

/* rimuove un oggetto dall'equipaggiamento */
inv_t *equipArray_remove( equipArray_t equipArray, invArray_t invArray)
{
	int index = 0;
	int index_buffer = 0;
	if( equipArray->n_items == 0) {
		fprintf( stderr, "Equipaggiamento vuoto!\n");
		return NULL;
	}

	equipArray_print( stdout, equipArray, invArray);
	printf( "Scegli indice: ");
	scanf( "%d", &index);

	index--;
	if( index >= 0  && index < equipArray->n_items) {
		index_buffer = equipArray->inv_index[index];
		equipArray->inv_index[index] = equipArray->inv_index[equipArray->n_items];
		equipArray->n_items--;
		return ( invArray_getByIndex( invArray, index_buffer));
	}
	return NULL;
}

/* torna indice (nel vettore inventario) dell'oggetto in posizione index (0..EQUIP_SLOT-1) di equipArray */
int equipArray_getEquipByIndex(equipArray_t equipArray, int index)
{
	if( !( index >= 0  &&  index < equipArray->n_items))
		return -1;
	return ( equipArray->inv_index[index]);
}
