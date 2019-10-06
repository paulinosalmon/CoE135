/* 
Name: 		Paulino I. Salmon III
SN: 		2015-11557
Section: 	HLMTRU
References:
https://stackoverflow.com/questions/12568379/kmalloc-size-allocation
http://www.tutorialsdaddy.com/linux-device-drivers/kmalloc-vs-vmalloc/
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
#include <linux/slab.h>	

void *ptr;

static int __init part2_init(void) {

	// #1
	printk(KERN_INFO "Page size is: %ld bytes.\n", PAGE_SIZE);

	ptr = (void *)vmalloc(1024); //in the kernel segment

	if(ptr == (void *) 0) {
		printk(KERN_INFO "Memory allocation failed.\n");
		return 0;
	}

	else
		printk(KERN_INFO "Memory allocated successfully.\n");

	return 0;
}

static void __exit part2_cleanup(void) {
	vfree(ptr);
	printk(KERN_INFO "Memory freed.\n");
    printk(KERN_INFO "Cleaning up vmalloc() module...\n");
}

module_init(part2_init);
module_exit(part2_cleanup);

MODULE_AUTHOR("Paulino I. Salmon III");

