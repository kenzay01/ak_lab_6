
/* hello2.c */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/bug.h> 
#include "hello1.h"
MODULE_AUTHOR("Moroz Daniel <imndaya946@gmail.com>");
MODULE_DESCRIPTION("Module Hello1");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int hello_count = 1;
module_param(hello_count, uint, 0444);
MODULE_PARM_DESC(hello_count, "Number of times to print 'Hello, world!'");

static int __init hello2_init(void)
{
	if (hello_count == 0 || (hello_count >= 5 && hello_count <= 10))
		pr_warn("Warning: hello_count = %d\n", hello_count);

	BUG_ON(hello_count > 10);

	print_hello(hello_count);

	pr_info("hello2 module loaded\n");
	return 0;
}

static void __exit hello2_exit(void)
{
	pr_info("hello2 module unloaded\n");
}

module_init(hello2_init);
module_exit(hello2_exit);
