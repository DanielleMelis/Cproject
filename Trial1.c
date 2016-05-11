// read graph from stream

#include<stdio.h>
#include<stdlib.h>


struct Node{
	int id;
	struct List* edge;
};

struct List{
	struct List* next;
	struct Node* node;
};



int main(int argc, char* argv[]){

	if (argc != 2){
		printf("Invalid input");
		exit(1);
	}

	FILE *file = fopen(argv[1], "r");
	char *line = NULL;
	size_t n = 0;

	while (getline(&line, &n, file) > 0){
		printf("%s", line);
		
		free(line);
		line = NULL;
	}

	// may free line again
	free(line);

	printf("done\n");

}