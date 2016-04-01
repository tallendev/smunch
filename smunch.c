#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/sched.h>

SYSCALL_DEFINE2(smunch, int, pid, unsigned long, bit_pattern)
{
    unsigned long flags;
    struct task_struct* p;
    rcu_read_lock();
    p = pid_task(find_vpid(pid), PIDTYPE_PID);
    rcu_read_unlock();
    if (p && !p->ptrace && thread_group_empty(p) && 
             lock_task_sighand(p, &flags))
    {
        if (bit_pattern & sigmask(SIGKILL))
        {
            if (p->exit_state == EXIT_ZOMBIE)
            {
                unlock_task_sighand(p, &flags);
                release_task(p);
                return 0;
            }
            printk(KERN_ALERT "STUFFS HAPPENIN\'");
            p->signal->shared_pending.signal.sig[0] |= sigmask(SIGKILL);
            unlock_task_sighand(p, &flags);
            wake_up_process(p);
            printk(KERN_ALERT "wakeup");
        }
        else
        {
            printk(KERN_ALERT "maybe?");
            p->signal->shared_pending.signal.sig[0] |= bit_pattern;
            unlock_task_sighand(p, &flags);
            signal_wake_up(p, 0); 
        }
    }
    else
    {
        return -1;
    }
    return 0;
}
