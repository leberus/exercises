#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/wait.h>
#include <linux/module.h>

#define MYIRQ 49

static int bottom_f( void *data );

static struct task_struct *mythread;

static wait_queue_head_t wq;
static atomic_t cond;

static atomic_t top_counter;
static atomic_t bottom_counter;
	

static int bottom_f( void *data )
{

		do
		{
			atomic_set( &cond, 0 );
			wait_event_interruptible( wq, atomic_read( &cond ) );
			
			if( atomic_read( &cond ) )
				atomic_inc( &bottom_counter );

			pr_info( " [ top counter - %d ] - [ bottom counter - %d ]\n", atomic_read( &top_counter ), atomic_read( &bottom_counter ) );
		}while( !kthread_should_stop() );

		return 0;
}	

static irqreturn_t mytop_halve( int irq, void *dev_id )
{
		atomic_inc( &top_counter );
		pr_info( "[ top counter - %d ]\n", atomic_read( &top_counter ) );
		
		atomic_set( &cond, 1 );
		wake_up_interruptible( &wq );

		return IRQ_NONE;
}

static __init int bottom_init( void )
{
	int ret = 0;
	
		ret = request_irq( MYIRQ, mytop_halve, IRQF_SHARED, 
					"shared_by_oscar", bottom_init );

		if( ret < 0 )
			pr_info( "Request irq: %d\n", ret );

		atomic_set( &top_counter, 0 );
		atomic_set( &bottom_counter, 0 );
		atomic_set( &cond, 0 );

		init_waitqueue_head( &wq );
		mythread = kthread_run( bottom_f, bottom_init, "mythread" );

		if( mythread == NULL )
			return -1;
		
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
