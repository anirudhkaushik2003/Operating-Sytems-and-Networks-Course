#include <stdio.h>

int main()
{
    printf("abc: ");
    int a[10];
    for(int i = 0;i<9;i++)
    {
        scanf("%d %d",&a[i],&a[i+1]);
        i++;
    }
    for(int i = 0;i<10;i++)
        printf("%d\n",a[i]);

}