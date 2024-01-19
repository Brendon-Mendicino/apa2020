#include <limits.h>

#include "inv.h"



/* funzioni di input delle statistiche */
int stat_read(FILE *fp, stat_t *statp)
{
	if( fscanf( fp, "%d%d%d%d%d%d", &statp->hp, &statp->mp, &statp->atk, &statp->def, &statp->mag, &statp->spr) != 6)
		return 0;
	return 1;
}

/* funzioni di ouput delle statistiche */
void stat_print(FILE *fp, stat_t stat, int soglia)
{
	if( stat.hp  < soglia) {
		stat.hp  = soglia;
	}
	if( stat.mp  < soglia) {
		stat.mp  = soglia;
	}
	if( stat.atk < soglia) {
		stat.atk = soglia;
	}
	if( stat.def < soglia) {
		stat.def = soglia;
	}
	if( stat.mag < soglia) {
		stat.mag = soglia;
	}
	if( stat.spr < soglia) {
		stat.spr = soglia;
	}
	fprintf( fp, "%d %d %d %d %d %d", stat.hp, stat.mp, stat.atk, stat.def, stat.mag, stat.spr);
}



/* funzioni di input di un oggetto dell'inventario */
void inv_read(FILE *fp, inv_t *invp)
{
	fscanf( fp, "%s%s", invp->nome, invp->tipo);
	stat_read( fp, &invp->stat);
}

/* funzioni di output di un oggetto dell'inventario */
void inv_print(FILE *fp, inv_t *invp)
{
	fprintf( fp, "%s %s ", invp->nome, invp->tipo);
	stat_print( fp, invp->stat, INT_MIN);
}

/* ritorna il campo stat di un oggetto dell'inventario */
stat_t inv_getStat(inv_t *invp)
{
	return (invp->stat);
}
