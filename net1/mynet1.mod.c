#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
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
	{ 0x6a398f25, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x292b58ba, __VMLINUX_SYMBOL_STR(free_netdev) },
	{ 0xdd5908e3, __VMLINUX_SYMBOL_STR(unregister_netdev) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xed6979cb, __VMLINUX_SYMBOL_STR(register_netdev) },
	{ 0x464bc8d, __VMLINUX_SYMBOL_STR(alloc_netdev_mqs) },
	{ 0xfd97df80, __VMLINUX_SYMBOL_STR(ether_setup) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

