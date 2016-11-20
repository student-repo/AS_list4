#include<stdio.h>

int main(int argc, char **argv)
{
        setuid(0);

        int i;
        char a[50] = {0};
	sprintf(a, "sudo kill -USR1 %s",argv[1]);
        printf("%s\n",a);

        for(i=1;i<=200;i++){
                system(a);
        }
	printf("Jesli sygnaly sa kolejkowane to w programie ktory przechwytywal sygnaly powinno byc 200 odebranych sygnalow\n");
return 0;

}
