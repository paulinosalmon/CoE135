/* 
Name: 		Paulino I. Salmon III
SN: 		2015-11557
Section: 	HLMTRU
Refereces: 	
https://github.com/torvalds/linux/blob/master/arch/nios2/kernel/cpuinfo.c
https://code.woboq.org/linux/linux/arch/x86/include/asm/processor.h.html#cpuinfo_x86
*/

#undef __KERNEL__
#define __KERNEL__

#undef MODULE
#define MODULE

#include <linux/module.h>	
#include <linux/kernel.h>	
#include <linux/cpufreq.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

extern struct cpuinfo_x86 boot_cpu_data;

static int __init show_cpuinfo(void) {

	unsigned int cpu = boot_cpu_data.cpu_index;

	printk(KERN_INFO "Initializing cpuinfo module...\n\nprocessor\t: %u\nvendor_id\t: %s\ncpu family\t: %d\nmodel\t\t: %u\nmodel name\t: %s\n",
		cpu,
		boot_cpu_data.x86_vendor_id,
		boot_cpu_data.x86,
		boot_cpu_data.x86_model,
		boot_cpu_data.x86_model_id
  	);

	return 0;

}

static void __exit cpu_cleanup(void) {
    printk(KERN_INFO "Cleaning up cpuinfo module...\n\n\n");
}

module_init(show_cpuinfo);
module_exit(cpu_cleanup);

MODULE_AUTHOR("Paulino I. Salmon III");
