#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//int noOfVertices;
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

	edgeNode1EdgeEnd->next = nodes[edgeNode1].edge;	//edges with connections
	nodes[edgeNode1].edge = edgeNode1EdgeEnd;

	edgeNode2EdgeEnd->next = nodes[edgeNode2].edge;
	nodes[edgeNode2].edge = edgeNode2EdgeEnd;

	nodes[edgeNode1].degree++;
	nodes[edgeNode2].degree++;
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

FILE *openFile(int argc, char* argv[]) {
	if (argc != 2){
		printf("Invalid input");
		exit(1);
	}

	return fopen(argv[1], "r");
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
}

int main(int argc, char* argv[]){
	FILE *file = openFile(argc, argv);

	int noOfVertices = readNoOfVertices(file);

	struct Node nodes[noOfVertices];
	getLines(file, noOfVertices, nodes);


	

	int currentNode = eulerCheck(noOfVertices, nodes);

	int noOfPassedEdges = 0;
	int counter = 0;
	int *backMeup = malloc(noOfEdges*sizeof(int));
	backMeup[counter] = currentNode;	

	printf("%d\n", currentNode);

	LOOPMEBACK:
	while(noOfPassedEdges != noOfEdges){
		noOfPassedEdges++;	
		
		int target = -1;
		struct EdgeEnd *currentConnection = nodes[currentNode].edge; // get the first edge	

		while(target == -1){		
			if(!currentConnection->visited){				
				target = currentConnection ->targetNodeIdx;
			}else if(currentConnection->next != NULL){
				currentConnection = currentConnection -> next;
			}else{
				currentConnection->visited = -1;
				currentConnection->otherSide->visited = -1;
				backMeup[counter] = 0; // NULL? is this correct?
				counter--;

				if(counter >= 0){
					currentNode = backMeup[counter];
					noOfPassedEdges--;
					goto LOOPMEBACK;
				}		
			}
		}

		currentConnection-> visited = 1;
		currentConnection -> otherSide -> visited = 1;

		currentNode = target;
		backMeup[++counter] = currentNode;
	}

	struct EdgeEnd *checkLastConnection = nodes[currentNode].edge;
	if(checkLastConnection->visited == -1){
		printf("HI, I will fix your problem now\n");
		int lastPoint = checkLastConnection->targetNodeIdx;
		backMeup[++counter]= lastPoint;
	}

	printf("Current Path: \t");
	for (int i = 0; i <= counter; i++){
		printf("%d ",backMeup[i]);
	}
	printf("\n");

	//small check in
	// int nodeMe = 97;
	// struct EdgeEnd *tryingmyBest = nodes[nodeMe].edge;
	// printf("all connections from 97: \n");
	// while(tryingmyBest->next != NULL){
	// 	tryingmyBest = tryingmyBest->next;
	// 	printf(" DEBUG %d - %d visited tag: %d\n", nodeMe, tryingmyBest->targetNodeIdx, tryingmyBest->visited);
	// }

	//free(backMeup);
	printf("done\n");
}