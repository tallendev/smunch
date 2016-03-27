#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/sched.h>

SYSCALL_DEFINE2(smunch, int, pid, unsigned long, bit_pattern)
{
    struct task_struct* p;
    
    rcu_read_lock();
    p = pid_task(find_vpid(pid), PIDTYPE_PID);
    if (p && !p->ptrace)
    {
        // check sigaddset if issue here
        p->signal->shared_pending.signal.sig[0] |= bit_pattern;
        p->signal->shared_pending.signal.sig[1] |= bit_pattern >> 32;
    }
    rcu_read_unlock();

    return 0;
}
