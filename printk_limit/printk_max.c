#include <linux/module.h>
#include <linux/init.h>

static int __init my_init( void )
{
	int i;

		for( i = 0 ; i < 100 ; i++ )
			if( printk_ratelimit() )
				printk( KERN_INFO "Hello: module loaded at 0x%p\n", my_init );
		printk( KERN_INFO "Finish!\n" );

		return 0;
}

static void __exit my_exit( void )
{
	printk( KERN_CRIT "Hello: module unloaded at 0x%p\n", my_exit );
	
}

module_init( my_init );
module_exit( my_exit );

MODULE_AUTHOR( "Oskar" );
MODULE_LICENSE( "GPL v2" );

