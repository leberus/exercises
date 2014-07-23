#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x568fba06, "module_layout" },
	{ 0x68f58ca6, "cdev_del" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0xd641eabf, "cdev_add" },
	{ 0x9c10fd4, "cdev_init" },
	{ 0x27e44a3d, "cdev_alloc" },
	{ 0x29537c9e, "alloc_chrdev_region" },
	{ 0x4610d0e8, "send_sig" },
	{ 0x27e1a049, "printk" },
	{ 0xf85dc997, "pid_task" },
	{ 0x556215a7, "find_vpid" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

