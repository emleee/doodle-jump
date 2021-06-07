#include <stdlib.h>
#include <stdio.h>

void main() {
    char *test = malloc(sizeof(char)*10);
    sprintf(test, "hello\n");
    printf(test);
}