#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/hrtimer.h>
#include <linux/timer.h>
#include <linux/slab.h>
#include <linux/version.h>

struct mydata
{
	int id;
	int interval;
	struct hrtimer hrt;
	ktime_t period;
};

static struct mydata *my;


enum hrtimer_restart f_timer( struct hrtimer *h )
{
	struct mydata *p = container_of( h, struct mydata, hrt );
	ktime_t now = h->base->get_time();

		pr_info( "Timer[ %d ] Period: %d\n", p->id, p->interval );
		hrtimer_forward( h, now, p->period );
		
		return HRTIMER_RESTART;
}

static int __init my_init( void )
{
	struct mydata *p;
	int i;

		my = kmalloc( 2 * sizeof( struct mydata ), GFP_KERNEL );

		for( p = my, i = 0; i < 2 ; p++, i++ )
		{
			p->id = i;
			p->interval = i > 0 ? 10 : 1;
			p->period = ktime_set( p->interval, 0 );
			hrtimer_init( &p->hrt, CLOCK_REALTIME, HRTIMER_MODE_REL );
			p->hrt.function = f_timer;
			hrtimer_start( &p->hrt, p->period, HRTIMER_MODE_REL );
		}

		return 0;
}

static void __exit my_exit( void )
{
	struct mydata *p;
	int i;
	
		for( p = my, i = 0; i < 2 ; p++, i++ )
		{
			pr_info( "Cancelling Timer[%d]\n", p->id );
			hrtimer_cancel( &p->hrt );
		}

		kfree( my );
}

module_init( my_init );
module_exit( my_exit );

MODULE_LICENSE("GPL");

