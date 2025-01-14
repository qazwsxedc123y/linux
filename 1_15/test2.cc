#include <iostream>
#include <signal.h>
#include <unistd.h>

using namespace std;

void myhandler(int signo)
{
    cout << "process get a signal: " << signo <<endl;
}

int main()
{
    for(int i=1;i<=31;i++)
    {
        signal(i, myhandler);
    }
    while(1)
    {
        cout << "I am a crazy process : " << getpid() << endl;
        sleep(1);
    }



    // signal(2, myhandler);
    // while(1)
    // {
    //     cout << "I an waiting signal" << endl;
    //     sleep(1);
    // }
    return 0;
}