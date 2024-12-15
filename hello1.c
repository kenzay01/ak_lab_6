/* hello1.c */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include <linux/list.h>
#include "hello1.h"

MODULE_AUTHOR("Moroz Daniel <imndaya946@gmail.com>");
MODULE_DESCRIPTION("Module Hello1");
MODULE_LICENSE("Dual BSD/GPL");

struct hello_data {
	struct list_head list;
	ktime_t before;
	ktime_t after;
};

static LIST_HEAD(hello_list);
static unsigned int call_count = 0;

void print_hello(unsigned int n)
{
	unsigned int i;
	struct hello_data *data;

	for (i = 0; i < n; i++) {
		call_count++;

		if (call_count == 5) {
			data = NULL;
		} else {
			data = kmalloc(sizeof(*data), GFP_KERNEL);
		}

		if (!data) {
			pr_err("Memory allocation failed at call %u\n", call_count);
			WARN_ON(!data);
			continue;
		}

		data->before = ktime_get();
		pr_info("Hello, world!\n");
		data->after = ktime_get();

		list_add_tail(&data->list, &hello_list);
	}
}
EXPORT_SYMBOL(print_hello);

static int __init hello1_init(void)
{
	pr_info("hello1 module loaded\n");
	return 0;
}

static void __exit hello1_exit(void)
{
	struct hello_data *data, *tmp;
	s64 delta_ns;

	list_for_each_entry_safe(data, tmp, &hello_list, list) {
		delta_ns = ktime_to_ns(ktime_sub(data->after, data->before));
		pr_info("Time taken for print: %lld ns\n", delta_ns);
		list_del(&data->list);
		kfree(data);
	}

	pr_info("hello1 module unloaded\n");
}

module_init(hello1_init);
module_exit(hello1_exit);