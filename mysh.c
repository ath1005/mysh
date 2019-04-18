/// file: mysh.c
/// description: TODO
/// author: Atlee Hasson

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void prompt(int counter){
	printf("mysh[%d]> ", counter);	
}

int main(int argc, char *argv[]){
	char *buffer;
	int count = 0;
	size_t bufsize = 256;

	buffer = malloc(bufsize * sizeof(char));
	prompt(count);
	getline(&buffer, &bufsize, stdin);

	char * command = strtok(buffer, " \n");

	while(strcmp(command, "quit")){
		printf("%s\n", buffer);

		char * token = strtok(NULL, " \n");
		while(token != NULL){
			printf("%s\n", token);
			token = strtok(NULL, " \n");
		}
		
		count++;
		prompt(count);
		getline(&buffer, &bufsize, stdin);
		command = strtok(buffer, " \n");
	}
	
	free(buffer);	
	return EXIT_SUCCESS;
}
