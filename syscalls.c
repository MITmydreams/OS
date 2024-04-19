SYSCALL_DEFINE1(ps_counter, int __user *, num){
	struct task_struct* task;
	int counter = 0;
	printk("[Syscall] ps_counter\n");
	for_each_process(task){
	counter ++;
	}
	copy_to_user(num, &counter, sizeof(int));
	return 0;
}

SYSCALL_DEFINE5(mytop, int __user * , counter ,int __user*, PID, char __user*, COMMAND, unsigned long long __user*, RUNTIME, long __user*, ISRUNNING)
{
	struct task_struct* task;
	int count = 0;
	printk("[Syscall] mytop\n");
	printk("[StuID] PB21000069\n");
	for_each_process(task) {
		copy_to_user(&PID[count], &(task->pid), sizeof(int));
		copy_to_user(&COMMAND[count*16], &(task->comm), 16 * sizeof(char));
		copy_to_user(&RUNTIME[count], &((task->se).sum_exec_runtime), sizeof(unsigned long long));
		copy_to_user(&ISRUNNING[count], &(task->state), sizeof(long));	/* -1 unrunnable, 0 runnable, >0 stopped */
		count++;
	}
	copy_to_user(counter, &count, sizeof(int));
	return 0;
}
