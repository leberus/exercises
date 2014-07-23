#include <linux/init.h>
#include <linux/module.h>
#include <linux/pci.h>

static __init int mypci_init( void )
{
	struct pci_dev *pdev = NULL;

		for_each_pci_dev( pdev )
		{
			pr_info( "Device: %d\n", pdev->device );
			pci_dev_put(pdev);
		}

	return 0;
}

static __exit void mypci_exit( void )
{
	pr_info( "Unloaded\n" );
}

module_init( mypci_init );
module_exit( mypci_exit );
