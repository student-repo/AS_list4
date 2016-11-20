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

