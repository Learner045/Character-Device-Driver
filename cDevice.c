#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include<linux/signal.h>
#include<linux/sched/signal.h>


#define  DEVICE_NAME "process_list"    // The device will appear at /dev/process_list using this value
#define  CLASS_NAME  "processList"        // The device class -- this is a character device driver

static int majorNumber;  
static struct class*  process_listClass  = NULL;
static struct device* process_listDevice = NULL;\

static char processInfo_buffer[20000] = {0}; //stores process info - pid, and stuff
static char task_state[200] = {0}; //store the state of task - running or interrupted or stuff
static short size_of_buffer;
static struct task_struct *task;

//file ops
static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);

static char* getStatus(int c);

static struct file_operations fops =
{
   .open = dev_open,
   .read = dev_read,
   .release = dev_release,
};

static int __init processListCharDevice_init(void){
   printk(KERN_INFO "ProcessListChar: Initializing the character device LKM\n");
 
   // register device and get it a major number 
   majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
   
   if (majorNumber<0){
      printk(KERN_ALERT "ProcessListChar device failed to register a major number\n");
      return majorNumber;
   }
   
   printk(KERN_INFO "ProcessListChar: registered correctly with major number %d\n", majorNumber);
 
   // Create/Register the device class
   process_listClass = class_create(THIS_MODULE, CLASS_NAME);
   
   if (IS_ERR(process_listClass)){               
      unregister_chrdev(majorNumber, DEVICE_NAME); //if err, unregister the device
      printk(KERN_ALERT "Failed to register device class\n");
      return PTR_ERR(process_listClass);          // Correct way to return an error on a pointer
   }
   printk(KERN_INFO "ProcessListChar: device class registered correctly\n");
 
   // Create/Register the device driver - makedevice(major, minor num)
   process_listDevice = device_create(process_listClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
   
   if (IS_ERR(process_listDevice)){               
      class_destroy(process_listClass);  //destroy the struct we created        
      unregister_chrdev(majorNumber, DEVICE_NAME); //unregister device
      printk(KERN_ALERT "Failed to create the device\n");
      return PTR_ERR(process_listDevice);
   }
   printk(KERN_INFO "ProcessListChar: device class created correctly\n"); // Made it! device was initialized
   return 0;
}


static void __exit processListCharDevice_exit(void){
   device_destroy(process_listClass, MKDEV(majorNumber, 0));     // remove the device
   class_unregister(process_listClass);                          // unregister the device class
   class_destroy(process_listClass);                             // destroy struct we created
   unregister_chrdev(majorNumber, DEVICE_NAME);             // unregister the major number
   
   printk(KERN_INFO "ProcessListChar: Goodbye from the LKM!\n");
}


static int dev_open(struct inode *inodep, struct file *filep){
   
   printk(KERN_INFO "ProcessListChar: Device has been opened \n");
   return 0;
}

//snprintf(task_state, 200, "%s", getStatus(1));

static char* getStatus(int c){
  switch(c){
   case 0: return "TASK_RUNNING";
   case 1: return "TASK_INTERRUPTIBLE";
   case 2: return "TASK_UNINTERRUPTIBLE";
   case 3: return "TASK_STOPPED";
   case 4: return "TASK_TRACED";
   case 5: return "EXIT_DEAD";
   case 6: return "EXIT_ZOMBIE";
   case 7: return "TASK_DEAD";
   case 8: return "TASK_WAKEKILL";
   case 9: return "TASK_WAKING";
   case 10: return  "TASK_PARKED";
   case 11: return "TASK_NOLOAD";
   case 12: return "TASK_NEW";
   case 13: return "TASK_STATE_MAX";
   case 14: return "EXIT_ZOMBIE, EXIT_DEAD";
   case 15: return "TASK_WAKEKILL, TASK_UNINTERRUPTIBLE";
   case 16: return "TASK_WAKEKILL, TASK_STOPPED";
   case 17: return "TASK_WAKEKILL, TASK_TRACED";
   case 18: return "TASK_UNINTERRUPTIBLE, TASK_NOLOAD";
   case 19: return "TASK_INTERRUPTIBLE, TASK_UNINTERRUPTIBLE";
   case 20: return "TASK_RUNNING, TASK_INTERRUPTIBLE, TASK_UNINTERRUPTIBLE , TASK_STOPPED, TASK_TRACED, EXIT_ZOMBIE, EXIT_DEAD";
   case 21: return "TASK_NORMAL, TASK_STOPPED, TASK_TRACED";
   default: return "OTHER";
  }
}



static ssize_t dev_read(struct file *filep, char *user_buffer, size_t len, loff_t *offset){
    printk(KERN_INFO "ProcessListChar: start reading from device \n");
    
int err_code=0;

	for_each_process(task)
	{
   if (task->state == 0){snprintf(task_state, 200, "%s", getStatus(0));}
   else if (task->state == 1){snprintf(task_state, 200, "%s", getStatus(1));}
   else if (task->state == 2){snprintf(task_state, 200, "%s", getStatus(2));}
   else if (task->state == 4){snprintf(task_state, 200, "%s", getStatus(3));}
   else if (task->state == 8){snprintf(task_state, 200, "%s", getStatus(4));}
   else if (task->state == 16){snprintf(task_state, 200, "%s", getStatus(5));}
   else if (task->state == 32){snprintf(task_state, 200, "%s", getStatus(6));}
   else if (task->state == 64){snprintf(task_state, 200, "%s", getStatus(7));}
   else if (task->state == 128){snprintf(task_state, 200, "%s", getStatus(8));}
   else if (task->state == 256){snprintf(task_state, 200, "%s", getStatus(9));}
   else if (task->state == 512){snprintf(task_state, 200, "%s", getStatus(10));}
   else if (task->state == 1024){snprintf(task_state, 200, "%s", getStatus(11));}
   else if (task->state == 2048){snprintf(task_state, 200, "%s", getStatus(12));}
   else if (task->state == 4096){snprintf(task_state, 200, "%s", getStatus(13));}
	 else if (task->state == (16 | 32)){snprintf(task_state, 200, "%s", getStatus(14));}
   else if (task->state == (128 | 2)){snprintf(task_state, 200, "%s", getStatus(15));}
   else if (task->state == (128 | 4)){snprintf(task_state, 200, "%s", getStatus(16));}
   else if (task->state == (128 | 8)){snprintf(task_state, 200, "%s", getStatus(17));}
   else if (task->state == (2 | 1024)){snprintf(task_state, 200, "%s", getStatus(18));}
   else if (task->state == (1 | 2)){snprintf(task_state, 200, "%s", getStatus(19));}
   else if (task->state == (0 | 1 | 2 | 4 | 8 | 32 |16)){snprintf(task_state, 200, "%s", getStatus(20));}
   else if (task->state == (1 | 2 | 4 | 8)){snprintf(task_state, 200, "%s", getStatus(21));}
   else {snprintf(task_state, 200, "%s", getStatus(22));}

		sprintf(processInfo_buffer + strlen(processInfo_buffer), "PID=%d  PPID=%d  CPU=%d  STATE= %s\n", task->pid, task->real_parent->pid, task_cpu(task)+1, task_state);
   
		size_of_buffer = strlen(processInfo_buffer);
	}
 
	err_code = copy_to_user(user_buffer, processInfo_buffer, size_of_buffer); //to from size

 
  printk(KERN_INFO "ProcessListChar: Device returning info to user program\n");
 
	return err_code;
    
}


static int dev_release(struct inode *inodep, struct file *filep){
   printk(KERN_INFO "ProcessListChar: Device successfully closed\n");
   return 0;
}


module_init(processListCharDevice_init); //called when module is installed
module_exit(processListCharDevice_exit); //called when module is removed


MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Shreya");
MODULE_DESCRIPTION("A simple Linux char driver for returning process information"); 