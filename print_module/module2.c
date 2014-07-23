#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static char *state[3] = { "LIVE", "COMING", "GOING" };

static int __init my_init( void )
{
	struct module *m = THIS_MODULE;
	struct list_head *l;
	int j = 0;
	
		l = &m->list;
		l = l->prev;

		list_for_each_entry( m, l, list )
		{
			pr_info("%3d MOD:%20s, taints = %u\n", j++, m->name, m->taints);
		}
		

		return 0;
}

static void __exit my_exit( void )
{
	printk( KERN_INFO "Exit\n" );
}

module_init( my_init );
module_exit( my_exit );
