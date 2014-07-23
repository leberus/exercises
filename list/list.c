#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>

struct my_struct
{
	int id;
	struct list_head list;
};


static int __init my_init( void )
{
	struct my_struct my_list;
	struct my_struct *p;
	struct list_head *l, *q;
	int i;
	
		INIT_LIST_HEAD( &my_list.list );

		for( i = 0 ; i < 5 ; i++ )
		{
			p = kmalloc( sizeof(struct my_struct), GFP_KERNEL );
			p->id = i;
			printk( KERN_INFO "Id: %d\n", p->id );
			list_add( &(p->list), &(my_list.list) );
		}

		printk( KERN_INFO  "%d\n", list_empty( &my_list.list ) );

		list_for_each( l, &my_list.list )
		{
			p = list_entry( l, struct my_struct, list );
			printk( KERN_INFO "Id: %d\n", p->id );
		}

	
		list_for_each_safe( l, q, &(my_list.list) )
		{
			p = list_entry( l, struct my_struct, list );
			list_del( l );
			kfree( p );
		}

		printk( KERN_INFO "%d\n", list_empty( &my_list.list ) );

		return 0;
		
}

static void __exit my_exit( void )
{
	printk( KERN_INFO "Exit\n" );
}

module_init( my_init );
module_exit( my_exit );

