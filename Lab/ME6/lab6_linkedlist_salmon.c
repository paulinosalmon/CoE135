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
#include <linux/slab.h>

int counter = 0;

const char * array[] = {
	"Kim Chaewon", 
	"Kwon Eunbi", 
	"Kim Minju", 
	"Ahn Yujin", 
	"Jo Yuri"
};

struct birthday {
	int day;
	int month;
	int year;
	struct list_head list;
};

struct birthday *ptr, *next, *kim_chaewon, *kwon_eunbi, *kim_minju, *ahn_yujin, *jo_yuri;

static LIST_HEAD(birthday_list);

static int __init linked_list_init(void) {

	printk(KERN_INFO "Initializing linked list module...\n");

	kim_chaewon = kmalloc(sizeof(*kim_chaewon), GFP_KERNEL);
	kwon_eunbi = kmalloc(sizeof(*kwon_eunbi), GFP_KERNEL);
	kim_minju = kmalloc(sizeof(*kim_minju), GFP_KERNEL);
	ahn_yujin = kmalloc(sizeof(*ahn_yujin), GFP_KERNEL);
	jo_yuri = kmalloc(sizeof(*jo_yuri), GFP_KERNEL);

	// Chaewon
	kim_chaewon->day = 1;
	kim_chaewon->month= 8;
	kim_chaewon->year = 2000;

	// Eunbi
	kwon_eunbi->day = 27;
	kwon_eunbi->month= 9;
	kwon_eunbi->year = 1995;

	// Minju
	kim_minju->day = 5;
	kim_minju->month= 2;
	kim_minju->year = 2001;

	// Yujin
	ahn_yujin->day = 1;
	ahn_yujin->month= 9;
	ahn_yujin->year = 2003;

	// Yuri
	jo_yuri->day = 22;
	jo_yuri->month= 10;
	jo_yuri->year = 2001;

	INIT_LIST_HEAD(&kim_chaewon->list);
	list_add_tail(&kim_chaewon->list, &birthday_list);
	list_add_tail(&kwon_eunbi->list, &birthday_list);
	list_add_tail(&kim_minju->list, &birthday_list);
	list_add_tail(&ahn_yujin->list, &birthday_list);
	list_add_tail(&jo_yuri->list, &birthday_list);

	list_for_each_entry_safe(ptr, next, &birthday_list, list) {
    	printk(KERN_INFO "%s: %d/%d/%d\n", array[counter], ptr->month, ptr->day, ptr->year);
    	counter++;
	}
	
    return 0;	
}

static void __exit linked_list_cleanup(void) {
    printk(KERN_INFO "Cleaning up linked list module...\n\n\n");

    list_for_each_entry_safe(ptr, next, &birthday_list, list) {
		list_del(&ptr->list);
		kfree(ptr);
	}
}

module_init(linked_list_init);
module_exit(linked_list_cleanup);

MODULE_AUTHOR("Paulino I. Salmon III");
