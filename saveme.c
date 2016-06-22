#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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


int main(int argc, char* argv[]){

	if (argc != 2){
		printf("Invalid input");
		exit(1);
	}

	FILE *file = fopen(argv[1], "r");
	char *line = NULL;
	size_t n = 0;

	getline(&line, &n, file);
	int noOfVertices = atoi(line);
	printf("%d\n", noOfVertices);
	free(line);
	line=NULL;

	struct Node nodes[noOfVertices];
	memset(nodes, 0, noOfVertices*sizeof(struct Node));

	int noOfEdges = 0;

	while(getline(&line, &n, file) > 0){
		noOfEdges++;

		char *left = strsep(&line, " ");
		int edgeNode1 = atoi(left);
		int edgeNode2 = atoi(line);

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

		free(left);
		line = NULL;
	}

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

	int currentNode;

	if(no_of_odd_nodes == 2){
		currentNode = keepMe[0];
		printf("START with %d \n", currentNode);
	}else if(no_of_odd_nodes == 0){
		//start anywhere
		printf("You may start anywhere");
		srand(time(NULL));
		currentNode = ( rand() % noOfVertices);
	}else if(no_of_odd_nodes != 2){
		printf("Euler path is not possible\n");		
	}

	int noOfPassedEdges = 0;
	
	int counter = 0;
	int *backMeup = malloc(noOfEdges*sizeof(int));
	memset(backMeup, -1, noOfEdges*sizeof(int));

	backMeup[counter] = currentNode;


LOOPMEBACK: 	

	while(noOfPassedEdges != noOfEdges){
		int target = -1;
		struct EdgeEnd *currentConnection = nodes[currentNode].edge;

		while(target == -1){
			if(!currentConnection->visited){				
			target = currentConnection ->targetNodeIdx;
			noOfPassedEdges++;

			struct EdgeEnd *currentTest = nodes[currentNode].edge;
				while(currentTest->next != NULL){
					if(currentTest->visited == -1){
						currentTest->visited = 0;
						currentTest->otherSide->visited = 0;
					}	
				currentTest = currentTest->next;						
				}

			}else if(currentConnection->next != NULL){
				currentConnection = currentConnection -> next;

			}else{					
				currentConnection->visited = -1;
				currentConnection->otherSide->visited = -1;
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

 	printf("Current Path: \t");
	for (int i = 0; i <= counter; i++){
		printf("%d ",backMeup[i]);
	}
	printf("\n");

	free(line);
	free(backMeup);
	printf("done\n");
}