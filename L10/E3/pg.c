#include <limits.h>

#include "equipArray.h"
#include "pg.h"

static char str_buff[50];

/* lettura da file */
int pg_read(FILE *fp, pg_t *pgp)
{
	if( fscanf( fp, "%s%s%s", pgp->cod, pgp->nome, pgp->classe) != 3)
		return 0;
	if( !stat_read( fp, &pgp->b_stat))
		return 0;

	pgp->eq_stat.hp  = pgp->b_stat.hp;
	pgp->eq_stat.mp  = pgp->b_stat.mp;
	pgp->eq_stat.atk = pgp->b_stat.atk;
	pgp->eq_stat.def = pgp->b_stat.def;
	pgp->eq_stat.mag = pgp->b_stat.mag;
	pgp->eq_stat.spr = pgp->b_stat.spr;
	// Dopo i controlli alloco equip
	pgp->equip = equipArray_init();
	return 1;
}

/* scrittura su file */
void pg_print(FILE *fp, pg_t *pgp, invArray_t invArray)
{
	fprintf( fp, "%s %s %s ", pgp->cod, pgp->nome, pgp->classe);
	if( equipArray_inUse( pgp->equip) != 0) {
		stat_print( fp, pgp->eq_stat, 1);
		fprintf( fp, "\n==+ EQUIPMENT +==\n");
		equipArray_print( fp, pgp->equip, invArray);
		fprintf( fp, "=================");
	}
	else {
		stat_print( fp, pgp->b_stat, 1);
	}
}

/* non essendo struct dinamica, pulisce chiamando il distruttire di equipArray */
void pg_clean(pg_t *pgp)
{
	equipArray_free( pgp->equip);
}


/* modifica personaggio aggiungendo/togliendo un equipaggiamento selezionato da inventario:
di fatto e' sufficiente chiamare l'opportuna funzione dal modulo equipArray */
void pg_updateEquip(pg_t *pgp, invArray_t invArray)
{
	inv_t *inv;
	printf( "Scegliere se aggiungere o rimuovere un oggetto.\n");
	printf( "[add] [remove]: ");
	scanf( "%s", str_buff);

	if( strcmp( "add", str_buff) == 0) {
		inv = equipArray_add( pgp->equip, invArray);
		if( inv != NULL) {
			pgp->eq_stat.hp  += inv->stat.hp;
			pgp->eq_stat.mp  += inv->stat.mp;
			pgp->eq_stat.atk += inv->stat.atk;
			pgp->eq_stat.def += inv->stat.def;
			pgp->eq_stat.mag += inv->stat.mag;
			pgp->eq_stat.spr += inv->stat.spr;
		}
	}
	else if( strcmp( "remove", str_buff) == 0) {
		inv = equipArray_remove( pgp->equip, invArray);
		if( inv != NULL) {
			pgp->eq_stat.hp  -= inv->stat.hp;
			pgp->eq_stat.mp  -= inv->stat.mp;
			pgp->eq_stat.atk -= inv->stat.atk;
			pgp->eq_stat.def -= inv->stat.def;
			pgp->eq_stat.mag -= inv->stat.mag;
			pgp->eq_stat.spr -= inv->stat.spr;
		}
	}
	else
		fprintf( stderr, "Input invalido!\n");
}
