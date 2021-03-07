#include "Rii.h"
#include "graph_struct.h"
#include "aux_sort_functions.h"

// Funcion que compara dos vertices por el nombre. Esta funcion es usada en OrdenNatural()
int cmp_name(const void *n1, const void *n2){
	u32 a = (*(struct vertex **)n1)->name;
	u32 b = (*(struct vertex **)n2)->name;
	if(a < b)
		return -1;
	else if(a > b)
		return 1;
	else
		return 0;
}

// Funcion que compara dos vertices por el grado. Esta funcion es usada en WelshPowell()
int cmp_degree(const void *d1, const void *d2){
	u32 a = (*(struct vertex **)d2)->degree;
	u32 b = (*(struct vertex **)d1)->degree;
	if(a < b)
		return -1;
	else if(a > b)
		return 1;
	else
		return 0;
}

// Funcion que compara dos vertices por el color. Esta funcion es usada en RMBCnormal()
int cmp_color_normal(const void *c1, const void *c2){
	u32 a = (*(struct vertex **)c1)->color;
	u32 b = (*(struct vertex **)c2)->color;
	if(a < b)
		return -1;
	else if(a > b)
		return 1;
	else
		return 0;
}

// Funcion que compara dos vertices por el color. Esta funcion es usada en RMBCrevierte()
int cmp_color_reverse(const void *c1, const void *c2){
	u32 a = (*(struct vertex **)c2)->color;
	u32 b = (*(struct vertex **)c1)->color;
	if(a < b)
		return -1;
	else if(a > b)
		return 1;
	else
		return 0;
}

// Funcion que compara dos vertices con respecto a la prioridad. 
// Esta funcion es usada en RMBCchicogrande()
int cmp_RMBCchicogrande_priority(const void *p1, const void *p2){
	u32 a_p = (*(struct vertex **)p1)->RMBCchicogrande_priority;
	u32 b_p = (*(struct vertex **)p2)->RMBCchicogrande_priority;
	u32 a_c = (*(struct vertex **)p1)->color;
	u32 b_c = (*(struct vertex **)p2)->color;
	if(a_p < b_p)
		return -1;
	else if(a_p > b_p)
		return 1;
	else{
		if(a_c < b_c)
			return -1;
		else if(a_c > b_c)
			return 1;
		else 
			return 0;
	}
}

