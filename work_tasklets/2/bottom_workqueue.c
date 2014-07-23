#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>

#define MYIRQ 49

static void bottom_f( struct work_struct *w );

DECLARE_WORK( mybottom, bottom_f );

static void bottom_f( struct work_struct *w )
{
	static unsigned long counter = 0;
	
		pr_info( "[ bottom counter - %ld ]\n", ++counter );
}	

static irqreturn_t mytop_halve( int irq, void *dev_id )
{
	static unsigned long counter = 0;

		pr_info( "[ top counter - %ld ]\n", ++counter );
		schedule_work( &mybottom );
		
		return IRQ_NONE;
}

static __init int bottom_init( void )
{
	int ret = 0;
	
		ret = request_irq( MYIRQ, mytop_halve, IRQF_SHARED, 
					"shared_by_oscar", bottom_init );

		if( ret < 0 )
			pr_info( "Request irq: %d\n", ret );

		INIT_WORK( &mybottom, bottom_f );
		
		return ret;
}

static __exit void bottom_exit( void )
{
	synchronize_irq( MYIRQ );
	free_irq( MYIRQ, bottom_init );
}

module_init( bottom_init );
module_exit( bottom_exit );
MODULE_AUTHOR( "Oscar Salvador" );
MODULE_DESCRIPTION( "bottom halves" );
MODULE_LICENSE( "GPL v2" );
