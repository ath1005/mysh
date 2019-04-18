/// file: mysh.c
/// description: TODO
/// author: Atlee Hasson

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
	char *buffer;
	size_t bufsize = 256;

	buffer = malloc(bufsize * sizeof(char));
	//prompt
	getline(&buffer, &bufsize, stdin);

	char * command = strtok(buffer, " \n");
	printf("%s\n", command);
	
	return main(argc, argv);
}
