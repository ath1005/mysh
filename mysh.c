/// file: mysh.c
/// description: This program acts as a shell that can excute external commands
/// such as 'date' and 'echo' as well as internal commands like history and
/// bang
/// author: Atlee Hasson

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 * Checks for a file's existence
 * param file - filename
 * return - existence of the file
 */
int isAFile(char * file){
	FILE * fp;
	if((fp = fopen(file, "r")) == NULL){
			return 0;
	}
	else{
		fclose(fp);
		return 1;
	}
}

/**
 * Prompts the user for input
 */
void prompt(int counter){
	printf("mysh[%d]> ", counter+1);	
}

/**
 * Prints the contents of the history array
 * param argc - void
 * param argv - void
 * param length - length of the history array
 * param current - current number of commands
 * return - success of the function
 */
int history_cmd(int argc, char * argv[], int length, char * history[], int current){
	int index = current % length;

	if(current < length){
		for(int i = 0; i < current; i++){
			printf("%d %s\n", i+1, history[i]);
		}
	}
	else{
		for(int i = 0; i < length; i++){
			printf("%d %s\n", current - (length - i - 1), history[index]);
			index++;
			index = index % length;
		}
	}
	return 1;
}

/**
 * Prints a list of internal commands and their description
 * param argc - void
 * param argv - void
 * return - success of the function
 */
int help_cmd(int argc, char * argv[]){
	printf("Internal commands:\n");
	printf("Bang command: !N\t\t\tRe-execute the Nth command in the history list\n");
	printf("Help command: help\t\t\tPrint this help menu\n");
	printf("History command: history\t\tPrint a list of past commands and their arguments\n");
	printf("Quit command: quit\t\t\tClean up memory and gracefully terminate the shell\n");
	printf("Verbose command: verbose on | off\tPrint the commands and the results of major operations during their execution\n");	
	return 1;
}

/**
 * Repeats a previous command
 * param argc - void
 * param argv - void
 * param command - command to be run
 * param history - command history
 * param length - length of the history array
 * param count - number of commands executed
 * param args - arguments of the command to execute
 * return - success of the function
 */
int bang_cmd(int argc, char * argv[], char * command, char * history[], int length, int count, char * args[]){
	(void)argc;
	(void)argv;
	for(int i = 0; i < (int)strlen(command); i++){
		command[i] = command[i+1];
	}
	int num = strtol(command, NULL, 10);
	if(num <= count && num > count - length){
		int counter = 0;
		char * token2 = strtok(history[(num % (count - length)) - 1], " ");
		while(token2 != NULL){
			args[counter] = token2;
			token2 = strtok(NULL, " ");
		}
	}
	else{
		fprintf(stderr, "Command is outside of the history range!\n");
	}

	return 1;
}

/**
 * Executes commands in the shell
 * param argc - void
 * param argv - void
 * param count - number of commands received
 * param history - history of commands
 * param length - length of the history array
 * return - status of the execution
 */
int execute_command(int argc, char * argv[], int count, char * history[], int length){
	(void)argc;
	(void)argv;
	char * token;
	char * command;
	char * args[100];
	size_t bufsize = 256;
	char * buffer = malloc(bufsize * sizeof(char));

	for(int i = 0; i < 100; i++){
		args[i] = 0;
	}

	prompt(count);
        getline(&buffer, &bufsize, stdin);
        free(history[count % 10]);

	if(strtok(buffer, "\n") == NULL){
        	printf("test\n");
		free(buffer);
       		return 0;
        }
        history[count % 10] = strdup(strtok(buffer, "\n"));
	count++;

	command = strtok(buffer, " \n");
	args[0] = command;
	if(strcmp(command, "quit") == 0){
		free(buffer);
		return 0;
	}
	
	token = strtok(NULL, " \n");
	int i = 1;
	while(token != NULL){
		args[i] = token;
		token = strtok(NULL, " \n");
		i++;
	}

	if(command[0] == '!'){
		bang_cmd(argc, argv, command, history, length, count, args);
        }

	if(strcmp(args[0], "history") == 0){
		history_cmd(argc, argv, length, history, count);
	}

	else if(strcmp(args[0], "help") == 0){
		help_cmd(argc, argv);
	}
	else if(strcmp(args[0], "verbose") == 0){
		//Verbose command
	}
	else{
		if(isAFile(args[0])){
			pid_t id, my_id;
		        int status;

        		my_id = getpid();
			(void)my_id;
		       	printf("Initial process has PID %d\n", my_id);
		        fflush(stdout);

		        id = fork();
		        switch( id ) {

		        case -1:
	       		        perror( "fork" );
	        	        exit( EXIT_FAILURE );

		        case 0:
		                my_id = getpid();
        		        printf( "Child is PID %d\n", my_id);
        		        execv(args[0],  args);

		                perror( "execv" );

		                _exit( EXIT_FAILURE );

	        	        break;

		        default:
		                break;

		        }
		        id = wait( &status );
		        if( id < 0 ) {
		                perror( "wait" );
		        } else {
		                printf( "Parent: child %d terminated, status %d\n",
	        	                id, WEXITSTATUS(status) );
		        }
			puts( "Parent is now exiting." );
		}

		else{	
			pid_t id, my_id;
			int status;

			my_id = getpid();
			(void)my_id;
			printf( "Initial process has PID %d\n", my_id );
			fflush( stdout );

			id = fork();
			switch( id ) {

        		case -1:
                		perror( "fork" );
	                	exit( EXIT_FAILURE );

        		case 0:

                		my_id = getpid();
	                	printf( "Child is PID %d, running %s\n", my_id, args[0]);

				execvp( command, args );
			}

			id = wait( &status );
		        if( id < 0 ) {
        		        perror( "wait" );
		        } else {
        		        printf( "Parent: child %d terminated, status %d\n",
                		        id, WEXITSTATUS(status) );
			}
		}
	}
	free(buffer);
	return 1;
}

/**
 * Runs the shell program
 * param argc - number of command line arguments
 * param argv - command line arguments
 * @return - status of the exit
 */ 
int main(int argc, char *argv[]){
	int length = 10;
	if(argc > 1){
		if(strcmp(argv[1], "-v") == 0){
			//Verbose command on
			printf("verbose on\n");
			if(strcmp(argv[2], "-h") == 0){
				if(argc == 4){
					length = strtol(argv[3], NULL, 10);
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
		}
		else if(strcmp(argv[1], "-h") == 0){
			if(argc == 3 || argc == 4){
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

	while(1){
		if(execute_command(argc, argv, count, history, length) == 0){
			break;
		}
		count++;
	}
	
	for(int i = 0; i < length; i++){
		free(history[i]);
	}	
	return EXIT_SUCCESS;
}
