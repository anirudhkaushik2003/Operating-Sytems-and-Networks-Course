#include "echo.h"

void echo_out(token_mat input)
{
    int i = 1;
    while (input.args[i] != NULL)
    {
        printf("%s ", input.args[i]);
        i++;
    }
    printf("\n");
}