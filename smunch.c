#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/sched.h>

SYSCALL_DEFINE2(smunch, int, pid, unsigned long, bit_pattern)
{
    struct task_struct* p;
    
    rcu_read_lock();
    p = pid_task(find_vpid(pid), PIDTYPE_PID);
    rcu_read_unlock();
    if (p && !p->ptrace && thread_group_empty(p))
    {
        // check sigaddset if issue here
        p->signal->shared_pending.signal.sig[0] |= bit_pattern;
        p->signal->shared_pending.signal.sig[1] |= bit_pattern >> 32;
	// wake up if sigkill 
	signal_wake_up(p, sigmask(sigkill) & bit_pattern); 
    }

    return 0;
}
