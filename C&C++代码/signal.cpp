#include "header.h"
using namespace std;
#ifndef THREAD_POOL_SIZE
#define THREAD_POOL_SIZE ((const int)50)
#endif
#ifndef RUN
#define RUN 1
#endif
#ifndef EXIT
#define EXIT 0
#endif
static volatile int processSign=RUN;
static volatile int threadPoolSign=RUN;
static volatile int taskNumer=0;
static pthread_mutex_t globleMutex=PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t queueMutex=PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t globleCond=PTHREAD_COND_INITIALIZER;

static void process_exit(int sig){
	if(sig==SIGTERM){
		processSign=EXIT;
	}
}
struct Task{
	int number;
	char* details;
	const char* sourceFile;
	const char* resultFile;
};
queue<Task> taskQueue;
void *taskDispatchThread(void* arg){
	int index;
	index=*(int*) arg;
	Task newTask;
	newTask.number=++taskNumber;
	newTask.detail="filename"+" to " +"filename";
	const char* source="原始文件地址";
	const char* result="处理后文件地址";
	newTask.source=source;
	newTask.result=result;
	pthread_mutex_lock(&queueMutex);
	taskQueue.push(newTask);
	pthread_mutex_unlock(&queueMutex);
}
void *singleThread(void* arg){
	int index;
	index=*(int*)arg;
	while(1){
		pthread_mutex_lock(&globleMutex);
		pthread_mutex_lock(&queueMutex);
		register int empty=taskQueue.empty();
		pthread_mutex_unlock(&queueMutex);
		while(threadPoolSign==RUN&&empty){
			pthread_cond_wait(&globleCond,&globleMutex);
			pthread_mutex_lock(&queueMutex);
			empty=taskQueue.empty();
			pthread_mutex_unlock(&queueMutex);
		}
		if(threadPoolSign==EXIT){
			pthread_mutex_unlock(&globleMutex);
			break;
		}
		pthread_mutex_lock(&queueMutex);
		Task task=taskQueue.front();
		taskQueue.pop();
		pthread_mutex_unlock(&queueMutex);
		pthread_mutex_unlock(&globleMutex);
		//任务处理，没写完。。 
	}
}
int main(){
	int i,j;
	pthread_t threadId[THREAD_POOL_SIZE];
	signal(SIGTERM,process_exit);
	processSign=RUN;
	threadPoolSign=RUN;
	//创建线程池 
	for(i=0;i<THREAD_POOL_SIZE-1;i++){
		if(0!=(rc=pthread_creat(threadId+i,NULL,singleThread,&i))){
			processSign=EXIT;
			pthread_cond_broadcast(&globleCond);
			for(j=0;j<i;j++)pthread_join(threadId[j],NULL);
			printf("资源不足，线程池创建失败！"); 
			return -1;
		}
	}
	if(0!=(rc=pthread_creat(threadId+i,NULL,taskDispatchThread,&i)));
	while(processSign==RUN)usleep(10000);
	//线程池退出 
	threadPoolSign=EXIT;
	pthread_cond_broadcast(&globleCond);
	//销毁线程池 
	for(i=0;i<THREAD_POOL_SIZE;i++)pthread_join(threadId[i],NULL);
	printf("Process exit.");
} 
