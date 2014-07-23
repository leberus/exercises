#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>

#define MYIRQ 49

static void bottom_f( unsigned long data );

DECLARE_TASKLET( mybottom, bottom_f, MYIRQ );

static atomic_t bottom_counter;
static atomic_t top_counter;

static void bottom_f( unsigned long data )
{
		while( atomic_read( &bottom_counter ) < atomic_read( &top_counter ) )
		{
			atomic_inc( &bottom_counter );
			pr_info( " [ top counter - %d ] - [ bottom counter - %d ]\n", atomic_read( &top_counter ), atomic_read( &bottom_counter ) );
		}
}	

static irqreturn_t mytop_halve( int irq, void *dev_id )
{

		atomic_inc( &top_counter );
		pr_info( "[ top counter - %d ]\n", atomic_read( &top_counter ) );
		tasklet_schedule( &mybottom );
		
		return IRQ_NONE;
}

static __init int bottom_init( void )
{
	int ret = 0;
	
		ret = request_irq( MYIRQ, mytop_halve, IRQF_SHARED, 
					"shared_by_oscar", bottom_init );
		pr_info( "Ret: %d\n", ret );

		if( ret < 0 )
			pr_info( "Request irq: %d\n", ret );

		atomic_set( &bottom_counter, 0);
		atomic_set( &top_counter, 0 );
		
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
