#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include<signal.h>
#include <stdlib.h>
#include <fcntl.h>


int wordsAmount(const char sentence[ ]);

void  execute(char **argv,int wait);

void myhandle(int mysignal);

char *removeEtFromCommand(char *command, int runInBackground);

void runsource(int pfd[], char **cmd1);

void rundest(int pfd[], char **cmd2);

char *removeBeginSpaces(char *str1);

void fillTableCommand(char *tableCommand[], char *stringCommand, int a);

void outputInputRedirection(char *line, int redirectionType, char redirectionChar, char *redirectionStr);

void handleCommonCommand(char *line);

void handlePipe(char *line);

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
  if(line == NULL){
    ;
  }
  else if (line != NULL && strpbrk(line, "|") != 0){
    handlePipe(line);
  }
  else if (line != NULL && strstr(line, "2>") != NULL){
    outputInputRedirection(line, 2, ' ', "2>");
  }
  else if (line != NULL && strpbrk(line, ">") != 0){
    outputInputRedirection(line, 1, '>', "");
  }
  else if (line != NULL && strpbrk(line, "<") != 0){
    outputInputRedirection(line, 0, '<', "");
  }
  else{
    handleCommonCommand(line);
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
  pid_t  pid;
  int    status;

  if ((pid = fork()) < 0) {     /* fork a child process           */
       printf("*** ERROR: forking child process failed\n");
       exit(1);
  }
  else if (pid == 0) {
       dup2(pfd[0], 0);	/* this end of the pipe becomes the standard input */
   		close(pfd[1]);		/* this process doesn't need the other end */
   		execvp(cmm2[0], cmm2);	/* run the command */
   		perror(cmm2[0]);/* it failed! */
      waitpid(pid, NULL, 0);
       exit(0);
  }
}

void
runsource(int pfd[], char **cmm1)	/* run the first part of the pipeline, cmd1 */
{
  pid_t  pid;
  int    status;

  if ((pid = fork()) < 0) {     /* fork a child process           */
       printf("*** ERROR: forking child process failed\n");
       exit(1);
  }
  else if (pid == 0) {
    dup2(pfd[1], 1);	/* this end of the pipe becomes the standard output */
   close(pfd[0]); 		/* this process don't need the other end */
   execvp(cmm1[0], cmm1);	/* run the command */
   perror(cmm1[0]);	/* it failed! */
   waitpid(pid, NULL, 0);
       exit(0);
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

void fillTableCommand(char *tableCommand[], char *stringCommand, int a){
  int i = 0;
  char *p = strtok (stringCommand, " ");


      while (p != NULL){
          tableCommand[i++] = p;
          p = strtok (NULL, " ");
      }
      tableCommand[a]=NULL;
    }
void outputInputRedirection(char *line, int redirectionType, char redirectionChar, char *redirectionStr){
  char *secCmdElem = (char *) malloc((int)strlen(line));
  char *firstCmdElem = (char *) malloc((int)strlen(line));
  int saved_stdout;
  strcpy(secCmdElem, line);
  strcpy(firstCmdElem, line);
  saved_stdout = dup(1);
  int a;

if(redirectionType == 2){
  secCmdElem = strstr(secCmdElem, redirectionStr);
  memmove(secCmdElem, secCmdElem+2, strlen(secCmdElem));
  a = strlen(firstCmdElem) - strlen(secCmdElem) - 2;
}
else{
  secCmdElem = strchr(secCmdElem, redirectionChar);
  memmove(secCmdElem, secCmdElem+1, strlen(secCmdElem));
  a = strlen(firstCmdElem) - strlen(secCmdElem) - 1;
}
  secCmdElem=removeBeginSpaces(secCmdElem);
  firstCmdElem[a]='\0';

  int n =wordsAmount(firstCmdElem);
  char *command3Array[n+1];
  fillTableCommand(command3Array, firstCmdElem, n);

  int newfd;	/* new file descriptor */
  if(redirectionType == 0){
    newfd = open(secCmdElem, O_RDONLY);
  }
  else{
    if ((newfd = open(secCmdElem, O_CREAT|O_TRUNC|O_WRONLY, 0644)) < 0) {
  		perror(secCmdElem);	/* open failed */
  		exit(1);
  	}
  }

  dup2(newfd, redirectionType);

  pid_t  pidd;

  if ((pidd = fork()) < 0) {     /* fork a child process           */
       printf("*** ERROR: forking child process failed\n");
       exit(1);
  }
  else if (pidd == 0) {
       execvp(*command3Array, command3Array);
       perror("exec: error");
       exit(0);
  }
  waitpid(pidd, NULL, 0);
  dup2(saved_stdout, redirectionType);
close(saved_stdout);
}

void handleCommonCommand(char *line){
  int runInBackground = etAtTheEnd(line);

  line = removeEtFromCommand(line, runInBackground);

  int a =wordsAmount(line);

  char *commandArray[a+1];
  fillTableCommand(commandArray, line, a);

      if(*commandArray != NULL && strcmp("exit",commandArray[0]) == 0 ){
        exit(1);
      }

    execute(commandArray, runInBackground);
}

void handlePipe(char *line){
  char *pipeCmd1 = (char *) malloc((int)strlen(line));
  char *pipeCmd2 = (char *) malloc((int)strlen(line));
  const char pipeChar = '|';
  strcpy(pipeCmd1, line);
  strcpy(pipeCmd2, line);

  pipeCmd1 = strchr(pipeCmd1, pipeChar);
memmove(pipeCmd1, pipeCmd1+1, strlen(pipeCmd1));
int a = strlen(pipeCmd2) - strlen(pipeCmd1) - 1;
pipeCmd1=removeBeginSpaces(pipeCmd1);
pipeCmd2[a]='\0';

    int aa =wordsAmount(pipeCmd1);
    char *commandArray[aa+1];
    fillTableCommand(commandArray, pipeCmd1, aa);


        int aaa =wordsAmount(pipeCmd2);

        char *command2Array[aaa+1];
        fillTableCommand(command2Array, pipeCmd2, aaa);

int pid, status;
int fd[2];

  pipe(fd);

  runsource(fd, command2Array);
  rundest(fd, commandArray);

  close(fd[0]); close(fd[1]);

}
