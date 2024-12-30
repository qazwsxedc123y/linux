#include <stdio.h>
#include <stdlib.h>

int main() {
    // 调用 Shell 脚本
    printf("Calling Shell Script...\n");
    int shell_status = system("./script.sh");
    if (shell_status == 0) {
        printf("Shell Script executed successfully.\n");
    } else {
        printf("Failed to execute Shell Script.\n");
    }

    // 调用 Python 脚本
    printf("\nCalling Python Script...\n");
    int python_status = system("python3 script.py");
    if (python_status == 0) {
        printf("Python Script executed successfully.\n");
    } else {
        printf("Failed to execute Python Script.\n");
    }

    return 0;
}