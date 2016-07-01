#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
using namespace std;
void* write_pid(void *value)
{
    cerr<<"get process\n";
    pid_t pid = getpid();
    cerr<<"pid: "<<pid<<"\n";
    ofstream fout;
    fout.open("/home/rita/box.pid", fstream::out);
        cerr<<"sucessful write\n";
    fout << pid;
    fout.close();
    return 0;
}

int main()
{
    pthread_t thread;

    if (pthread_create(&thread, NULL, write_pid, NULL) != 0)
    {
        cerr<<"no process\n";
        return 1;
    }

    if (pthread_join(thread, NULL) != 0)
    {
        cerr<<"no join\n";
        return 1;
    }

    return 0;
 }
