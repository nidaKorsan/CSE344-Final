#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int dest;//destination node
    int src;//source node
}edge_t;//src->dest

typedef struct node{
    int vertex;
    struct node *next;
}node_t;

typedef struct{
    int numVertice;//number of vertices
    int numEdge;//number of edges
    node_t **adjLists;//unknown number of vertices, adjaceny lists list
}graph_t;

typedef struct qnode{
    int data;
    struct qnode *prev;      //pointer to previous node (Closer to front)
    struct qnode *next;      //pointer to next node (Closer to back)
}qnode_t;

typedef struct{
    qnode_t *front;
    qnode_t *rear;
}queue_t;

node_t* createNode(int v);
graph_t* createGraph(int vertices);
void addEdge(graph_t* graph, edge_t newEdge);
void destroyGraph(graph_t *g);
void printGraph(graph_t* graph);
queue_t* initQueue();
void enqueue(queue_t *queue, int element);
void dequeue(queue_t *queue, int *retElem);
void displayQueue(queue_t *queue);
void freeQueue(queue_t *queue);


