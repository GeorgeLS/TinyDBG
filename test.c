#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    FILE *fp = fopen("test.txt", "w");
    fprintf(fp, "Hello World!\n");
    fclose(fp);
    return 0;
}