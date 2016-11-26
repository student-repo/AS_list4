#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include<signal.h>
#include <stdlib.h>

int wordsAmount(const char sentence[ ]);

void  execute(char **argv,int wait);

void myhandle(int mysignal);

char *removeEtFromCommand(char *command, int runInBackground);

void runsource(int pfd[], char **cmd1);

void rundest(int pfd[], char **cmd2);

char *removeBeginSpaces(char *str1);

int main(void)
{

  if(signal(SIGTERM, myhandle) == SIG_ERR){
    printf("\ncan't catch SIGTERM \n");
  }
  if (signal(SIGINT, myhandle) == SIG_ERR){
     printf("\ncan't catch SIGSTOP\n");
}


char *line = NULL;
size_t size;
printf("lsh shell $ ");
while (getline(&line, &size, stdin)) {

printf("lsh shell $ ");

if(feof(stdin)){
  exit(1);
}
line=strtok(line, "\n");

if (line != NULL && strpbrk(line, "|") != 0){
  char *pipeCmd1 = (char *) malloc((int)strlen(line));
  char *pipeCmd2 = (char *) malloc((int)strlen(line));
  const char pipeChar = '|';
  strcpy(pipeCmd1, line);
  strcpy(pipeCmd2, line);
  printf("%s\n", pipeCmd2 );

  pipeCmd1 = strchr(pipeCmd1, pipeChar);
memmove(pipeCmd1, pipeCmd1+1, strlen(pipeCmd1));
int a = strlen(pipeCmd2) - strlen(pipeCmd1) - 1;
pipeCmd1=removeBeginSpaces(pipeCmd1);
pipeCmd2[a]='\0';
  //
  // printf("%s\n", pipeCmd1 );
  // printf("%s\n", pipeCmd2 );



    int aa =wordsAmount(pipeCmd1);
    int ii = 0;
    char *pp = strtok (pipeCmd1, " ");
    char *command1Array[aa+1];

        while (pp != NULL){
            command1Array[ii++] = pp;
            pp = strtok (NULL, " ");
        }

        if(*command1Array != NULL && strcmp("exit",command1Array[0]) == 0 ){
          exit(1);
        }
        // int l;
        // for(l=0;l<aa;l++)
        // printf("%s\n",command1Array[l] );

        int aaa =wordsAmount(pipeCmd2);
        int iii = 0;
        char *ppp = strtok (pipeCmd2, " ");
        char *command2Array[aaa+1];

            while (ppp != NULL){
                command2Array[iii++] = ppp;
                ppp = strtok (NULL, " ");
            }

            if(*command2Array != NULL && strcmp("exit",command2Array[0]) == 0 ){
              exit(1);
            }

// printf("\n" );
//             for(l=0;l<aaa;l++)
//             printf("%s\n",command2Array[l] );
int pid, status;
int fd[2];

  pipe(fd);
  runsource(fd, command1Array);
	rundest(fd, command2Array);

  close(fd[0]); close(fd[1]);
  while ((pid = wait(&status)) != -1)	/* pick up all the dead children */
		fprintf(stderr, "process %d exits with %d\n", pid, WEXITSTATUS(status));
	// exit(0);


}
else{
  int runInBackground = etAtTheEnd(line);

  line = removeEtFromCommand(line, runInBackground);

  int a =wordsAmount(line);

  int i = 0;
  char *p = strtok (line, " ");
  char *commandArray[a+1];

      while (p != NULL){
          commandArray[i++] = p;
          p = strtok (NULL, " ");
      }

      if(*commandArray != NULL && strcmp("exit",commandArray[0]) == 0 ){
        exit(1);
      }

    execute(commandArray, runInBackground);
}

}

  return 0;
}

int wordsAmount(const char sentence[ ])
{
    int counted = 0;
    if(sentence==NULL){
      return 0;
    }

    const char* it = sentence;
    int inword = 0;

    do switch(*it) {
        case '\0':
        case ' ': case '\t': case '\n': case '\r': // TODO others?
            if (inword) { inword = 0; counted++; }
            break;
        default: inword = 1;
    } while(*it++);

    return counted;
}

void  execute(char **argv, int runInBackground)
{
     pid_t  pid;
     int    status;
     int emptyArray = strlen(argv);

     if ((pid = fork()) < 0) {     /* fork a child process           */
          printf("*** ERROR: forking child process failed\n");
          exit(1);
     }
     else if (pid == 0) {
          execvp(*argv, argv);
          perror("exec: error");
          exit(0);

     }
     else if (emptyArray != 0 && runInBackground == 0){
       waitpid(pid, NULL, 0);
     }
}

void myhandle(int mysignal){
	printf("myhandle with signal %d\n", mysignal);

}

int etAtTheEnd(const char str[]){
  if(str==NULL){
    return 0;
  }
    return (str && *str && str[strlen(str) - 1] == '&') ? 1 : 0;
}

char *removeEtFromCommand(char *command, int runInBackground){
  if(command==NULL){
    return command;
  }

  if(runInBackground == 0){
    return command;
  }
  command[strlen(command) - 1] = '\0';
  return command;


}


void
rundest(int pfd[], char **cmm2)	/* run the second part of the pipeline, cmd2 */
{
	int pid;

  // printf("%s\n",cmm2);
	switch (pid = fork()) {

	case 0: /* child */
		dup2(pfd[0], 0);	/* this end of the pipe becomes the standard input */
		close(pfd[1]);		/* this process doesn't need the other end */
		execvp(cmm2[0], cmm2);	/* run the command */
		perror(cmm2[0]);	/* it failed! */

	default: /* parent does nothing */
		break;

	case -1:
		perror("fork");
		exit(1);
	}
}


void
runsource(int pfd[], char **cmm1)	/* run the first part of the pipeline, cmd1 */
{
	int pid;	/* we don't use the process ID here, but you may wnat to print it for debugging */
// printf("%s\n",cmm1 );
	switch (pid = fork()) {

	case 0: /* child */
		dup2(pfd[1], 1);	/* this end of the pipe becomes the standard output */
		close(pfd[0]); 		/* this process don't need the other end */
		execvp(cmm1[0], cmm1);	/* run the command */
		perror(cmm1[0]);	/* it failed! */

	default: /* parent does nothing */
		break;

	case -1:
		perror("fork");
		exit(1);
	}
}

char *removeBeginSpaces(char *str1)
{
    char *str2;
    str2=str1;
    while (*str2==' ') str2++;
    if (str2!=str1) memmove(str1,str2,strlen(str2)+1);
    return str1;
}
