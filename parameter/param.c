#include <linux/module.h>
#include <linux/init.h>

static int irq = 12;

static int __init my_init( void )
{
	printk( KERN_INFO "Hello: module loaded at 0x%p value: %d\n", my_init, irq );
	return 0;
}

static void __exit my_exit( void )
{
	printk( KERN_CRIT "Hello: module unloaded at 0x%p\n", my_exit );
	
}

module_init( my_init );
module_exit( my_exit );
module_param( irq, int, S_IRUGO );

MODULE_AUTHOR( "Oskar" );
MODULE_LICENSE( "GPL v2" );

