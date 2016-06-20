#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int noOfVertices;
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

void getNodes(FILE *file, char *line, size_t n){
	getline(&line, &n, file);
	noOfVertices = atoi(line);
	//printf("DEBUG:	%d\n", noOfVertices);
	free(line);
	line=NULL;
}

void getEdges(struct Node *nodes, int edgeNode1, int edgeNode2){
	//printf("DEBUG:	create node from %d to %d\n", edgeNode1, edgeNode2);

	struct EdgeEnd *edgeNode1EdgeEnd = calloc(1, sizeof(struct EdgeEnd));
	struct EdgeEnd *edgeNode2EdgeEnd = calloc(1, sizeof(struct EdgeEnd));

	edgeNode2EdgeEnd->otherSide = edgeNode1EdgeEnd;
	edgeNode1EdgeEnd->otherSide = edgeNode2EdgeEnd;

	edgeNode1EdgeEnd->targetNodeIdx = edgeNode2;
	edgeNode2EdgeEnd->targetNodeIdx = edgeNode1;

	edgeNode1EdgeEnd->next = nodes[edgeNode1].edge;	//edges with connections
	nodes[edgeNode1].edge = edgeNode1EdgeEnd;
	//printf("DEBUG:	add edge end to %d with target node idx %d\n", edgeNode1, edgeNode1EdgeEnd->targetNodeIdx);

	edgeNode2EdgeEnd->next = nodes[edgeNode2].edge;
	nodes[edgeNode2].edge = edgeNode2EdgeEnd;
	//printf("DEBUG:	add edge end to %d with target node idx %d\n", edgeNode2, edgeNode2EdgeEnd->targetNodeIdx);

	nodes[edgeNode1].degree++;
	nodes[edgeNode2].degree++;
}

int eulerCheck(struct Node *nodes){
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
		//printf("START with %d \n", currentNode);
	}else if(no_of_odd_nodes == 0){
		//start anywhere
		//printf("You may start anywhere");
		srand(time(NULL));
		return ( rand() % noOfVertices);
	}else if(no_of_odd_nodes != 2){
		printf("Euler path is not possible\n");
		exit(-1);
	}
}

int main(int argc, char* argv[]){

	if (argc != 2){
		printf("Invalid input");
		exit(1);
	};

	FILE *file = fopen(argv[1], "r");
	char *line = NULL;
	size_t n = 0;

	getNodes(file, line, n);

	struct Node nodes[noOfVertices];
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

	int currentNode = eulerCheck(nodes);

	int noOfPassedEdges = 0;
	
	int *allPossibilities = malloc(noOfVertices*sizeof(int));
	int counter = 0;
	int *backMeup = malloc(noOfEdges*sizeof(int));
	backMeup[counter] = currentNode;	

	printf("%d\n", currentNode);

LOOPMEBACK: 	

while(noOfPassedEdges != noOfEdges){
	noOfPassedEdges++;	
		
		int target = -1;
		struct EdgeEnd *currentConnection = nodes[currentNode].edge;	

		while(target == -1){		

			if(!currentConnection->visited){				
				target = currentConnection ->targetNodeIdx;
				

				struct EdgeEnd *currentTest = nodes[currentNode].edge;
				int possibilityPtr = 0;
				while(currentTest->next != NULL){
					currentTest = currentTest->next;
					allPossibilities[possibilityPtr++] = currentTest->targetNodeIdx;		
				}
// Let's check what we even got in allPossibilities

				printf("All possbilities \t");
				for (int i = 0; i < possibilityPtr; i++){
					printf("%d  ", allPossibilities[i]);
				}
				printf("\n");


			}else if(currentConnection->next != NULL){
				currentConnection = currentConnection -> next;

			}else{
					//printf("DEBUG: backtrack needed\n");
							currentConnection->visited = -1;
							currentConnection->otherSide->visited = -1;
							backMeup[counter] = NULL;
							counter--;

												//printf("DEBUG: counter index addressing %4d\n", counter );
							if(counter >= 0){
								currentNode = backMeup[counter];
								noOfPassedEdges--;
								goto LOOPMEBACK;
							}		
			}	

		}		

		currentConnection-> visited = 1;
		currentConnection -> otherSide -> visited = 1;	

	//	printf("%d  ", target);

		printf("route from %d to %d\n", currentNode, target);

		printf("\n");

		currentNode = target;
		backMeup[++counter] = currentNode;

//	A quick checkpoint to see which path is calculated


		
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
	free(allPossibilities);
	printf("done\n");
}