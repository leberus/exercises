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
	{ 0x3ec8886f, "param_ops_int" },
	{ 0xd3f74e31, "class_destroy" },
	{ 0x9ee3dc36, "device_destroy" },
	{ 0x8c971e0b, "device_create" },
	{ 0x704af77a, "__class_create" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0x68f58ca6, "cdev_del" },
	{ 0xd641eabf, "cdev_add" },
	{ 0x9c10fd4, "cdev_init" },
	{ 0x27e44a3d, "cdev_alloc" },
	{ 0xdce16e04, "kmem_cache_destroy" },
	{ 0x29537c9e, "alloc_chrdev_region" },
	{ 0xb14a346e, "kmem_cache_create" },
	{ 0xba3be94c, "module_refcount" },
	{ 0x190b27b, "kmem_cache_alloc" },
	{ 0x71de9b3f, "_copy_to_user" },
	{ 0x77e2f33, "_copy_from_user" },
	{ 0xa1c76e0a, "_cond_resched" },
	{ 0xd4e45d05, "kmem_cache_free" },
	{ 0x27e1a049, "printk" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

