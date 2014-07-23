#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/semaphore.h>

static struct semaphore mysemaphore;

static int __init my_init( void )
{
	sema_init( &mysemaphore, 1 );
	pr_info( "Semaphore initialitzed\n" );

        return 0;
}

static void my_exit( void )
{
	pr_info( "Exiting..\n" );
}

EXPORT_SYMBOL( mysemaphore );

module_init( my_init );
module_exit( my_exit );

MODULE_AUTHOR("Oscar Salvador");
MODULE_DESCRIPTION("Init semaphore");
MODULE_LICENSE("GPL v2");
