#include<stdio.h>
#include<signal.h>

int s=1;

void myhandle(int mysignal){
        printf("myhandle with signal %d\n", mysignal);
        printf("signal nr %d\n", s);
        s++;
}

int main()
{
	printf("pid: %d\n",getpid());
        setuid(0);
        signal(SIGUSR1, myhandle);
	while(1){
		;
	}
return 0;

}
