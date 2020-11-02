#ifndef _REENTRANT
#define _REENTRANT
#endif

#include <pthread.h>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#define BABYBIRDS 10
#define WORMS 10
#define HUNGER 10

int babyB,babyHungry ,hunger;

class monitor{

public:

    explicit monitor(){

        pthread_mutex_init(&mutex, NULL);

        pthread_cond_init(&fill,NULL);

        pthread_cond_init(&empty, NULL);
    }
    void fillDish(void *arg){

        pthread_mutex_lock(&mutex);

        while(worms != 0) pthread_cond_wait(&empty,&mutex);

        worms = WORMS;

        printf("DAD FILLED THE DISH, WORMS ARE NOW: %d\n", worms);

        pthread_cond_signal(&fill);

        pthread_mutex_unlock(&mutex);
    }

    void eatWorm(void *arg){

        pthread_mutex_lock(&mutex);

        while(worms == 0) pthread_cond_wait(&fill,&mutex);

        worms--;

        printf("BIRD %ld, WORMS %d\n",(long)arg, worms);

        pthread_cond_signal(&empty);

        pthread_mutex_unlock(&mutex);

        sleep(static_cast<unsigned int>(0.5));
    }
private:

    int worms = WORMS;

    pthread_mutex_t mutex;

    pthread_cond_t fill;

    pthread_cond_t empty;

}dish;

void *ParentBird(void *arg){
    while(babyHungry>= 0) dish.fillDish(arg);
}

void *BabyBird(void *arg){
    while(hunger != 0) dish.eatWorm(arg);
}

int main(int argc, char *argv[]){

    pthread_attr_t attr;

    pthread_t workerid[BABYBIRDS + 1];

    pthread_attr_init(&attr);

    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

    hunger = (argc > 3)? atoi(argv[3]) : HUNGER;

    babyB = (argc > 1)? atoi(argv[1]) : BABYBIRDS;

    (argc > 2) ? atoi(argv[2]) : WORMS;

    babyHungry = babyB;

    long i = 0;

    pthread_create(&workerid[0],NULL ,ParentBird, (void *) i);

    for(i = 1; i <= babyB; i++) pthread_create(&workerid[i],NULL, BabyBird, (void *) i);

    for(i = 1; i<= babyB; i++) pthread_join(workerid[i], NULL);

}
