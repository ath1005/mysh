/// file: mysh.c
/// description: TODO
/// author: Atlee Hasson

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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

void prompt(int counter){
	printf("mysh[%d]> ", counter+1);	
}

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
}

int help_cmd(int argc, char * argv[]){
	printf("Internal commands:\n");
	printf("Bang command: !N\t\t\tRe-execute the Nth command in the history list\n");
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
       		return 1;
        }
        history[count % 10] = strdup(strtok(buffer, "\n"));
	count++;

	command = strtok(buffer, " \n");
	args[0] = command;
	if(strcmp(command, "quit") == 0){
		free(buffer);
		return 1;
	}
	
	token = strtok(NULL, " \n");
	int i = 1;
	while(token != NULL){
		args[i] = token;
		token = strtok(NULL, " \n");
		i++;
	}

	if(command[0] == '!'){
                for(int i = 0; i < strlen(command); i++){
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

		        // start by having the original process report its identity

        		my_id = getpid();   // ask OS for our PID
		       	printf( "Initial process has PID %d\n", my_id );
		        fflush( stdout );

		        // create the child process

		        id = fork();
		        switch( id ) {

		        case -1: // the fork() failed
	       		        perror( "fork" );
	        	        exit( EXIT_FAILURE );

		        case 0: // we are the child process

        		        // report our identity
		                my_id = getpid();
        		        printf( "Child is PID %d\n", my_id);
        		        execv(args[0],  args);

		                perror( "execv" );

		                _exit( EXIT_FAILURE );

	        	        break;

		        default: // we are the parent
		                break;

		        }

		        // parent will wait for child to exit
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
		        char *argv[4];
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
}

int main(int argc, char *argv[]){
	char *buffer;
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
		if(execute_command(argc, argv, count, history, length) == 1){
			break;
		}
		count++;
	}	
	return EXIT_SUCCESS;
}
