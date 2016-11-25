#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include<signal.h>

int wordsAmount(const char sentence[ ])
{
    int counted = 0; // result
    if(sentence==NULL){
      return 0;
    }

    // state:
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


void  execute(char **argv)
{
     pid_t  pid;
     int    status;

     if ((pid = fork()) < 0) {     /* fork a child process           */
          printf("*** ERROR: forking child process failed\n");
          exit(1);
     }
     else if (pid == 0) {
          execvp(*argv, argv);
          perror("exec: error");

     }
}

void myhandle(int mysignal){
	printf("myhandle with signal %d\n", mysignal);

}

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
	// printf("%s",line);
//	printf("%s",line[0]);
// if(line==EOF){
//   exit(1);
// }


line=strtok(line, "\n");


	int a =wordsAmount(line);

    int i = 0;
    char *p = strtok (line, " ");
    char *array[a+1];


    while (p != NULL)
    {
        array[i++] = p;
        p = strtok (NULL, " ");
    }

    if(*array != NULL && strcmp("exit",array[0]) == 0 ){
      exit(1);
    }

    // for (i = 0; i < a; ++i)
    //        printf("%s\n", array[i]);

	char *aa[] = {"ls", NULL};
  execute(array);

  printf("lsh shell $ ");

}

  return 0;
}
