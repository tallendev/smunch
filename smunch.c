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
    printk(KERN_ALERT "pid_task yo");
    if (p && !p->ptrace && thread_group_empty(p))
    {
    	printk(KERN_ALERT "p && !p->ptrace && thread_group_empty(p)"); 
        // check sigaddset if issue here
        if (p->exit_state == EXIT_ZOMBIE)
	{
            if (bit_pattern & sigmask(sigkill))
	    {
	        p->exit_state = EXIT_DEAD; 
	    }
	    else
	    {
	        return 0;
            }
	}
	p->signal->shared_pending.signal.sig[0] |= bit_pattern;
	signal_wake_up(p, sigmask(sigkill) & bit_pattern); 
    }
    else
    {
    	printk(KERN_ALERT "p && !p->ptrace && thread_group_empty(p)"); 
        return -1;
    }
    printk(KERN_ALERT "done..."); 

    return 0;
}
