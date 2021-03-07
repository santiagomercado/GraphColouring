#include "Rii.h"
#include "aux_ConstruccionDelGrafo.h"
#include "graph_struct.h"

#define MAX_LENGTH_LINE 1024
#define COMMENTARY "c"
#define PAIR "p edge "
#define EDGE "e "
#define PAIR_PATTERN PAIR "%u %u"
#define EDGE_PATTERN EDGE "%u %u"

// Devuelve true si la linea leida es un comentario
static bool is_commentary(char* line){
	bool res = true;
	for(u32 i = 0; i < strlen(COMMENTARY); i++){
		res = res && (line[i] == COMMENTARY[i]);
	}
	return res;
}

// Devuelve true si la linea leida es el par p edge n m
static bool is_pair(char* line){
	bool res = true;
	for(u32 i = 0; i < strlen(PAIR); i++){
		res = res && (line[i] == PAIR[i]);
	}
	return res;
}

// Devuelve true si la linea leida es un lado
static bool is_edge(char* line){
	bool res = true;
	for(u32 i = 0; i < strlen(EDGE); i++){
		res = res && (line[i] == EDGE[i]);
	}
	return res;
}

static bool is_loaded(u32* ptr){return ptr != NULL;}

// Ordena a los pares xy de menor a mayor con respecto a x
int cmp_edge(const void* x, const void* y){
    const u32* a = *(const u32 **)x;
    const u32* b = *(const u32 **)y;
    if(a[0] < b[0])
        return -1;
    else if(a[0] > b[0])
    	return 1;
    else
    	return 0;
}

// Dado nombre de un vertice, devuelve puntero
// a la estructura del vertice(busqueda binaria)
static struct vertex* get_vertex_data(Grafo g, u32 v){
	struct vertex* res = NULL;
	u32 first, last, middle; 
   	first = 0;
   	last = g->n - 1;
   	middle = (first+last)/2;
   	while (first <= last) {
		if (g->v[middle]->name  < v)
			first = middle + 1;    
		else if (g->v[middle]->name == v) {
			res = g->v[middle];
			break;
		}
		else
			last = middle - 1;
		middle = (first + last)/2;
	}
	return res;
}

// Checkea q los datos leido en stdin sean correctos
int check_read_data(u32 *n, u32*m, u32** edge, u32 i){
	if(n != NULL && m != NULL){ 							// Si leyeron n y m
		if(i < *m) 											// Si la cantidad de lados leidos es menor que m,
			return ERR_EDGE; 								// devuelve error
		else{ 												// Si la cantidad de lados leidos es m
			if(!is_vertex_number_correct(edge, *n, *m)) 	// Checkea si la cantidad de vertices leidos se es n
				return ERR_VERTEX; 							// Si la cantidad de vertices leidos difiere de n, devuelve error
		}
	}else{
		return ERR_FIRST_LINE; 								// Si no se leyeron n o m, devuelve error
	}
	return CORRECT_DATA;
}

// Funcion para liberar variable auxiliares
void free_aux_data(u32* n, u32* m, u32** edge){
	if(edge != NULL){
		for(u32 j = 0; j < 2*(*m); j++){
			if(edge[j] != NULL)
				free(edge[j]);
		}
		free(edge);
	}
	if(n != NULL)
		free(n);
	if(m != NULL)
		free(m);
}

// Esta funcion es usada luego de que ocurra algun problema con los allocs.
// Libera toda la memoria pedida anteriromente(antes del problema con los allocs)
// para guardar el grafo
void free_struct(Grafo g){
	if(g->available_colors != NULL)
		free(g->available_colors);
	for(u32 i = 0; i < g->n; i++){
		if(g->v[i]->neighbors != NULL)
			free(g->v[i]->neighbors);
		if(g->v[i] != NULL)
			free(g->v[i]);
	}
	if(g->v != NULL)
		free(g->v);
	if(g != NULL)
		free(NULL);
}

// Setea los vecinos de cada vertice 
// {PRE COND:edge tiene q tener los lados xy e yx,
// y ademas edge tiene q esta ordenado de menor a
// mayor con respecto a la primer coordenada}
static int set_neighbors(Grafo g, u32** edge){
	u32 j = 0;
	u32 k;
	for(u32 i = 0; i < g->n; i++){ 																	// Recorre todos los vertices
		g->v[i]->neighbors = (struct vertex **)malloc((g->v[i]->degree)*sizeof(struct vertex *)); 	// Pide memoria para almacenar todos los
																									// vecinos del vertice
		if(g->v[i]->neighbors == NULL) 																// Si hubo problemas con el alloc,
			return EXIT_FAILURE; 																	// entonces devuelve error
		k = 0;
		while(j < 2*(g->m) && g->v[i]->name == edge[j][0]){											// Si la primer coordenada de edge es
																									// igual al nombre del i-esimo vertice
			g->v[i]->neighbors[k] = get_vertex_data(g, edge[j][1]);									// Agrega al vertice que hace lado con el
																									// i-esimo vertice al arreglo de vecinos del i-esimo vertice
			k++;
			j++;
		}
	}
	return EXIT_SUCCESS;
}
// Setea toda la estructura del grafo
// edge ya esta ordenado con respecto a la primer coordenada
int set_graph(Grafo g, u32** edge, u32 n, u32 m){
	u32 n_v = 0;
	g->n = n; 															// Setea el numero de vertices del grafo
	g->m = m; 															// Setea el numero de lados del grafo
	g->delta = 0; 														// Setea el grado mas alto del grafo
	g->colors = 0; 														// Setea cantidad de colores del grafo
	g->v = (struct vertex **)malloc(n*sizeof(struct vertex *)); 		// Pide memoria para almacenar los vertices
	if(g->v == NULL) 													// Su hubo problemas con el alloc,
		return EXIT_FAILURE; 											// entonces devuelve error
	for(u32 i = 0; i < 2*m; i++){ 										// Recorre edge
		if(i == 0 || edge[i-1][0] != edge[i][0]){ 						// Si lee otro vertice,
			g->v[n_v] = (struct vertex *)malloc(sizeof(struct vertex)); // entonces pide memoria para almacenar un nuevo vertice
			if(g->v[n_v] == NULL){ 										// Si hubo algun problema con el alloc,
				return EXIT_FAILURE; 									// entonces devuelve error
			}
			g->v[n_v]->name = edge[i][0];								// Setea el nombre del vertice
			g->v[n_v]->color = g->colors;								// Setea el color del vertice
			g->v[n_v]->is_colored = true; 								// Indica de que el vertice esta coloreado
			g->v[n_v]->RMBCchicogrande_priority = 0; 					// Setea la prioridad de chicogrande a 0.
			g->colors++;												// Setea el grado del vertice en 1
			g->v[n_v]->degree = 1;										// Aumenta la cantidad de colores de g
			n_v++;														// Aumenta n_v para leer el proximo lado de edge
		}else															// Si lee el mismo vertice,
			g->v[n_v-1]->degree++;										// entonces aumenta el grado del vertice
		if(g->v[n_v-1]->degree > g->delta) 								// Comprueba si hay q actualizar g->delta
				g->delta = g->v[n_v-1]->degree; 
	}
	g->available_colors = (bool *)calloc(g->delta+1, sizeof(bool)); 	// Pide memoria para almacenar el arreglo de colores
	if(g->available_colors == NULL){ 									// Si hubo problemas con el alloc,
		return EXIT_FAILURE; 											// entonces devuelve error
	}
	if(set_neighbors(g, edge) != EXIT_SUCCESS){ 						// Seteo los vecinos de cada vertice. Si hubo algun problema,
		return EXIT_FAILURE; 											// entonces devuelve error
	}
	return EXIT_SUCCESS;
}
//checkea si la cantidad de vertices leidos coincide con n
bool is_vertex_number_correct(u32** edge, u32 n, u32 m){
	u32 v_count = 1;
	qsort(edge, 2*m, 2*sizeof(u32), cmp_edge);
	for(u32 i = 0; i < 2*m - 1; i++){
		if(edge[i][0] != edge[i+1][0]){
			v_count++;
		}
	}
	return v_count == n;
}
// Lee del stdin y carga los datos en ptr_n,ptr_m y ptr_edge
int read_stdin(u32** ptr_n, u32** ptr_m, u32*** ptr_edge,u32* ptr_i){	
	char read_line[MAX_LENGTH_LINE]; 												// Guarda la linea leida del stdin
	u32 i = 0;																		// Lleva la cuenta de lados cargados
	u32* n = NULL; 																	// Guarda la cantidad de vertices
	u32* m = NULL; 																	// GUarda la cantidad de lados
	u32** edge = NULL; 																// Guarda los lados del grafo
	while(fgets(read_line, MAX_LENGTH_LINE, stdin) != NULL){ 						// Mientras haya algo para leer
		if(is_commentary(read_line) && !(is_loaded(n) && is_loaded(m))){			// Si es un comentario y todavia no cargó n,m
			/*nothing*/ 															// Omite el comentario
		}
		else if(is_pair(read_line) && !(is_loaded(n) && is_loaded(m))){ 			// Si es la linea p edge ... y todavia no cargó n,m
			n = (u32 *)malloc(sizeof(u32)); 										// Pide memoria para almacenar cantidad de vertices
			m = (u32 *)malloc(sizeof(u32)); 										// Pide memoria para almacenra cantidad de lados
			if(n == NULL || m == NULL) 												// Si hubo problemas con algun alloc,
				return EXIT_FAILURE; 												// entonces devuelve error
			if(sscanf(read_line,PAIR_PATTERN,n,m) != 2) 							// Si no cargo correctamente n y m,
				break; 																// entonces para la carga							
			edge = (u32 **)malloc(2*(*m)*sizeof(u32 *)); 							// Pide memoria para almacenar los lados
			if(edge == NULL) 														// Si hubo algun problema con el alloc,
				return EXIT_FAILURE; 												// entonces devuelve error
			for(u32 j = 0; j < 2*(*m); j++){
				edge[j] = (u32 *)malloc(2*sizeof(u32));
				if(edge[j] == NULL){
					for(u32 k = 0; k < j; k++)
						free(edge[k]);
					free(edge);
					return EXIT_FAILURE;
				}
			}
		}
		else if(is_edge(read_line) && is_loaded(n) && is_loaded(m)){ 				// Si es un lado y ya cargo n,m
			if(sscanf(read_line,EDGE_PATTERN,&edge[i][0],&edge[i][1]) != 2) 		// Carga el lado en edge. Si hubo algun problema,
				break;																// entonces para la carga
			else{
				edge[i+(*m)][0] = edge[i][1];										// Ya cargado el lado xy, carga el lado yx
				edge[i+(*m)][1] = edge[i][0];										
				i++;
				if(i >= *m) 														// Si ya cargó todos los lados,
					break; 															// entonces para la carga
			}
		}else 																		// Si la linea leida no cumple ninguna de las condiciones anteriores,
			break; 																	// entonces para la carga
	}
	*ptr_n = n; *ptr_m = m; 														// Carga los datos
	*ptr_edge = edge; *ptr_i = i;
	return EXIT_SUCCESS;
} 
