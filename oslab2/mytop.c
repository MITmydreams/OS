#include<stdio.h>
#include<unistd.h>
#include<sys/syscall.h>
#include<stdlib.h>
#include<string.h>

#define MAXCOMMAND 1024
#define LENGTH		20

typedef struct PROCESS {
	int p_pid;
	char p_command[16];
	unsigned long long p_runtime;
	long p_isrunning;
	int p_cpu;
}PROCESS;

int partition(PROCESS* a, int left, int right) {
	int k = rand() % (right - left) + left;
	PROCESS temp;
	PROCESS key = a[k];
	a[k] = a[right - 1];
	a[right - 1] = key;
	int i = left;
	for (int j = left; j < right; j++) {
		if (a[j].p_cpu >= key.p_cpu) {
			temp = a[i];
			a[i] = a[j];
			a[j] = temp;
			i++;
		}
	}
	return i;
}

void qSort(PROCESS* a, int left, int right) {
	if (left == right)
		return;
	int middle = partition(a, left, right);
	qSort(a, left, middle - 1);
	qSort(a, middle, right);
}

void quickSort(PROCESS* a, int length) {
	qSort(a, 0, length);
}

int main(int argc, char** argv)
{
	int time = 1;
	int timecnt = -1;
	if (argc >= 2) {
		time = 0;
		for (int i = 0; i < strlen(argv[1]); i++) {
			time = time * 10 + argv[1][i] - '0';
		}
	}
	if (argc == 3) {
		timecnt = 0;
		for (int i = 0; i < strlen(argv[2]); i++) {
			timecnt = timecnt * 10 + argv[2][i] - '0';
		}
	}
	PROCESS process[MAXCOMMAND];
	int counter0;
	int counter1;
	int* lastPID = (int*)malloc(MAXCOMMAND * sizeof(int));
	int* PID = (int*)malloc(MAXCOMMAND * sizeof(int));
	char* lastCOMMAND = (char*)malloc(MAXCOMMAND * 16 * sizeof(char));
	char* COMMAND = (char*)malloc(MAXCOMMAND * 16 * sizeof(char));
	unsigned long long* LAST_RUNTIME = (unsigned long long*)malloc(MAXCOMMAND * sizeof(unsigned long long));
	unsigned long long* RUNTIME = (unsigned long long*)malloc(MAXCOMMAND * sizeof(unsigned long long));
	long* ISRUNNING = (long*)malloc(MAXCOMMAND * sizeof(long));
	/* REAL_RUNTIME = RUNTIME - LAST_RUNTIME */
	int cnt;
	syscall(549, &counter1, PID, COMMAND, RUNTIME, ISRUNNING);
	
	while (1)
	{
		sleep(time);
		system("clear");
		counter0 = counter1;
		for (int i = 0; i < counter0; i++) {
			lastPID[i] = PID[i];
			strcpy(&lastCOMMAND[i*16], &COMMAND[i*16]);
			LAST_RUNTIME[i] = RUNTIME[i];
		}
		syscall(549, &counter1, PID, COMMAND, RUNTIME, ISRUNNING);
		cnt = 0;
		for (int i = 0; i < counter0; i++) {
			for (int j = 0; j < counter1; j++) {
				if (lastPID[i] == PID[j]&&strcmp(&lastCOMMAND[i * 16], &COMMAND[j * 16])==0) {
					strcpy(process[cnt].p_command, &(COMMAND[j * 16]));
					process[cnt].p_pid = PID[j];
					process[cnt].p_runtime = RUNTIME[j];
					process[cnt].p_isrunning = ISRUNNING[j] ? 0 : 1;
					process[cnt].p_cpu = (RUNTIME[j] - LAST_RUNTIME[i]) / 10000 / time;
					cnt++;
				}
			}
		}
		
		quickSort(process, cnt);
		printf("PID\tCOMM\t\tISRUNNING\t%%CPU\tTIME\n");
		for (int i = 0; i < cnt; i++) {
			if (i >= 20)
				break;
			printf("%d\t%-16s\t%d\t%.2f\t%.2f\n", process[i].p_pid, process[i].p_command, process[i].p_isrunning, (process[i].p_cpu * 0.01), (process[i].p_runtime * 1.0 / 10e6));
		}
		timecnt--;
		if (timecnt == 0)
			break;
	}
	return 0;
}