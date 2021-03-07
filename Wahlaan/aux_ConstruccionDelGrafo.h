#ifndef AUX_CONSTRUCCIONDELGRAFO_H
#define AUX_CONSTRUCCIONDELGRAFO_H

#define CORRECT_DATA 0
#define ERR_FIRST_LINE 1
#define ERR_EDGE 2
#define ERR_VERTEX 3

void free_struct(Grafo g);

void free_aux_data(u32* n, u32* m, u32** edge);

int read_stdin(u32 **ptr_n, u32 **ptr_m, u32*** ptr_edge, u32* ptr_i);

int check_read_data(u32 *n, u32*m, u32** edge, u32 i);

int cmp_edge(const void* x, const void* y);

bool is_vertex_number_correct(u32** edge, u32 n, u32 m);

int set_graph(Grafo g, u32** edge, u32 n, u32 m);
#endif