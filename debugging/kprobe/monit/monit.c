#include <linux/module.h>
#include <linux/init.h>
#include <linux/kallsyms.h>

static int __init my_init( void )
{

	pr_info( "I'm going to export monitored_f\n" );
	return 0;
}

static void __exit my_exit( void )
{
	pr_info( "Exit..\n" );
}

module_init( my_init );
module_exit( my_exit );

MODULE_AUTHOR("Oscar Salvador");
MODULE_DESCRIPTION("MONITORED");
MODULE_LICENSE("GPL v2");
