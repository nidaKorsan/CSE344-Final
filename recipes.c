/**********************************************
; *  344 Sytem Programming FINAL              *
; *  Spring 2020                              *
; *  Author: Nida Korsan                      *
; *********************************************/
#include"recipes.h"


int daemonBorn(mainArgs *margs){
    pid_t sid;
    switch (fork()) { //Become background process 
        case -1:
            printf("Error during fork for server %s\n", strerror(errno));
            return -1;
        case 0: break; //Child falls through; adopted by init
        default: _exit(EXIT_SUCCESS);//parent terminates and shell prompt is back
    }
    //start a new session and free itself of any association with a controlling terminal
    if((sid = setsid()) == -1){
        printf("Error during setid %s\n", strerror(errno));
    }
    switch (fork()) {//Ensure we are not session leader 
        case -1:// thanks to 2nd fork, there is no way of acquiring a tty
            printf("Error during fork for server %s\n", strerror(errno));
            return -1;
        case 0: break;
        default: _exit(EXIT_SUCCESS);
    }
    umask(0);//Clear file mode creation mask
    //TODO may uncomment this later
    /*if ((chdir("/")) < 0) {// Change to root directory
        printf("Error during chdir %s\n",strerror(errno));
        return -1;
    }*/
    int i;
    //Close all of the open file descriptors (all possible ones)
    for (i = sysconf(_SC_OPEN_MAX); i >= 0; close (i--));
    //open input file and output log file
    if((margs->fin = open(margs->inputPath, O_RDWR)) == -1){
        printf("Error while opening file %s\n", strerror(errno));
        return -1;
    }
    if((margs->fout = open(margs->outputPath, O_RDWR | O_CREAT)) == -1){
        printf("Error while opening file %s\n", strerror(errno));
        close(margs->fin);
        return -1;
    }
    /*char lokum[25]; strcpy(lokum,"lokumgib"); 
    write(margs->fout, lokum, strlen(lokum));*/
    //sleep(6);
    return 0;
}

//for reading command line arguments
int readArguments(int argc, char *argv[], mainArgs *margs){
    int opt;
    int flag_i = 0, flag_o = 0, flag_p = 0, flag_s = 0, flag_x = 0;
    //./server -i pathToFile -p PORT -o pathToLogFile -s 4 -x 24
    if(argc != 11) {
        printf("Wrong argument count. There should be exactly 11.\nUsage case : ./server -i pathToFile -p PORT -o pathToLogFile -s 4 -x 24\n");
        return -1;
    }
    while((opt = getopt(argc, argv, "i:p:o:s:x:")) != -1)
    {
        switch(opt)
        {
            case 'i':
                if (flag_i){
                    printf("-i option is already given.\nUsage case : ./server -i pathToFile -p PORT -o pathToLogFile -s 4 -x 24\n");
                    return -1;
                } /*If -i parameter is already given give error*/
                margs->inputPath = optarg;
                flag_i = 1;
                break;
            case 'p':
                if(flag_p){
                    printf("-p option is already given.\nUsage case : ./server -i pathToFile -p PORT -o pathToLogFile -s 4 -x 24\n");
                    return -1;
                } /*If -p parameter is already given give error*/
                margs->port = atoi(optarg);
                flag_p = 1;
                break;
            case 'o':
                if(flag_o){
                    printf("-o option is already given.\nUsage case : ./server -i pathToFile -p PORT -o pathToLogFile -s 4 -x 24\n");
                    return -1;
                } /*If -o parameter is already given give error*/
                margs->outputPath = optarg;
                flag_o = 1;
                break;
            case 's':
                if(flag_s){
                    printf("-s option is already given.\nUsage case : ./server -i pathToFile -p PORT -o pathToLogFile -s 4 -x 24\n");
                    return -1;
                } /*If -s parameter is already given give error*/
                if((margs->threadNum = atoi(optarg)) < 2){
                    printf("-s argument cannot be less than 2.\n");
                    return -1;
                }
                flag_s = 1;
                break;
            case 'x':
                if(flag_x){
                    printf("-x option is already given.\nUsage case : ./server -i pathToFile -p PORT -o pathToLogFile -s 4 -x 24\n");
                    return -1;
                } /*If -x parameter is already given give error*/
                margs->maxThreadNum = atoi(optarg);
                flag_x = 1;
                break;                                
            case '?':
                printf("Unknown option\nUsage case : ./server -i pathToFile -p PORT -o pathToLogFile -s 4 -x 24\n");
                return -1;
        default:
            break;
        }
    }
    return 0;
}

//new node with vertex v
node_t* createNode(int v){
  node_t* newNode = malloc(sizeof(node_t));
  newNode->vertex = v;
  newNode->next = NULL;
  return newNode;
}

//Create a graph
graph_t* createAGraph(int verticeNum) {
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