#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/mutex.h>
#include <linux/interrupt.h>
#include <linux/stringify.h>

static struct mutex mymutex;
unsigned long mydata = 5;

irqreturn_t h_interrupt( int irq, void *dev_id )
{

	mutex_unlock( &mymutex );
	pr_info( "[INT] Mutex Unlocked: [ %d ]\n", atomic_read( &mymutex.count ) );
		
	return IRQ_HANDLED;
}

static int __init myinit( void )
{
	int ret;

		ret = request_irq( 48, h_interrupt, IRQF_SHARED, "mydev", &mydata );
		mutex_init( &mymutex );
		mutex_lock( &mymutex );
		pr_info( "[INIT] Mutex Unlocked: [ %d ]\n", atomic_read( &mymutex.count ) );

		return 0;
}

static void __exit myexit( void )
{
	synchronize_irq( 48 );
	free_irq( 48, &mydata );
	pr_info( "Goodbay..\n" );
}

module_init( myinit );
module_exit( myexit );
