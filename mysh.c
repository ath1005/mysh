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

	h->cmdHistory = calloc(length, sizeof(char*));

	for(int i = 0; i < length; i++){
		h->cmdHistory[i] = malloc(256 * sizeof(char));
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

int history_cmd(int argc, char * argv[], char *history[10], int current){
	int index = current % 10;
	for(int i = 0; i < 10; i++){
		printf("%d %s\n", current - (10 - i), history[index]);
		index++;
		index = index % 10;
	}
}

int main(int argc, char *argv[]){
	char *buffer;
	char *history[10];
	for(int i = 0; i < 10; i++){
		history[i] = 0;
	}

	int count = 0;
	//history h = create_history(10);
	size_t bufsize = 256;

	buffer = malloc(bufsize * sizeof(char));

	//h->cmdHistory[h->total] = strtok(buffer, "\n");

	char * command;
	char * token;

	while(1){
		prompt(count);
		getline(&buffer, &bufsize, stdin);
		free(history[count % 10]);

		history[count % 10] = strdup(strtok(buffer, "\n"));
		count++;

		command = strtok(buffer, " \n"); 
		if(strcmp(command, "quit") == 0){
			break;
		}

		while(token != NULL){
			token = strtok(NULL, " \n");
			printf("%s\n", token);
		}
	}

	history_cmd(argc, argv, history, count);
	
	//destroy_history(h);
	free(buffer);	
	return EXIT_SUCCESS;
}
