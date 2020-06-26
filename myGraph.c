#include "myGraph.h"

//new node with vertex v
node_t* createNode(int v){
  node_t* newNode = malloc(sizeof(node_t));
  newNode->vertex = v;
  newNode->next = NULL;
  return newNode;
}

//Create a graph
graph_t* createGraph(int verticeNum) {
  graph_t* graph = malloc(sizeof(graph_t));
  graph->numVertice = verticeNum;
  graph->adjLists = malloc(verticeNum * sizeof(node_t*));//allocate memory for verticeNum vertices
  int i;
  for (i = 0; i < verticeNum; i++)
    graph->adjLists[i] = NULL;//initially there are no edges
  return graph;
}

// Add edge from src to dest
void addEdge(graph_t* graph, edge_t newEdge) {
  node_t* newNode = createNode(newEdge.dest);
  newNode->next = graph->adjLists[newEdge.src];
  graph->adjLists[newEdge.src] = newNode;
  ++graph->numEdge;
}

//Free all memory used by graph
void destroyGraph(graph_t *g){
    int i;
    for(i = 0; i < g->numVertice; i++) free(g->adjLists[i]);
    free(g);
}

// Print the graph
void printGraph(graph_t* graph) {
  int v;
  for (v = 0; v < graph->numVertice; v++) {
    node_t* temp = graph->adjLists[v];
    printf("Vertex %d : ", v);
    while (temp) {
      printf("%d -> ", temp->vertex);
      temp = temp->next;
    }
    printf("\n");
  }
}

queue_t* initQueue()
{
    queue_t* queue = (queue_t*)malloc(sizeof(queue_t));
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}

void enqueue(queue_t *queue, int element){
    qnode_t *nptr = malloc(sizeof(qnode_t));
    nptr->data =  element;
    nptr->next = NULL;
    if (queue->rear == NULL || queue->front == NULL){
        queue->front = nptr;
        queue->rear = nptr;
    }
    else{
        queue->rear->next = nptr;
        queue->rear = queue->rear->next;
        //queue->rear->next = NULL;
    }
}
void dequeue(queue_t *queue, int *retElem){
    if (queue->front == NULL || queue->rear == NULL){
        *retElem = -1;
    }
    else{
        qnode_t *temp;
        temp = queue->front;
        queue->front = queue->front->next;
        //printf("Queue front %p\n", queue->front);
        //printf("\n\n%s deleted\n", temp->data);
        *retElem = temp->data;
        free(temp);
    }
}

void display(queue_t *queue)
{
    qnode_t *temp;
    temp = queue->front;
    while (temp != NULL)
    {
        printf("%d\t", temp->data);
        temp = temp->next;
    }
    printf("\n");
}