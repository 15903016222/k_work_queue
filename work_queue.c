#include <linux/init.h>
#include <linux/module.h>
#include <linux/irq.h>
#include <linux/interrupt.h> 
#include <linux/input.h>
#include <linux/timer.h>

static struct workqueue_struct *dma_queue;
static struct work_struct btn_work;
static struct timer_list mytimer;

static void btn_work_function(struct work_struct *work)
{
    printk("down: %s\n", __func__);
}

static void mytimer_function (unsigned long data)
{
	// 无名工作队列的启动底半部的延后处理函数
//    schedule_work(&btn_work);
    // 有名工作队列的启动底半部的延后处理函数 -- 效率高于无名的工作队列
	queue_work (dma_queue, &btn_work);

    mod_timer(&mytimer, jiffies + msecs_to_jiffies(1000));
    printk("up: %s\n", __func__);
    return ; 
}

static int btn_init(void)
{
    init_timer(&mytimer);
    mytimer.expires = jiffies + msecs_to_jiffies(1000);
    mytimer.function = mytimer_function;
    add_timer(&mytimer);
  
	dma_queue = create_workqueue("dma_queue");
    INIT_WORK(&btn_work, btn_work_function);
    return 0;
}

static void btn_exit(void)
{
    del_timer(&mytimer);
}
module_init(btn_init);
module_exit(btn_exit);
MODULE_LICENSE("GPL");
