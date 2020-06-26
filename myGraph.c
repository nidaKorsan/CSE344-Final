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
