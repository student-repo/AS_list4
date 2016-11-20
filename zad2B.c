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

