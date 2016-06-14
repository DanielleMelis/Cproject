#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* 	A code we deserve but don't need.  
	Warning: if following side effects are observed, you are strongly
	advised to step away from the screen
	-	questioning the meaning of your life
	-  	an irresistable urge to hit the person behind this code
	-	frustration and losing faith in humanity
	-	worshipping the flying magical pointers

*/

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

struct TraceBackList{
	int NodeID;
	struct TraceBackList* link;
}

void analyze(Node nodes[]) {

}


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

		edgeNode1EdgeEnd->next = nodes[edgeNode1].edge;
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
												//printf("DEBUG: number of edges: %4d\n", noOfEdges); //input100 2614

/*	Check if an Euler path even exists 	*/

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
												//printf("DEBUG: number of vertices with odd degree: %d\n", no_of_odd_nodes );
	int currentNode;

	if(no_of_odd_nodes == 2){
		currentNode = keepMe[0];
		printf("START with %d \n", currentNode);
	}else if(no_of_odd_nodes == 0){
		printf("You may start anywhere");
		srand(time(NULL));
		currentNode = ( rand() % noOfVertices);
	}else if(no_of_odd_nodes != 2){
		printf("Euler path is not possible\n");
		
	}
									
	int noOfPassedEdges = 0;
	int *allPossibilities = malloc(noOfVertices*sizeof(int));
	int counter = 0;
	int *backMeup = malloc(noOfEdges*sizeof(int));
	backMeup[counter] = currentNode;
	

												//printf("DEBUG: %d  ", currentNode);
 	while(noOfPassedEdges != noOfEdges){
		
					int target = -1;
												//printf("DEBUG: current node %d\n", currentNode );
					struct EdgeEnd *currentConnection = nodes[currentNode].edge;

					while(target == -1){		

						if(!currentConnection->visited){				
							target = currentConnection ->targetNodeIdx;

							struct EdgeEnd *currentTest = nodes[currentNode].edge;

							int possibilityPtr = 0;
							while(currentTest->next != NULL){
								currentTest = currentTest->next;
								allPossibilities[possibilityPtr++] = currentTest->targetNodeIdx;
								/* 	recover from previous mistakes 	*/
								if(currentTest->visited == -1){
									currentTest->visited = 0;
									currentTest->otherSide->visited = 0;
								}		
							}


			// Let's check what we even got in allPossibilities

			//				printf("All possbilities \t");
			//				for (int i = 0; i < possibilityPtr; i++){
			//					printf("%d  ", allPossibilities[i]);
			//				}
			//				printf("\n");


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
								break;
							}	
				
						}

					}
					++noOfPassedEdges;					

				//	printf("%d  ", target);

					printf("route from %d to %d\n", currentNode, target);

					currentConnection->visited = 1;			
					currentConnection->otherSide->visited = 1;	 // segmentation fault				

				//	printf("\n");

					currentNode = target;
					counter++;
			//		printf("counter: %d\n", counter);
					backMeup[counter] = currentNode;

			//	A quick checkpoint to see which path is calculated

					printf("Current Path: \t");
					for (int i = 0; i <= counter; i++){
						printf("%d ",backMeup[i]);
					}
					printf("\n");
					
				}

	free(line);
	free(backMeup);
	free(allPossibilities);
	printf("done\n");
}

