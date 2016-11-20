List 4

Task 1

$ cat > zad1.c
#include<stdio.h>

int main(){
	printf("Open bash...");
	setuid(0);
	system("sudo /bin/bash");
	return 0;
}

$ gcc zad1.c -o zad1

$ sudo chown root:root zad1

$ sudo chmod 4755 zad1

$ sudo chmod +s zad1

$ ./zad1

//////////////////////////////////////////

how test? -> 
sudo chown root:root /path/to/application
sudo chmod 700 /path/to/application

set only root rigth to a file and try to modify them

////////////////////////////////////////

Task 2

a)

$ cat > zad2a.c
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

$  gcc zad2.c -o zad2

$ ./zad2

And in another terminal

$ ps -aux | grep zad2

$ kill -USR1 pid_of_zad2

$ kill -SIGTERM pid_of_zad2

$ kill -SIGKILL pid_of_zad2    or    $ kill -9 pid_of_zad2

or 

$ kill -SIGSTOP pid_of_zad2


We see that in the above code, we have tried to handle a user defined signal USR1. Also, as we know that two signals KILL and STOP cannot be handled.


Task 2B

System ignored commands 

$ sudo kill -SIGKILL 1

and

$ sudo kill -SIGTERM 1

The only signals that can be sent to process ID 1, the init process, are those for which init has explicitly installed signal handlers. This is done to assure the system is not brought down accidentally.


Task 2B

 if you want get all sinals do

$ /bin/bash

and

$ kill -l

$ ps -eo pid,cmd,start,etime

$ cat > zad2B.c
#include<stdio.h>

int main()
{
	setuid(0);
	int i;
	char a[50] = {0};
	for(i=1;i<=62;i++){
		sprintf(a, "sudo kill -%d 1",i);
		printf("%s\n",a);
		system(a);
	}
return 0;

}

$ gcc zad2B.c -o zad2B
 
$ sudo chown root:root zad2B

$ sudo chmod 4755 zad2B

$ sudo chmod +s zad2B
 
$ ./zad2B

and run this to check if something happen (nothing)

$ ps -eo pid,cmd,start,etime
