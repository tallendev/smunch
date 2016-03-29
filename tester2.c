#include <stdio.h>
#include <errno.h>
#include <sys/syscall.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

#define the_goob(arg) syscall(325, arg)
#define init_sc(arg) syscall(326, arg)
#define get_sc(arg) syscall(327, arg)
#define smunch(arg1, arg2) syscall(329, arg1, arg2)

int main(int argc, char** argv)
{
    int pid;
    unsigned long bits;
    if (argc != 2)
    {
        printf("No arg\n");
        return -1;
    }
    pid = atoi(argv[1]);
    bits = 1UL << (SIGKILL - 1);
    return smunch(pid, bits);
}
