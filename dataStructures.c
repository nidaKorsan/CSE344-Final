/**********************************************
; *  344 Sytem Programming FINAL              *
; *  Spring 2020                              *
; *  Author: Nida Korsan                      *
; *********************************************/

#include "dataStructures.h"

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

void displayQueue(queue_t *queue)
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

int isQueueEmpty(queue_t *queue){
  if(queue->front == NULL)
    return 1;
  return 0;
}

void freeQueue(queue_t *queue){
    while(queue->front != NULL){
        qnode_t *temp = queue->front;
        queue->front = queue->front->next;
        free(temp);
    }
    queue->rear = NULL;
    free(queue);
    //free(queue->front);
}

char* bfsSearch(graph_t *graph, int source, int destination){
	char *ret = calloc(100,sizeof(char));
	char tempBuf[20] = "";
	if(source == destination){
        sprintf(ret, "%d->%d",source, destination);
		return ret;//both nodes are the same a->a
	}
	int pred[graph->numVertice], dist[graph->numVertice];
	int *visited = (int*)calloc(graph->numVertice, sizeof(int));
	int i, flag = 0;
	qnode_t *tempFront;
	node_t* temp;
	for(i = 0; i < graph->numVertice; ++i){
		dist[i] = INTMAX; 
        pred[i] = -1;
	}
	queue_t * queue = initQueue();
	visited[source] = 1;
	dist[source] = 0;
	enqueue(queue, source);
	while(!isQueueEmpty(queue)){
		dequeue(queue, &source);
		temp = graph->adjLists[source];
		while (temp) {
			if(!visited[temp->vertex]){//if adjacent is not visited
				visited[temp->vertex] = 1;//mark it visited
				dist[temp->vertex] = dist[source] + 1;
				pred[temp->vertex] = source;
				enqueue(queue, temp->vertex);//add it to the queue
				if(temp->vertex == destination){
					tempFront = queue->front;
					queue->front = NULL;
					flag = 1;
					break;
				}
			}
			temp = temp->next;
		}
	}
	if(!flag){
		strcpy(ret, "NO PATH");
		return ret;
	}
	else{
		queue->front = tempFront;
		int path[dist[destination] + 1], pathIn = 0;
		ret = realloc(ret, (dist[destination] + 1) * 10 * sizeof(char));
		strcpy(ret, "");
		int crawl = destination;
		path[pathIn++] = crawl;
		while(pred[crawl] != -1){
			path[pathIn++] = pred[crawl];
			crawl = pred[crawl];
		}
		for(i = pathIn - 1; i >= 0; --i){
			if(i == 0)sprintf(tempBuf,"%d", path[i]);
			else sprintf(tempBuf,"%d -> ", path[i]);
			strcat(ret, tempBuf);
			*tempBuf = '\0';
		}
	}
	freeQueue(queue);
	return ret;
}

void printList(linkedList_t *list) {
   lnode_t*ptr = list->head;
   printf("\nList is [ ");
	
   //start from the beginning
   while(ptr != NULL) {
      printf("(%d %d,%s) ",ptr->keysrc, ptr->keydest, ptr->data);
      ptr = ptr->next;
   }
   printf(" ]\n");
}

void insertFirst(linkedList_t *list, int keysrc, int keydest, char* data) {
    char *temp = find(list, keysrc, keydest);
    //printf("char data in insert %s\n", data);
    if(temp == NULL){//if it's not already in list
        //printf("Its not in the list %d->%d\n", keysrc, keydest);
        //create a link
        lnode_t *link = (lnode_t*) malloc(sizeof(lnode_t));
        link->keysrc = keysrc;
        link->keydest = keydest;
        link->data = malloc(sizeof(char) * strlen(data));
        strcpy(link->data, data);
        //point it to old first node
        link->next = list->head;
        //point first to new first node
        list->head = link;
    }
}

int isEmptyList(linkedList_t *list) {
   return (list->head == NULL) ? 1 : 0;
}

//find a link with given key
char* find(linkedList_t *list, int keysrc, int keydest){
    //start from the first link
    lnode_t* current = list->head;
    //if list is empty
    if(isEmptyList(list)){
        return NULL;
    }
    //navigate through list
        //printf("current key %d %d, given key %d %d\n", current->keysrc, current->keydest, keysrc, keydest);
    while(current->keysrc != keysrc || current->keydest != keydest){
        //if it is last node
        if(current->next == NULL){
            return NULL;
        }
        else{
            //go to next link
            current = current->next;
        }
    }
    //if data found, return the current Link
    return current->data;
}

void destroyList(linkedList_t *list){
    while(list->head != NULL){
        lnode_t*tempLink = list->head;
        //mark next to first link as first 
        list->head = list->head->next;
        free(tempLink->data);
        free(tempLink);
    }
	free(list);
}

linkedList_t* initList(){
	linkedList_t *list = (linkedList_t*)malloc(sizeof(linkedList_t));
	list->head = NULL;
	return list;
}