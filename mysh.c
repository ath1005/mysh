/// file: mysh.c
/// description: TODO
/// author: Atlee Hasson

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void prompt(int counter){
	printf("mysh[%d]> ", counter);	
}

int history_cmd(int argc, char * argv[], int length, char * history[], int current){
	int index = current % length;

	if(current < length){
		for(int i = 0; i < current; i++){
			printf("%d %s\n", i, history[i]);
		}
	}
	else{
		for(int i = 0; i < length; i++){
			printf("%d %s\n", current - (length - i), history[index]);
			index++;
			index = index % length;
		}
	}
}

int help_cmd(int argc, char * argv[]){
	printf("Internal commands:\n");
	printf("Bang command: !N\t\t\tRe-execute the Nth command in the hisroty list\n");
	printf("Help command: help\t\t\tPrint this help menu\n");
	printf("History command: history\t\tPrint a list of past commands and their arguments\n");
	printf("Quit command: quit\t\t\tClean up memory and gracefully terminate the shell\n");
	printf("Verbose command: verbose on | off\tPrint the commands and the results of major operations during their execution\n");	
}

int bang_cmd(int argc, char * argv[], int command, char * history[]){
	char * target = history[command];
}

int execute_command(int argc, char * argv[], int count, char * history[], int length){
	char * token;
	char * command;
	size_t bufsize = 256;
	char * buffer = malloc(bufsize * sizeof(char));

	prompt(count);
        getline(&buffer, &bufsize, stdin);
        free(history[count % 10]);

	if(strtok(buffer, "\n") == NULL){
        	printf("test\n");
       		return 1;
        }
        history[count % 10] = strdup(strtok(buffer, "\n"));
	count++;

	command = strtok(buffer, " \n");
	if(strcmp(command, "quit") == 0){
		return 1;
	}

	while(token != NULL){
		token = strtok(NULL, " \n");
	}

	if(strcmp(command, "history") == 0){
		history_cmd(argc, argv, length, history, count);
	}
	else if(command[0] == '!'){
		for(int i = 0; i < strlen(command); i++){
			command[i] = command[i+1];
		}
		int num = strtol(command, NULL, 10);
		printf("%d\n", num);
	}
	else if(strcmp(command, "help") == 0){
		help_cmd(argc, argv);
	}
	else if(strcmp(command, "verbose") == 0){
		//Verbose command
	}
	free(buffer);
}

int main(int argc, char *argv[]){
	char *buffer;
	int length = 10;
	if(argc > 1){
		if(strcmp(argv[1], "-v") == 0){
			//Verbose command on
			printf("verbose on\n");
		}
		else if(strcmp(argv[1], "-h") == 0){
			if(argc == 3){
				length = strtol(argv[2], NULL, 10);
				if(length <= 0){
					fprintf(stderr, "usage: mysh [-v] [-h pos_num]\n");
					return EXIT_FAILURE;
				}
			}
			else{
				fprintf(stderr, "usage: mysh [-v] [-h pos_num]\n");
				return EXIT_FAILURE;
			}	
		}
		else{
			fprintf(stderr, "usage: mysh [-v] [-h pos_num]\n");
			return EXIT_FAILURE;
		}
	}
	
	char * history[length];

	for(int i = 0; i < length; i++){
		history[i] = 0;
	}

	int count = 0;
	//size_t bufsize = 256;

	//buffer = malloc(bufsize * sizeof(char));

	//char * command;
	//char * token;

	while(1){
		if(execute_command(argc, argv, count, history, length) == 1){
			break;
		}
		count++;
		/**
		prompt(count);
		getline(&buffer, &bufsize, stdin);
		free(history[count % 10]);
		
		if(strtok(buffer, "\n") == NULL){
			printf("test\n");
			break;
		}
		history[count % 10] = strdup(strtok(buffer, "\n"));
		count++;

		command = strtok(buffer, " \n");
		if(strcmp(command, "quit") == 0){
			break;
		}

		while(token != NULL){
			token = strtok(NULL, " \n");
		}
		
		if(strcmp(command, "history") == 0){
			history_cmd(argc, argv, length, history, count);
		}
		else if(command[0] == '!'){
			for(int i = 0; i < strlen(command); i++){
				command[i] = command[i+1];
			}
			int num = strtol(command, NULL, 10);
			printf("%d\n", num);
		}
		else if(strcmp(command, "help") == 0){
			help_cmd(argc, argv);
		}
		else if(strcmp(command, "verbose") == 0){
			//Verbose command
		}
		*/
	}

	//free(buffer);	
	return EXIT_SUCCESS;
}
