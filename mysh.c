/// file: mysh.c
/// description: TODO
/// author: Atlee Hasson

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct history_s{
	int count;
	int length;
	int * numHistory;
	char * commandHistory;
};

typedef struct history_s *history;

history create_history(int count){
	history h = NULL;

	h = malloc(sizeof(struct history_s));

	h->numHistory = calloc(count, sizeof(int));

	h->commandHistory = NULL;

	h->count = count;
	h->length = 0;

	return h;
}

void destroy_history(history h){
	free(h->numHistory);
	free(h);
}

int prompt(int counter){
	printf("mysh[%d]> ", counter);
	counter++;
	return counter;	
}

int main(int argc, char *argv[]){
	char *buffer;
	size_t bufsize = 256;
	history h = create_history(10);

	buffer = malloc(bufsize * sizeof(char));
	prompt(h->length);
	getline(&buffer, &bufsize, stdin);

	char * command = strtok(buffer, " \n");

	while(strcmp(command, "quit")){
		printf("%s\n", command);
		h->length++;

		prompt(h->length);
		getline(&buffer, &bufsize, stdin);
		command = strtok(buffer, " \n");
	}
	
	destroy_history(h);
	free(buffer);	
	return EXIT_SUCCESS;
}
