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

	//	if(edgeNode1 == edgeNode2){			// ignore loops
	//		continue;
	//	}

		printf("create node from %d to %d\n", edgeNode1, edgeNode2);

		struct EdgeEnd *edgeNode1EdgeEnd = calloc(1, sizeof(struct EdgeEnd));
		struct EdgeEnd *edgeNode2EdgeEnd = calloc(1, sizeof(struct EdgeEnd));

		edgeNode2EdgeEnd->otherSide = edgeNode1EdgeEnd;
		edgeNode1EdgeEnd->otherSide = edgeNode2EdgeEnd;

		edgeNode1EdgeEnd->targetNodeIdx = edgeNode2;
		edgeNode2EdgeEnd->targetNodeIdx = edgeNode1;

		edgeNode1EdgeEnd->next = nodes[edgeNode1].edge;	//edges with connections
		nodes[edgeNode1].edge = edgeNode1EdgeEnd;
		printf("add edge end to %d with target node idx %d\n", edgeNode1, edgeNode1EdgeEnd->targetNodeIdx);

		edgeNode2EdgeEnd->next = nodes[edgeNode2].edge;
		nodes[edgeNode2].edge = edgeNode2EdgeEnd;
		printf("add edge end to %d with target node idx %d\n", edgeNode2, edgeNode2EdgeEnd->targetNodeIdx);

		nodes[edgeNode1].degree++;
		nodes[edgeNode2].degree++;

		free(left);
		//free(line);
		line = NULL;
	}

	// check for n euler path
	// make it more elegant

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

	printf("DEBUG number of vertices with odd degree: %d\n", no_of_odd_nodes );

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
	
	int *stack = malloc(noOfVertices*sizeof(int));
	int counter = 1;
	stack[0] = currentNode;
	int backMeup;

//	printf("%d  ", currentNode);
	while(noOfPassedEdges < noOfEdges){
		noOfPassedEdges++;
		int target = -1;
		struct EdgeEnd *currentConnection = nodes[currentNode].edge;

		// for(int index = 0; index < noOfVertices; index++){
		// 	struct EdgeEnd *currentTest = nodes[index].edge;

		// 	printf(" DEBUG %d - %d visited tag: %d\n", index, nodes[index].edge -> targetNodeIdx, nodes[index].edge -> visited);
			
		// 	if( currentTest->next != NULL ){
		// 		currentTest = nodes[index].edge->next;	
		// 		printf(" DEBUG %d - %d visited tag: %d\n", index, currentTest->targetNodeIdx, currentTest->visited);
		// 	}

		// }		
		

		while(target == -1){			

			if(!currentConnection->visited){
				if(currentConnection ->targetNodeIdx != backMeup){
					target = currentConnection ->targetNodeIdx;
				}				
				stack[counter] = target;
				counter++;

			}else if(currentConnection->next != NULL){
				currentConnection = currentConnection -> next;

			}else{
				printf("backtrack needed\n");
				currentNode = stack[counter-1];
				backMeup = stack[counter];
				return 0;
			}	

		}
		

		currentConnection-> visited = 1;
		currentConnection -> otherSide -> visited = 1;	

	//	printf("%d  ", target);

		printf("route from %d to %d\n", currentNode, target);
		printf("stack atm: ");
		for (int i = counter-1; i >=0 ; i--){				
			printf("%d\t", stack[i]);			
		}
		printf("\n");

		currentNode = target;
		
	}

	free(line);
	printf("done\n");
}

