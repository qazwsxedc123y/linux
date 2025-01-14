#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main()
{
    const char *mgs0 = "hello printf\n";
    const char *mgs1 = "hello fwrite\n";
    const char *mgs2 = "hello write\n";

    printf("%s",mgs0);
    fwrite(mgs1, strlen(mgs1), 1, stdout);
    write(1, mgs2, strlen(mgs2));

    fork();

    return 0;
}