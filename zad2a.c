#include<stdio.h>
#include<signal.h>

void myhandle(int mysignal){
	printf("myhandle with signal %d\n", mysignal);

}

int main(int argc, char *argv[]){
	
	int i = 0;

	if(signal(SIGTERM, myhandle) == SIG_ERR){
		printf("\ncan't catch SIGTERM \n");
	}

	if (signal(SIGUSR1, myhandle) == SIG_ERR){
        printf("\ncan't catch SIGUSR1\n");
	}

   	 if (signal(SIGKILL, myhandle) == SIG_ERR){
        printf("\ncan't catch SIGKILL\n");
	}

   	 if (signal(SIGSTOP, myhandle) == SIG_ERR){
        printf("\ncan't catch SIGSTOP\n");
	}

	while(1){
		printf("%d\n",i);
		i++;
		sleep(1);
	}
	
	return 0;
}
