#include "Rii.h"
#include "graph_struct.h"


int main(){
	Grafo g,h;
	// Loads a graph g (reads from stdin)
	g = ConstruccionDelGrafo();
	if(g != NULL){
		// Generates a copy h of the loaded graph
		h = CopiarGrafo(g);
	}else
		h = NULL;
	if(h != NULL){
		// Sorts the vertices of g by the real name of
		// the vertices
		OrdenNatural(g);
		// Runs the greedy algorithm and prints the number
		// of colours used in the resulting colouring.
		printf("OrdenNatural: %u colours\n", Greedy(g));
		// Sorts the vertices of h (copy) by its degree in a non-increasing order
		OrdenWelshPowell(h);
		// Runs the greedy algorithm and prints the number
		// of colours used in the resulting colouring.
		printf("OrdenWelshPowell: %u colours\n", Greedy(h));
		// Destroys both graphs
		DestruccionDelGrafo(g);
		DestruccionDelGrafo(h);
	}else
		printf("Graph: %p\n", (void*)g);
	return 0;
}
