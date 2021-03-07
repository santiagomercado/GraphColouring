#include "Rii.h"
#include "graph_struct.h"
#include "aux_ConstruccionDelGrafo.h"
#include "aux_Greedy.h"
#include "aux_sort_functions.h"
#include "queue.h"

Grafo ConstruccionDelGrafo(){
	u32* n = NULL; 																	// Guarda la cantidad de vertices
	u32* m = NULL; 																	// Guarda la cantidad de lados
	u32** edge = NULL; 																// Guarda los lados
	u32 i; 																			// Guarda la cantidad de lados leidos
	Grafo g = NULL; 																// El grafo q se devolvera
	switch(read_stdin(&n,&m,&edge,&i)){ 											// Lee del stdin los datos y los almacena en n,m, edge
		case EXIT_FAILURE: 															// En caso de haberse quedado sin memoria da error
			printf("Error en alocar memoria\n"); 				
			break;
		case EXIT_SUCCESS: 															// Caso contrario se checkean los datos leidos
			switch(check_read_data(n,m,edge,i)){
				case ERR_FIRST_LINE: 												// Caso donde la primer linea leida que no sea 
					printf("Error en primera linea sin comentario\n"); 				// comentario no empezó con p edge
					break;
				case ERR_EDGE: 														// Caso donde se leyeron menos de m lineas con el formato e _ _ 
					printf("Error de lectura en lado %u\n", i+1);
					break;
				case ERR_VERTEX: 													// Caso donde la cantidad de vertices leidos no coincide con n
					printf("Cantidad de vertices leidos no es la declarada\n");
					break;
				case CORRECT_DATA: 													// Caso donde todos los datos leidos son correctos 
					g = (Grafo)malloc(sizeof(struct GrafoSt)); 						// Solicita memoria para el grafo
						if(g != NULL){
							if(set_graph(g, edge, *n, *m) == EXIT_SUCCESS){			// Si todo anduvo bien, setea el grafo con los datos obtenidos de stdin,  
								Greedy(g); 											// Colorea al grafo con Greedy
							}else{
								printf("Error en alocar memoria\n"); 				// Caso contrario(falló algun alloc) se libera toda la memoria que se
								free_struct(g); 									// se solicitó hasta el momento del fallo
								g = NULL; 											// y se setea el grafo a NULL
							}
						}else
							printf("Error en alocar memoria\n");
					break;
			}
			free_aux_data(n,m,edge); 												// Se liberan datos auxiliares
			break;
	}
	return g;
}


void DestruccionDelGrafo(Grafo g){
	if(g->v != NULL){
		for(u32 i = 0; i < g->n; i++){
			free(g->v[i]->neighbors);
			free(g->v[i]);
		}
		free(g->v);
	}
	free(g->available_colors);
	free(g);
}

Grafo CopiarGrafo(Grafo g){
	Grafo copy = (Grafo)malloc(sizeof(struct GrafoSt));       //puntero a retornar
	u32** c_edge = (u32**)malloc(2 * (g->m) * sizeof(u32*));  //matriz de lados, tendra 2*(lados de g) filas
	for(u32 k = 0; k < 2*(g->m); k++){
				c_edge[k] = (u32 *)malloc(2*sizeof(u32));     //cada fila tiene dos columnas
	}
	u32 added_rows = 0;                                       //cant filas añadidas
	for(u32 i = 0; i < (g->n); ++i){
		for (u32 j = 0; j < (g->v[i]->degree); ++j){
			c_edge[j+added_rows][0] = g->v[i]->name;
			c_edge[j+added_rows][1] = g->v[i]->neighbors[j]->name;
		}
		added_rows = added_rows + (g->v[i]->degree);
	}
	//Es necesario ordenar el arreglo para llamar a set_graph
	qsort(c_edge, 2*(g->m), 2*sizeof(u32), cmp_edge);
	set_graph(copy, c_edge, g->n, g->m);
	if(c_edge != NULL){
		for(u32 j = 0; j < 2*(g->m); j++){
			if(c_edge[j] != NULL)
				free(c_edge[j]);
		}
		free(c_edge);
	}
	//Ahora le damos a copy el coloreo de g
	for (u32 i = 0; i < (copy->n); ++i){
		copy->v[i]->color = g->v[i]->color;
	}
	return copy;
}

u32 NumeroDeVertices(Grafo g){
	return g->n;
}

u32 NumeroDeLados(Grafo g){
	return g->m;
}

u32 NumeroDeColores(Grafo g){
	return g->colors;
}

u32 NombreDelVertice(Grafo g, u32 i){
	return g->v[i]->name;
}

u32 ColorDelVertice(Grafo g, u32 i){
	return g->v[i]->color;
}

u32 GradoDelVertice(Grafo g, u32 i){
	return g->v[i]->degree;
}

u32 ColorJotaesimoVecino(Grafo g, u32 i, u32 j){
	if(i < g->n && j < g->v[i]->degree)
		return g->v[i]->neighbors[j]->color;
	else
		return UINT32_MAX;
}

u32 NombreJotaesimoVecino(Grafo g, u32 i, u32 j){
	return g->v[i]->neighbors[j]->name;
}

u32 Greedy(Grafo g){
	u32 c = 0; 								// Almacena color
	g->v[0]->color = c; 					// Colorea al primer vertice(en el orden dado)
	g->v[0]->is_colored = true; 			// Setea color_FLAG a 1 para indicar que este vertice ya esta coloreado
	g->colors = c+1; 						// Aumenta la cantidad de colores usados en el coloreo
	for(u32 i = 1; i < g->n; i++) 			// Setea el color_FLAG a 0 de todos los vertices(menos el primero) 
		g->v[i]->is_colored = false; 		// para indicar que no estan coloreados
	for(u32 i = 1; i < g->n; i++){ 
		set_colors(g, i); 					// Actualiza el arreglo g->available_colors con los
											// colores ya utilizados en los vecinos del i-esimo vertice
		c = find_color(g); 					// Busca el menor color no usado por los vecinos del i-esimo vertice
		g->v[i]->color = c; 				// Asigna el color al i-esimo vertice
		g->v[i]->is_colored = true; 		// Setea color_FLAG a 1 para indicar que este vertice ya esta coloreado
		clean_colors(g, i); 				// Resetea el arreglo g->available_colors
		if((c+1) > g->colors) 				// Checkea si hay que aumentar la cantidad de colores usados en el coloreo
			g->colors = c+1;
	}
	return g->colors;
}

int Bipartito(Grafo g){
	struct queue * v_queue = (struct queue *)malloc(sizeof(struct queue));  //Creación de una cola de vértices.
	empty(v_queue, g);                                                      //Se reserva espacio para guardar vértices adentro
	u32 colored = 0;                                                        //colored guarda la cantidad de vértices coloreados.
	for(u32 i = 0; i < NumeroDeVertices(g); i++){
		g->v[i]->is_colored = false;                                        //is_colored de cada vértice debe ser false al comienzo.
	}
	struct vertex * root = NULL;                                            //root sera la raiz de una componente conexa
	while(colored < NumeroDeVertices(g)){
		u32 l_chosen = 0;                                                   //l_chosen indica a partir de dónde empezar a buscar
		for (u32 k = l_chosen; k < NumeroDeVertices(g); ++k){               //Buscamos un vertice tal que no ha sido coloreado.
			if(g->v[k]->is_colored!=true){                                  //Si encontramos uno, entonces
				root = g->v[k];                                             //lo guardamos en root
				root->color = 0;                                            //lo coloreamos con 0
				root->is_colored = true;                                    //decimos que ya está coloreado
				colored++;                                                  //pues se coloreó un vértice
				l_chosen = k+1;                                             //luego no volveremos a buscar en vértices anteriores
				break;
			}
		}
		enqueue(v_queue, g, root);                                          //agregamos tal vertice a la cola
		while(v_queue->size != 0){
			struct vertex * p = first(v_queue);                             //Se toma el primer elemento de la v_queue
			dequeue(v_queue, g);                                            //y se lo quita.
			for (u32 i = 0; i < p->degree; ++i){                            //Recorremos todos los vecinos de p.
				if(p->neighbors[i]->is_colored != true){                    //si el vecino no esta coloreado
					enqueue(v_queue, g, p->neighbors[i]);                   //lo agregamos a la cola
					p->neighbors[i]->color = 1 - p->color;                  //lo pintamos del color que no tiene p
					p->neighbors[i]->is_colored = true;
					colored++;
				}
			}
		}
	}
	u32 answer = 1;                                                         //Verificamos si el coloreo dado es propio.
	for (u32 i = 0; answer && i < g->n; ++i){
		struct vertex * verti = g->v[i];
		for (u32 j = 0; answer && j < verti->degree; ++j){
			answer = verti->color != verti->neighbors[j]->color;
		}
	}
	free(v_queue->vertexes);                                                //Liberamos la memoria utilizada por la v_queue.
	free(v_queue);
	if(answer==0){                                                          //Greedy da un coloreo propio si no lo era.
		Greedy(g);
	}
	return answer;
}

char OrdenNatural(Grafo g){
	qsort(g->v, g->n, sizeof(struct vertex *), cmp_name);
	return 0;
}

char OrdenWelshPowell(Grafo g){
	qsort(g->v, g->n, sizeof(struct vertex *), cmp_degree);
	return 0;
}

char SwitchVertices(Grafo g, u32 i, u32 j){
	if(i < g->n && j < g->n){
		struct vertex * aux = g->v[i];
		g->v[i] = g->v[j];
		g->v[j] = aux;
		return 0;
	}else
		return 1;
}

char RMBCnormal(Grafo g){
	qsort(g->v, g->n, sizeof(struct vertex *), cmp_color_normal);
	return 0;
}

char RMBCrevierte(Grafo g){
	qsort(g->v, g->n, sizeof(struct vertex *), cmp_color_reverse);
	return 0;
}

char RMBCchicogrande(Grafo g){
	u32* color_count = (u32 *)calloc(g->colors,sizeof(u32));					//arreglo q almacena la cantidad de ocurrencias de cada color
	if(color_count == NULL)
		return 1;															
	for(u32 j = 0; j < g->n; j++)												//este ciclo va aumentando la ocurrencia de los colores
		color_count[g->v[j]->color]++;
	for(u32 j = 0; j < g->n; j++) 												//este ciclo setea la prioridad q va a tener cada vertice
		g->v[j]->RMBCchicogrande_priority = color_count[g->v[j]->color]; 		//La prioridad de un vertice es el # de veces q aparecio
																				//el color del cual esta pintado
																				//si por ejemplo hay 2 vertices pintados con el color 0 y
																				//hay uno solo pintado con el color 1, el vertice pintado
																				//con el color 1 tendra mayor prioridad q los vertices
																				//pintado con el color 0
	qsort(g->v, g->n, sizeof(struct vertex *), cmp_RMBCchicogrande_priority); 	//ordena los vertices dependiendo de la prioridad, segun
																				//el comportamiento antes descripto
	free(color_count);
	return 0;
}

char SwitchColores(Grafo g, u32 i, u32 j){
	if(i<g->colors && j<g->colors){
		for(u32 i = 0; i < g->n; i++){
			if(g->v[i]->color == i){
				g->v[i]->color = j;
			}else if(g->v[i]->color==j){
				g->v[i]->color = i;
			}else{
				continue;
			}
		}
		return 0;
	}else{
		return 1;
	}
}
