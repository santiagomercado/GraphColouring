struct queue {
	u32 size;
	u32 fst;
	struct vertex** vertexes;
};

void empty(struct queue * q, Grafo g);
void enqueue(struct queue * q, Grafo g, struct vertex* v);
void dequeue(struct queue * q, Grafo g);
struct vertex * first(struct queue * q);