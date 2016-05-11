// read graph from stream

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

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

	int firstOddNodeOrLastNodeIdx = 0;
	while(nodes[firstOddNodeOrLastNodeIdx].degree % 2 != 1 && firstOddNodeOrLastNodeIdx < noOfVertices) {
		firstOddNodeOrLastNodeIdx++;
	}


	printf("node 1 first edge: %d \n", nodes[1].edge->targetNodeIdx);

	int currentNode = firstOddNodeOrLastNodeIdx;
	int noOfPassedEdges = 0;
	while(noOfPassedEdges < noOfEdges) {
		noOfPassedEdges++;
		int target = -1;
		struct EdgeEnd *targetEdgeEnd = nodes[currentNode].edge;
		printf("DEBUG %d\n", targetEdgeEnd->targetNodeIdx);
		printf("DEBUG %d\n", targetEdgeEnd->next->targetNodeIdx);
		printf("DEBUG %d\n", targetEdgeEnd->next->next->targetNodeIdx);

		while(target == -1) {
			printf("target %d is visited %d\n", targetEdgeEnd->targetNodeIdx, targetEdgeEnd->visited);


			if(!targetEdgeEnd->visited) {
				target = targetEdgeEnd->targetNodeIdx;
			} else {
				targetEdgeEnd=targetEdgeEnd->next;
			}
		}
		printf("mode from %d to %d\n", currentNode, target);

		nodes[currentNode].edge->visited = 1;
		nodes[currentNode].edge->otherSide->visited = 1;

		currentNode = target;
	}


	// may free line again
	free(line);

	printf("done\n");

}