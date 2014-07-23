#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/mutex.h>

extern struct mutex mymutex;

static int __init my_init( void )
{
        int ret;

                pr_info( "Trying to lock the mutex\n" );
                ret = mutex_trylock( &mymutex );
                pr_info( "Locked : %s\n", ret == 1 ? "yes" : "no" );

        return ( ret ) ? 0 : -ret;
}

static void my_exit( void )
{
	mutex_unlock( &mymutex );
}

module_init( my_init );
module_exit( my_exit );

MODULE_AUTHOR("Oscar Salvador");
MODULE_DESCRIPTION("Init mutex1");
MODULE_LICENSE("GPL v2");
