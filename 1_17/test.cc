#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

int main()
{
    pid_t id = fork();
    if(id == 0)
    {
        //child
        int cnt = 500;
        while(cnt)
        {
            cout << "i am a child process, pid: " << getpid() << "cnt: " << cnt << endl;
            sleep(1);
            cnt--;
        }

        exit(0);
    }

    // father
    int status = 0;
    pid_t rid = waitpid(id, &status, 0);
    if(rid == id)
    {
        cout << "child quit info, rid: " << rid << " exit code: " << 
            ((status>>8)&0xFF) << " exit signal: " << (status&0x7F) <<
                " core dump: " << ((status>>7)&1) << endl; // ? & (0000 0000 ... 0001)
    }
    return 0;
}