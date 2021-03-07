/**
Integrantes del grupo:
- Mercado, Santiago. E-mail: santiagomercado1997@gmail.com
- Capurro, Franco Nahuel. E-mail: franncapurro@gmail.com
**/

#ifndef RII_H
#define RII_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef uint32_t u32;

typedef struct GrafoSt *Grafo;

// Creates the graph
Grafo ConstruccionDelGrafo();

// Destroys the graph
void DestruccionDelGrafo(Grafo g);

// Copies the graph
Grafo CopiarGrafo(Grafo g);

// Graph colouring greedy algorithm
u32 Greedy(Grafo g);

// Bipartite function
int Bipartito(Grafo g);

// Returns the number of vertices of the graph
u32 NumeroDeVertices(Grafo g);

// Returns the number of edges of the graph
u32 NumeroDeLados(Grafo g);

// Returns the number of colours
// used in the graph colouring
u32 NumeroDeColores(Grafo g);

// Returns the real name of the vertex
// for the given index i
u32 NombreDelVertice(Grafo g, u32 i);

// Returns the colour of the vertex
// for the given index i
u32 ColorDelVertice(Grafo g, u32 i);

// Returns the degree (or valency) of
// the vertex for the given index i.
u32 GradoDelVertice(Grafo g, u32 i);

// Returns the colour of the jth
// neighbor to the ith vertex
u32 ColorJotaesimoVecino(Grafo g, u32 i, u32 j);

// Returns the name of the jth neighbor
// to the ith vertex
u32 NombreJotaesimoVecino(Grafo g, u32 i, u32 j);

// This function sorts the vertices by
// the "real" name of the vertices
char OrdenNatural(Grafo g);

// This function sorts the vertices according
// to the Welsh-Powell sorting (i.e sorts the
// vertices by its degree in a non-increasing order)
char OrdenWelshPowell(Grafo g);

// This function switches two vertices in the
// internal order of the graph
char SwitchVertices(Grafo g,u32 i, u32 j);

// This function sorts the vertices by its colour.
// For example if the graph has 6 vertices and has
// a list of colours 0,2,1,0,1,1, then the RMBCnormal()
// will generate the order 0,0,1,1,1,2
char RMBCnormal(Grafo g);

// Similiar as RMBCnormal() but sorts in the opposite
// way.For example if the graph has 6 vertices and has
// a list of colours 0,2,1,0,1,1, then the RMBCnormal()
// will generate the order 2,1,1,1,0.
char RMBCrevierte(Grafo g);

// This function sorts the vertices by the number of
// repetitions of the color of the vertex. For example
// if the graph has 6 vertices and has a list of
// colours 0,2,1,0,1,1, then the RMBCchicogrande()
// will generate the order 2,0,0,1,1,1.
char RMBCchicogrande(Grafo g);

// This function Switches the colour of the vertices.
// EVERY vertex coloured with i will be coloured
// with j and vice versa
char SwitchColores(Grafo g, u32 i, u32 j);
#endif
