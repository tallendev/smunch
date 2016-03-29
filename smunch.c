#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/sched.h>

SYSCALL_DEFINE2(smunch, int, pid, unsigned long, bit_pattern)
{
    int wstatus;
    struct task_struct* p;
    bool sigkill = bit_pattern & sigmask(SIGKILL);
    if (sigmask(SIGKILL) & bit_pattern)
    {
        printk(KERN_ALERT "SIGKILL RECV");
    }
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
            if (sigkill)
	    {
	    	sys_waitpid(pid, &wstatus, 0);
	        //p->exit_state = EXIT_DEAD;
	    }
	    else
	    {
	        return 0;
            }
	}
	if (sigkill && p->state == TASK_UNINTERRUPTIBLE)
	{
	}
	p->signal->shared_pending.signal.sig[0] |= bit_pattern;
	signal_wake_up(p, sigkill); 
    }
    else
    {
    	printk(KERN_ALERT "p && !p->ptrace && thread_group_empty(p)"); 
        return -1;
    }
    printk(KERN_ALERT "done..."); 

    return 0;
}
