#include <linux/module.h>
#include <linux/init.h>
#include <linux/kallsyms.h>

static char *name = "do_fork";



static int __init my_init( void )
{

	unsigned long addr;
	
		addr = kallsyms_lookup_name( "do_fork" );
		printk( KERN_INFO "Addr-> 0x%08lx\n", addr );	

	return 0;
}

static void __exit my_exit( void )
{
	pr_info("Exit..\n");
}

module_init( my_init );
module_exit( my_exit );

MODULE_AUTHOR("Oscar Salvador");
MODULE_DESCRIPTION("kallsyms_lookup_name");
MODULE_LICENSE("GPL v2");
