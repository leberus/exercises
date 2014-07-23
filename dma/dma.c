#include <linux/init.h>
#include <linux/module.h>
#include <linux/dma-mapping.h>

static unsigned int bufsize = 1024;
static dma_addr_t dma_addr;

static void *kernel_buffer;

static __init int myinit_dma( void )
{
	unsigned long diff;
	unsigned long real;

		kernel_buffer = dma_alloc_coherent( NULL, bufsize, &dma_addr, GFP_KERNEL );
		diff = ( unsigned long )kernel_buffer - dma_addr;

		pr_info( "Kernel buffer - %12p , Dma_addr - %12p\n", kernel_buffer, ( void * )dma_addr );
		pr_info( "Kernelbuffer - dma_addr - %12p, %12lu, page_offset - %12lu, diff - page_offset - %lu\n",
								( void * )diff, diff, PAGE_OFFSET, diff - PAGE_OFFSET );
		real = virt_to_phys( kernel_buffer );
		pr_info( "Real - %lx\n", real );

		strcpy( kernel_buffer, "Hello word\n" );

		return 0;
}

static __exit void myexit_dma( void )
{
	pr_info( "Kernel buffer contains: %s", ( char * )kernel_buffer );
	dma_free_coherent( NULL, bufsize, kernel_buffer, dma_addr );
}

module_init( myinit_dma );
module_exit( myexit_dma );
