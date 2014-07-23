#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <asm/atomic.h>

#define MYIRQ 49
#define MAXIRQ 50

static void bottom_h( unsigned long data );

struct myirqcount
{
	int irq;
};

static struct myirqcount irqcount_t;

DECLARE_TASKLET( shared_interrupts, bottom_h, ( unsigned long )&irqcount_t );

static atomic_t *events_handled;
static atomic_t *events_requested;
static atomic_t *irqs_shared;


static void bottom_h( unsigned long data )
{
	struct myirqcount *t = ( struct myirqcount * )data;

		pr_info( "Calling tasklet from interrupr: %d\n", t->irq );
		
		while( atomic_read( &events_handled[t->irq] ) < atomic_read( &events_requested[t->irq] ) )
		{
			atomic_inc( &events_handled[t->irq] );
			pr_info( " [ Events requested - %d ] - [ Events handled - %d ]\n", atomic_read( &events_requested[t->irq] ), atomic_read( &events_handled[t->irq] ) );
		} 
}	

static irqreturn_t mytop_halve( int irq, void *dev_id )
{
		irqcount_t.irq = irq;
		atomic_inc( &events_requested[irqcount_t.irq] );
		tasklet_schedule( &shared_interrupts );
		
		return IRQ_NONE;
}

static __init int bottom_init( void )
{
	int irq ;
	int ret = 0;

		events_handled = kmalloc( ( MAXIRQ -1 ) * sizeof( atomic_t ), GFP_KERNEL );
		events_requested = kmalloc( ( MAXIRQ -1 ) * sizeof( atomic_t ), GFP_KERNEL );
		irqs_shared = kmalloc( ( MAXIRQ -1 ) * sizeof( atomic_t ), GFP_KERNEL );

		for( irq = 0 ; irq < MAXIRQ ; irq++ )
		{
			atomic_set( &irqs_shared[irq], -1 );
			atomic_set( &events_handled[irq], 0 );
			atomic_set( &events_requested[irq], 0 );
	
			ret = request_irq( irq, mytop_halve, IRQF_SHARED, 
						"shared_by_oscar", irqs_shared );

			if( ret < 0 )
				pr_info( "Impossible share interrupt with: ( %d )\n", irq );
			else
				atomic_set( &irqs_shared[irq], 0 );	
		}

		return ret;
}

static __exit void bottom_exit( void )
{
	int irq;
	int freed_irq = 0;

		for( irq = 0 ; irq < MAXIRQ ; irq++ )
		{	
			if( atomic_read( &irqs_shared[irq] ) >= 0 )
			{
				pr_info( "Freeing irq [%d]: Events: ( %d ) - Handled Events: ( %d )\n", irq, atomic_read( &events_requested[irq] ), atomic_read( &events_handled[irq] ) );
				synchronize_irq( MYIRQ );
				free_irq( irq, irqs_shared );
				freed_irq++;
			}
		}

		pr_info( "Irq freed: ( %d )\n", freed_irq );
}

module_init( bottom_init );
module_exit( bottom_exit );
MODULE_AUTHOR( "Oscar Salvador" );
MODULE_DESCRIPTION( "bottom halves" );
MODULE_LICENSE( "GPL v2" );
