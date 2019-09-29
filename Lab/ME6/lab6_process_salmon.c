/* 
Name: 		Paulino I. Salmon III
SN: 		2015-11557
Section: 	HLMTRU
*/

#undef __KERNEL__
#define __KERNEL__

#undef MODULE
#define MODULE

#include <linux/module.h>	
#include <linux/kernel.h>	
#include <linux/init.h>		
#include <linux/list.h>
#include <linux/types.h>
#include <linux/sched/signal.h>

struct task_struct *ps_list;
int counter = 0;

static int __init process_init(void) {

	printk(KERN_INFO "Initializing process module...\n");

	for_each_process(ps_list) {
		printk(KERN_INFO "%s [%d]", ps_list->comm, ps_list->pid);
		counter++;
	}

	printk(KERN_INFO "Number of processes: %d\n", counter);

	return 0;
}

static void __exit process_cleanup(void) {
    printk(KERN_INFO "Cleaning up process module...\n\n\n");
}

module_init(process_init);
module_exit(process_cleanup);

MODULE_AUTHOR("Paulino I. Salmon III");
