#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include<signal.h>

int wordsAmount(const char sentence[ ]);

void  execute(char **argv,int wait);

void myhandle(int mysignal);

char *removeEtFromCommand(char *command, int runInBackground);

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
