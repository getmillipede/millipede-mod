#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/time.h>
#include <linux/kthread.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sebastien Rannou <mxs@sbrk.org> (@aimxhaisse)");
MODULE_DESCRIPTION("Inserts a millipede in your kernel");

static struct task_struct *spamming_thread;

#define DELAY_MS 5000
#define MILLIPEDE_SIZE 20

static void print_millipede(void)
{
  const char *padding_offsets[8] = {"  ", " ", "", " ", "  ", "   ", "    ", "   "};
  int i;

  printk(KERN_INFO "    ╚⊙ ⊙╝\n");
  for (i = 0; i < MILLIPEDE_SIZE; i++)
    {
      printk(KERN_INFO "%s╚═(███)═╝\n", padding_offsets[i % 8]);
    }
  printk(KERN_INFO"    ╔⊙ ⊙╗\n");
}

static int millipede_spamming_thread(void * data)
{
  while (!kthread_should_stop())
    {
      print_millipede();
      mdelay(DELAY_MS);
    }
  return 0;
}

static int __init millipede_init(void)
{
  spamming_thread = kthread_create(&millipede_spamming_thread, NULL, "spamirq");
  if (IS_ERR(spamming_thread))
    {
      printk(KERN_ALERT "kthread_create error\n");
      return 1;
    }
  kthread_bind(spamming_thread, 0);
  wake_up_process(spamming_thread);
  return 0;
}

static void __exit millipede_cleanup(void)
{
  kthread_stop(spamming_thread);
}

module_init(millipede_init);
module_exit(millipede_cleanup);
