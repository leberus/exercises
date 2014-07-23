#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/semaphore.h>

extern struct semaphore mysemaphore;

static int __init my_init( void )
{
        int ret;

                pr_info( "Trying to lock the semaphore\n" );
                ret = down_trylock( &mysemaphore );
                pr_info( "Locked : %s\n", ret == 0 ? "yes" : "no" );

        return ret;
}

static void my_exit( void )
{
	up( &mysemaphore );
}

module_init( my_init );
module_exit( my_exit );

MODULE_AUTHOR("Oscar Salvador");
MODULE_DESCRIPTION("Init lock1");
MODULE_LICENSE("GPL v2");
