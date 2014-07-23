#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/mutex.h>

static struct mutex mymutex;

static int __init my_init( void )
{
	mutex_init( &mymutex );
	pr_info( "Mutex initialitzed\n" );

        return 0;
}

static void my_exit( void )
{
	pr_info( "Exiting..\n" );
}

EXPORT_SYMBOL( mymutex );

module_init( my_init );
module_exit( my_exit );

MODULE_AUTHOR("Oscar Salvador");
MODULE_DESCRIPTION("Init mutex");
MODULE_LICENSE("GPL v2");
