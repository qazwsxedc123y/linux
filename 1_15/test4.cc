#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

// 异常产生信号

void myhandler(int signo)
{
    cout << "process get a signal: " << signo <<endl;
    sleep(1);
}

int main()
{

    // signal(11, myhandler);
    // int *p = NULL;
    // *p = 100;

    // while(1){
    //     cout<<"1"<<endl;
    //     sleep(1);
    // }


    //signal(2, SIG_DFL);

    // while(1)
    // {
    //     cout << "hello signal" << endl;
    //     sleep(1);
    // }

    // int a = 10;
    // int b = 0;

    // a /= b;
    return 0;
}