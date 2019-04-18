/// file: mysh.c
/// description: TODO
/// author: Atlee Hasson

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct history_s{
	int total;
	char ** cmdHistory;
	int length;
};

typedef struct history_s *history;

history create_history(int length){
	history h = NULL;

	h = malloc(sizeof(struct history_s));

	h->cmdHistory = malloc(length * sizeof(char*));

	for(int i = 0; i < length; i++){
		h->cmdHistory[i] = malloc(256 * sizeof(char*));
	}

	h->length = length;

	h->total = 0;

	return h;
}

void destroy_history(history h){
	for(int i = 0; i < h->length; i++){
		free(h->cmdHistory[i]);
	}
	
	free(h->cmdHistory);
	free(h);
}

void prompt(int counter){
	printf("mysh[%d]> ", counter);	
}

int main(int argc, char *argv[]){
	char *buffer;
	history h = create_history(10);
	size_t bufsize = 256;

	buffer = malloc(bufsize * sizeof(char));
	prompt(h->total);
	getline(&buffer, &bufsize, stdin);

	h->cmdHistory[h->total] = strtok(buffer, "\n");

	printf("%s\n", h->cmdHistory[0]);

	char * command = strtok(buffer, " \n");

	while(strcmp(command, "quit")){
		printf("%s\n", buffer);

		char * token = strtok(NULL, " \n");
		while(token != NULL){
			printf("%s\n", token);
			token = strtok(NULL, " \n");
		}
		
		h->total++;
		prompt(h->total);
		getline(&buffer, &bufsize, stdin);
		h->cmdHistory[h->total] = strtok(buffer, "\n");
		command = strtok(buffer, " \n");
	}
	
	destroy_history(h);
	free(buffer);	
	return EXIT_SUCCESS;
}
