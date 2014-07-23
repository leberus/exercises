#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/slab.h>


static struct mystruct {
	struct timer_list mytimmy;
	unsigned long data;
	char id;
}*a_mystruct;

unsigned long addr;	
unsigned long jiffies_on_load;

static void function_timer( unsigned long data )
{
	struct mystruct *p = ( struct mystruct * )data;
	unsigned int delay;

		pr_info( "[Timer] Function timer called! at address 0x%lx\n", data );
		pr_info( "[Timer %c] Jiffies on load: %ld, Current jiffies: %ld, Elapsed jiffies: %ld, Period %ld\n", 
						p->id, jiffies_on_load, jiffies, jiffies - jiffies_on_load, p->data );

		delay = p->data;
		mod_timer( &(p->mytimmy), jiffies + delay );
}


static int __init my_init( void )
{
	int nr_timers;
	unsigned long time;
	struct mystruct *p;
	
		a_mystruct = kmalloc( 2 * sizeof( struct mystruct ), GFP_KERNEL );
		
		for( p = a_mystruct, nr_timers = 0 ; nr_timers < 2 ; nr_timers++, p++ )
		{
			pr_info( "First structure allocated in: %p\n", p );
			time = nr_timers > 0 ? 1 : 10;
			init_timer( &(p->mytimmy) );
			p->mytimmy.expires = jiffies + ( HZ * time );
			p->mytimmy.function = function_timer;
			p->mytimmy.data = ( unsigned long )p;
			p->id = nr_timers > 0 ? 'B' : 'A';
			p->data = HZ * time;
			add_timer( &(p->mytimmy) );
			jiffies_on_load = jiffies;
		}

		return 0;
}

static void __exit my_exit( void )
{
	struct mystruct *p;
	int i;

		for( p = a_mystruct, i = 0 ; i < 2 ; p++, i++ )
		{
			pr_info( "Deleting Timer [ %c ], function allocated at %p\n", p->id, p );
			del_timer_sync( &p->mytimmy );
		}

		kfree( a_mystruct );

		pr_info( "Exit!\n" );
}

module_init( my_init );
module_exit( my_exit );


