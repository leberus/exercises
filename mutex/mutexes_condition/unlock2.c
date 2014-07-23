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
                pr_info( "Locked : %s - %d\n", ret == 1 ? "yes" : "no", ret );

        return ( ret ) ? 0 : -1;
}

static void my_exit( void )
{
	mutex_unlock( &mymutex );
}

module_init( my_init );
module_exit( my_exit );

MODULE_AUTHOR("Oscar Salvador");
MODULE_DESCRIPTION("Init mutex2");
MODULE_LICENSE("GPL v2");
