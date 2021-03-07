#include "Rii.h"
#include "graph_struct.h"

// Resetea el arreglo de colores disponibles
void clean_colors(Grafo g, u32 i){
	for(u32 j = 0; j < g->v[i]->degree; j++){
		if(g->v[i]->neighbors[j]->is_colored == true)
			g->available_colors[g->v[i]->neighbors[j]->color] = false;
	}
}

// Setea el arreglo de colores disponibles
void set_colors(Grafo g, u32 i){
	for(u32 j = 0; j < g->v[i]->degree; j++){
		if(g->v[i]->neighbors[j]->is_colored == true)
			g->available_colors[g->v[i]->neighbors[j]->color] = true;
	}
}

// Busca el minimo color disponible en g->available_colors
u32 find_color(Grafo g){
	u32 ret = g->delta;
	for(u32 i = 0; i < g->delta + 1; i++){
		if(!g->available_colors[i]){
			ret = i;
			break;
		}
	}
	return ret;
}

