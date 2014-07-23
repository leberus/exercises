#include <linux/init.h>
#include <linux/module.h>
#include <linux/netdevice.h>

static void mysetup(struct net_device *d);
static int mynet_open(struct net_device *p);
static int mynet_close(struct net_device *b);

static struct net_device *mydev;


static int mynet_open(struct net_device *p)
{
	pr_info("Activating the interface %s...\n", p->name);
	return 0;
}

static int mynet_close(struct net_device *b)
{
	pr_info("Disabling the interface %s...\n", b->name);
	return 0;
}

static struct net_device_ops ndo = {
        .ndo_open = mynet_open,
        .ndo_stop = mynet_close,
        //.ndo_start_xmit = stub_start_xmit,
};

static void mysetup(struct net_device *d)
{
        int j;

                for(j = 0 ; j < ETH_ALEN ; j++)
                        d->dev_addr[j] = (char)j;

                ether_setup (d);
		d->netdev_ops = &ndo;
}



static int __init my_init(void)
{
	int ret = 0;

		mydev = alloc_netdev(0, "mynet%d", mysetup);
	
		if( register_netdev(mydev) ) {
			pr_debug("Failed allocating netdev");
			free_netdev(mydev);
			ret = -EINVAL;
		}	
		pr_debug("Sucees!\n");

		return ret;		
}

static void __exit my_exit(void)
{
	unregister_netdev(mydev);
	free_netdev(mydev);
	pr_info("Device disabled\n");
}

module_init(my_init);
module_exit(my_exit);

