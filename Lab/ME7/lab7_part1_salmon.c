/* 
Name: 		Paulino I. Salmon III
SN: 		2015-11557
Section: 	HLMTRU
References:
https://stackoverflow.com/questions/12568379/kmalloc-size-allocation
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

static int __init part1_init(void) {

	// #1
	unsigned int i;
	for(i = 0; i < 255; i++) {
		ptr = kmalloc(i, GFP_KERNEL);
		printk(KERN_INFO "I got: %zu bytes of memory. I is: %d\n", ksize(ptr), i);
	    kfree(ptr);
	}

	// #2
	printk(KERN_INFO "Minimum size is: %d bytes.\n", KMALLOC_MIN_SIZE);

	// #3
	printk(KERN_INFO "Maximum size is: %ld bytes.\n", KMALLOC_MAX_SIZE);

    return 0;	
}

static void __exit part1_cleanup(void) {
    printk(KERN_INFO "Cleaning up kmalloc() module...\n");
}

module_init(part1_init);
module_exit(part1_cleanup);

MODULE_AUTHOR("Paulino I. Salmon III");

