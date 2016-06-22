#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int noOfEdges;

struct Node{
	int degree;
	struct EdgeEnd* edge;
};

struct EdgeEnd{
	struct EdgeEnd* otherSide;
	int visited;
	struct EdgeEnd* next;
	int targetNodeIdx;
};

FILE *openFile(int argc, char* argv[]) {
	if (argc != 2){
		printf("Invalid input");
		exit(1);
	}

	return fopen(argv[1], "r");
}

int readNoOfVertices(FILE *file){
	char * line = NULL;
	size_t zero = 0;

	getline(&line, &zero, file);
	int noOfVertices = atoi(line);
	free(line);
	return noOfVertices;
}

void getEdges(struct Node *nodes, int edgeNode1, int edgeNode2){
	struct EdgeEnd *edgeNode1EdgeEnd = calloc(1, sizeof(struct EdgeEnd));
	struct EdgeEnd *edgeNode2EdgeEnd = calloc(1, sizeof(struct EdgeEnd));

	edgeNode2EdgeEnd->otherSide = edgeNode1EdgeEnd;
	edgeNode1EdgeEnd->otherSide = edgeNode2EdgeEnd;

	edgeNode1EdgeEnd->targetNodeIdx = edgeNode2;
	edgeNode2EdgeEnd->targetNodeIdx = edgeNode1;

	edgeNode1EdgeEnd->next = nodes[edgeNode1].edge;
	nodes[edgeNode1].edge = edgeNode1EdgeEnd;

	edgeNode2EdgeEnd->next = nodes[edgeNode2].edge;
	nodes[edgeNode2].edge = edgeNode2EdgeEnd;

	nodes[edgeNode1].degree++;
	nodes[edgeNode2].degree++;
}

void getLines(FILE *file, int noOfVertices, struct Node *nodes) {
	char *line = NULL;
	size_t n = 0;

	memset(nodes, 0, noOfVertices*sizeof(struct Node));
	
	while(getline(&line, &n, file) > 0){
		char *left = strsep(&line, " ");
		int edgeNode1 = atoi(left);
		int edgeNode2 = atoi(line);
		getEdges(nodes, edgeNode1, edgeNode2);
		noOfEdges++;

		free(left);	
		line = NULL;
	}

	free(line);
	fclose(file);
}

int eulerCheck(int noOfVertices, struct Node *nodes){
	int no_of_odd_nodes = 0;
	int temp_nodeindex = 0;
	int keepMe[2];

	while(temp_nodeindex < noOfVertices){
		if(nodes[temp_nodeindex].degree % 2 == 1){
			if(no_of_odd_nodes <= 2){
				keepMe[no_of_odd_nodes] = temp_nodeindex;
			}
			no_of_odd_nodes++;
		}
		temp_nodeindex++;		
	}

	if(no_of_odd_nodes == 2){
		return keepMe[0];
	}else if(no_of_odd_nodes == 0){
		//start anywhere
		srand(time(NULL));
		return ( rand() % noOfVertices);
	}else if(no_of_odd_nodes != 2){
		printf("Euler path is not possible\n");
		exit(-1);
	}
}

int main(int argc, char* argv[]){
	FILE *file = fopen(argv[1], "r");
	
	int noOfVertices = readNoOfVertices(file);

	struct Node nodes[noOfVertices];
	getLines(file, noOfVertices, nodes);

	int startNodeIdx = eulerCheck(noOfVertices, nodes);

	int noOfPassedEdges = 0;	
	int *backMeup = malloc(noOfEdges*sizeof(int));
	memset(backMeup, -1, noOfEdges*sizeof(int));

	backMeup[noOfPassedEdges] = startNodeIdx;		

LOOPMEAGAIN: 	

	while(noOfPassedEdges != noOfEdges){		
		int target = -1;
		struct EdgeEnd *currentConnection = nodes[startNodeIdx].edge;
		while(target == -1){		

			if(!currentConnection->visited){				
				target = currentConnection ->targetNodeIdx;
				noOfPassedEdges++;
				currentConnection-> visited = 1;
				currentConnection -> otherSide -> visited = 1;	

				struct EdgeEnd *currentTest = nodes[startNodeIdx].edge;
				while(currentTest->next != NULL){
					if(currentTest->visited == -1){
						currentTest->visited = 0;
						currentTest->otherSide->visited = 0;
					}	
					currentTest = currentTest->next;					
				}
				startNodeIdx = target;
				backMeup[noOfPassedEdges] = startNodeIdx;

			}else if(currentConnection->next != NULL){
				currentConnection = currentConnection -> next;

			}else{				
				currentConnection->visited = -1;
				currentConnection->otherSide->visited = -1;
				
				if(noOfPassedEdges < 0){
					exit(-1);
				}
				noOfPassedEdges--;	
				startNodeIdx = backMeup[noOfPassedEdges];				
				goto LOOPMEAGAIN;	
			}	
		}	
	}

	printf("Current Path: \t");
	for (int i = 0; i <= noOfPassedEdges; i++){
		printf("%d ",backMeup[i]);

		struct EdgeEnd *releaseConnection = nodes[backMeup[i]].edge;
		struct EdgeEnd *cleanup;
		while(releaseConnection->next != NULL){
			cleanup = releaseConnection;
			releaseConnection = releaseConnection->next;
			free(cleanup);
		}
	}
	printf("\n");
	free(backMeup);
	printf("done\n");
}

//memory leaks with calloc
//cannot free the linked list
//problems with backmeup
