#include "Rii.h"
#include "graph_struct.h"
#include "queue.h"

void empty(struct queue * q, Grafo g){        //espacio reservado para guardar maximo n vertices
	q->size = 0;
	q->fst = 0;
	q->vertexes = (struct vertex **)malloc((g->n)*sizeof(struct vertex *));
}

void enqueue(struct queue * q, Grafo g, struct vertex* v){
	q->vertexes[(q->fst+q->size)%(g->n)] = v;
	q->size++;
}

void dequeue(struct queue * q, Grafo g){
	q->size--;
	q->fst = (q->fst + 1)%(g->n);
}

struct vertex * first(struct queue * q){
	return q->vertexes[q->fst];
}