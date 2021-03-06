/* Finding Tainted Modules
 *
 * All modules loaded on the system are linked in a list that can be
 * accessed from any module:
 *
 *    struct module {
 *     ....
 *    struct list_head list;
 *    ....
 *    char name[MODULE_NAME_LEN];
 *    ....
 *    unsigned int taints;
 *    ....
 *    }
 *
 * Write a module that walks through this linked list and prints out
 * the value of taints and any other values of interest.  (The module
 * structure is defined in /usr/src/linux/include/linux/module.h.)
 *
 * You can begin from THIS_MODULE.
 @*/

#include <linux/module.h>
#include <linux/init.h>

static int __init my_init(void)
{
	int j = 0;
	struct list_head *modules;
	struct module *m = THIS_MODULE;
	modules = &m->list;
	pr_info("\n");
	pr_info("%3d MOD:%20s, taints = %d\n", j++, m->name, m->taints);
	list_for_each_entry(m, modules, list) {
		pr_info("%3d MOD:%20s, taints = %d\n", j++, m->name, m->taints);
	/*	if (j > 100)	 just here to avoid a runaway, don't need 
			break; */
	}
	return 0;
}

static void __exit my_exit(void)
{
	pr_info("Bye: module unloaded from 0x%p\n", my_exit);
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Jerry Cooperstein");
MODULE_DESCRIPTION("LDD:2.0 s_07/lab2_taints.c.OLD");
MODULE_LICENSE("GPL v2");
