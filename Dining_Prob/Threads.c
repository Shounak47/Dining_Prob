#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

const int k = 5;

pthread_mutex_t chopsticks[k];
pthread_mutex_t bowl1;
pthread_mutex_t bowl2;
void acquire_chop(int);
void release_bowls();
void acquire_bowls();
void eat(int);
void putdown_chop(int);
void acquire_chop(int);
typedef struct mysemaphore{
int count;
pthread_mutex_t lock;
pthread_cond_t cv;
}mysemaphore;
mysemaphore roomlock;
void sem_create(mysemaphore *s,int val){
s->count=val;
pthread_mutex_init(&s->lock, NULL);
pthread_cond_init(&s->cv, NULL);
}
void signal1(mysemaphore *s){
pthread_mutex_lock(&s->lock);
//printf(%d\n,s->count);
s->count++;

pthread_mutex_unlock(&s->lock);
pthread_cond_signal(&s->cv);

}
void wait1(mysemaphore *s){
	pthread_mutex_lock(&s->lock);
	while(s<=0){
	pthread_cond_wait(&s->cv, &s->lock);

}
	s->count--;
  	pthread_mutex_unlock(&s->lock);

}
void * philosopher(void * num)
{
	while(1){
		int idx=*(int *)num;
	wait1(&roomlock);

	printf("Philosopher %d has sat down on the table\n",idx);
	acquire_chop(idx);
	acquire_bowls();
	eat(idx);
	release_bowls();
	putdown_chop(idx);

	signal1(&roomlock);
	printf("Philosopher %d has left the table and is thinking\n",idx);
	}
	

	return NULL;
}
void release_bowls(){
	pthread_mutex_unlock(&bowl1);
	pthread_mutex_unlock(&bowl2);
}
void putdown_chop(int idx){
	pthread_mutex_unlock(&chopsticks[(idx+1)%k]);
	pthread_mutex_unlock(&chopsticks[idx]);
	printf("Philosopher %d has finished eating\n",idx);
}
void acquire_bowls(){
pthread_mutex_lock(&bowl1);
pthread_mutex_lock(&bowl2);
}
void eat(int idx){
	printf("Philosopher %d is eating with fork %d and fork %d\n",idx,idx,(idx+1)%k);
	sleep(1);
}
void acquire_chop(int idx){
	pthread_mutex_lock(&chopsticks[idx]);
	pthread_mutex_lock(&chopsticks[(idx+1)%k]);
}
int main(){
pthread_t ptid[k];
pthread_mutex_init(&bowl1, NULL);
pthread_mutex_init(&bowl2, NULL);
sem_create(&roomlock,k-1);
int a[k];
	for(int i=0;i<k;i++){
	a[i]=i;
	pthread_mutex_init(&chopsticks[i],NULL);
	pthread_create(&ptid[i],NULL,philosopher,&a[i]);


	}
	for(int i=0;i<k;i++){
		pthread_join(ptid[i],NULL);
	}
	return 0;
}