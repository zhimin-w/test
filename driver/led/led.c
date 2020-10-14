
#include <linux/module.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>

#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/gpio.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/interrupt.h>

// MX6ULL_PAD_SNVS_TAMPER3__GPIO5_IO03 

#define GPIO_TO_PIN(bank, gpio)  (((bank) - 1) * 32 + (gpio))
#define LED2   GPIO_TO_PIN(5, 3)

#define LED_NAME "led"
dev_t devid;
struct cdev  *cdev;
struct class *class;

static int led_open(struct inode *inode, struct file *file)
{
	printk(" led_open ... \n");
	return 0;
}

static int led_close(struct inode *inode, struct file *file)
{
	return 0;
}

static long led_ioctl(struct file* filp, unsigned int cmd,unsigned long arg)
{
	switch(cmd)
	{
		case 0:
		{
			gpio_set_value(LED2, 0);
			break;
		}
		case 1:
		{	
			gpio_set_value(LED2, 1);
			break;
		}
	}

	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = led_open,
	.release = led_close,	
	.unlocked_ioctl = led_ioctl,
};


static int led_init(void)
{
	int ret = alloc_chrdev_region(&devid, 0, 1, LED_NAME);
	if(ret)
	{
		printk(" alloc_chrdev_region error ");
		return -1;
	}
	
	cdev = cdev_alloc();
	cdev->owner = THIS_MODULE;
	cdev_init(cdev, &fops);
	cdev_add(cdev, devid, 1);	

	class = class_create(THIS_MODULE, LED_NAME);
	device_create(class, NULL, devid, NULL, LED_NAME);

	ret = gpio_request(LED2, "led2");
	if(ret)
	{
		printk(" gpio_request error... \n");
		return -1;
	}
	gpio_direction_output(LED2, 1);
	

	return 0;
}

static void led_exit(void)
{
	unregister_chrdev_region(devid, 1);
	cdev_del(cdev);
	
	device_destroy(class, devid);
	class_destroy(class);
	
	gpio_free(LED2);
}


module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");





