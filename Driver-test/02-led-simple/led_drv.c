#include <linux/module.h>

#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/kmod.h>
#include <linux/gfp.h>
#include <asm/io.h>

/* registers */
// IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 地址：0x02290000 + 0x14
static volatile unsigned int *IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3;

// GPIO5_GDIR 地址：0x020AC004
static volatile unsigned int *GPIO5_GDIR;

//GPIO5_DR 地址：0x020AC000
static volatile unsigned int *GPIO5_DR;

/*1.确定主设备号*/
static int major = 0;
static struct class *led_class;

/*3.实现open read write等接口，填入file_operations结构体*/
static int led_drv_open(struct inode *node, struct file *file){
  printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
  /*gpio引脚配置*/
  /* enable gpio5
	 * configure gpio5_io3 as gpio
	 * configure gpio5_io3 as output 
	 */
	*IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 &= ~0xf;
	*IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 |= 0x5;

	*GPIO5_GDIR |= (1<<3);
  
  return 0;
}

static ssize_t led_drv_write(struct file *file, const char __user *buf, size_t size, loff_t *offset){  
  char status;
  
  printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
  copy_from_user(&status, buf, 1);
  if(status == 1){
		/* set gpio to let led on */
		*GPIO5_DR &= ~(1<<3);
  }else{
		/* set gpio to let led off */
		*GPIO5_DR |= (1<<3);
  }
  
  return 1;
}

/*2.定义file_operations结构体*/
static struct file_operations led_drv_ops = {
  .owner = THIS_MODULE,
  .open = led_drv_open,
  .write = led_drv_write,
};

/*4.注册file_operations结构体到内核*/
/*5.驱动程序入口*/
static int __init led_drv_init(void){
  int err;
  
  printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
  major = register_chrdev(0, "fpvdope led", &led_drv_ops);

  /*ioremmap*/
	// IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 地址：0x02290000 + 0x14
	IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 = ioremap(0x02290000 + 0x14, 4);	
	// GPIO5_GDIR 地址：0x020AC004
	GPIO5_GDIR = ioremap(0x020AC004, 4);
	//GPIO5_DR 地址：0x020AC000
	GPIO5_DR  = ioremap(0x020AC000, 4);
  
	led_class = class_create(THIS_MODULE, "led class");
	err = PTR_ERR(led_class);
	if (IS_ERR(led_class)) {
    printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);	
    unregister_chrdev(major, "fpvdope led");
    return -1;
	}
  
  device_create(led_class, NULL, MKDEV(major, 0), NULL, "fpvdope led");
  
  return 0;
}

/*6.驱动程序出口*/
static void __exit led_drv_exit(void){
  printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
  
  iounmap(IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3);
	iounmap(GPIO5_GDIR);
	iounmap(GPIO5_DR);
  
  device_destroy(led_class, MKDEV(major, 0));    
  class_destroy(led_class);
  unregister_chrdev(major, "fpvdope led");
}

/*7.其他完善*/
module_init(led_drv_init);
module_exit(led_drv_exit);
MODULE_LICENSE("GPL");

