#ifndef GRAPH_STRUCT
#define GRAPH_STRUCT

struct vertex {
	u32 name; 						//nombre del vertice
	u32 color; 						//color del vertice
	u32 degree; 					//grado del vertice
	bool is_colored;				//indica si el vertice tiene un color q fue asignado por greedy o no
	u32 RMBCchicogrande_priority; 	//es usado para ordenar los vertices segun la especificacion de RMBC_chicogrande
	struct vertex **neighbors; 		//arreglo de vecinos del vertice
};

struct GrafoSt {
	u32 n; 							//numero de vertices del grafo
	u32 m; 							//numero de lados del grafo
	u32 delta; 						//el grado mas alto de todos los vertices
	u32 colors; 					//cantidad de colores con los q esta coloreado el grafo
	bool *available_colors; 		//arreglo auxiliar usado en greedy para buscar el minimo color disponible
	struct vertex ** v; 			//arreglo de vertices del grafo
};

#endif