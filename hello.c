#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>

#include <linux/errno.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>

MODULE_AUTHOR("Roman Lytvynenko <lytvynenko.roman@lll.kpi.ua>");
MODULE_DESCRIPTION("Hello, world! module");
MODULE_LICENSE("Dual BSD/GPL");

// II.A
static unsigned int counter = 1;
// II.D
module_param(counter, uint, 0644);
MODULE_PARM_DESC(counter, "How many times print 'Hello, world!'");

// III
struct l_struct {
	struct list_head list;
	ktime_t timestamp;
};

// IV
static LIST_HEAD(l_struct_list);

static int __init hello_init(void)
{
	// II.C
	if (counter > 10) {
		pr_err("Parameter is too large, make it <= 10\n");
		return -EINVAL;
	}

	// II.B
	if (counter == 0 || (counter >= 5 && counter <= 10))
		pr_warn("Parameter has a non-standard value. Continuing...\n");

	int i;

	for (i = 0; i < counter; i++) {
		// V
		struct l_struct *l_struct_ex = kmalloc(sizeof(*l_struct_ex), GFP_KERNEL);

		l_struct_ex->timestamp = ktime_get();
		list_add(&l_struct_ex->list, &l_struct_list);
		printk(KERN_EMERG "Hello, world!\n");
	}

	return 0;
}

// VI
static void __exit hello_exit(void)
{
	struct l_struct *l_struct_ex, *temp;

	list_for_each_entry_safe(l_struct_ex, temp, &l_struct_list, list) {
		pr_info("Time stamp: %lld\n", l_struct_ex->timestamp);
		list_del(&l_struct_ex->list);
		kfree(l_struct_ex);
	}
}

module_init(hello_init);
module_exit(hello_exit);
