#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static char *state[3] = { "LIVE", "COMING", "GOING" };

static int __init my_init( void )
{
	struct module *module_t = THIS_MODULE;
	struct module *p;
	struct list_head *l;
		
		list_for_each( l, &module_t->list )
		{	
			p = list_entry( l, struct module, list );
			printk( KERN_INFO "Name: %s\n", p->name );
		}

		return 0;
}

static void __exit my_exit( void )
{
	printk( KERN_INFO "Exit\n" );
}

module_init( my_init );
module_exit( my_exit );
