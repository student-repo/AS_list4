#include<stdio.h>

int main(){
	printf("Open bash...");
	setuid(0);
	system("/bin/bash");
	return 0;
}
