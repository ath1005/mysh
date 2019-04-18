/// file: mysh.c
/// description: TODO
/// author: Atlee Hasson

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int prompt(int counter){
	printf("mysh[%d]> ", counter);
	counter++;
	return counter;	
}

int main(int argc, char *argv[]){
	char *buffer;
	size_t bufsize = 256;

	buffer = malloc(bufsize * sizeof(char));
	prompt(0);
	getline(&buffer, &bufsize, stdin);

	char * command = strtok(buffer, " \n");
	printf("%s\n", command);

	free(buffer);	
	return main(argc, argv);
}
