#include "lib2.h"

co_t main_co, *current_co;
mythr_t *current_thread, main_thread;
int idCounter = 0;
int minSleepTime = 0;
pthread_mutex_t lock;
mysem_t *s;

// Initialization of the main coroutine
int mycoroutines_init(co_t *main) {
    if (main == NULL) return -1;

    if (getcontext(&main->context) == -1) return -1;
    main->context.uc_stack.ss_sp = (void *) malloc(STACK_SIZE); // Allocate a stack for main_co
    if (!main->context.uc_stack.ss_sp) return -1;
    main->context.uc_stack.ss_size = STACK_SIZE;
    main->context.uc_link = NULL; // No context to return to after main
    main->finished = 0;
    current_co = main;
    return 0;
}

// Creation of a new coroutine
// 'body' is a pointer to a function to be executed by the coroutine
int mycoroutines_create(co_t *co, void (body)(void *), void *arg) {
    if (!co || !body) return -1;

    co->stack = (void *) malloc(STACK_SIZE);
    if (!co->stack) return -1;

    if (getcontext(&co->context) == -1) return -1;
    
    co->context.uc_stack.ss_sp = co->stack;
    co->context.uc_stack.ss_size = STACK_SIZE;
    co->context.uc_link = &main_thread.coroutine.context; // when coroutine finishes return to &main_co.context
    co->finished = 0;
    makecontext(&co->context, (void (*)(void))body, 1, arg);
    return 0;
}

int mycoroutines_switchto(co_t *co) {
    if (co == NULL || co->finished == 1) return -1;

    co_t *prev_co = current_co;
    current_co = co;
    //printf("Switching context: %p -> %p\n", prev_co, current_co);
    if (swapcontext(&prev_co->context, &co->context) == -1) return -1;

    return 0;
}

int mycoroutines_destroy(co_t *co) {
    if (!co || co->finished) return -1;

    if (co->stack != NULL) free(co->stack);
    else printf("Unsuccessful free\n");
    co->stack = NULL;
    co->finished = 1;
    printf("success destroy\n");
    return 0;
}

int mythreads_init() {
    main_thread.coroutine = main_co;
    mycoroutines_init(&main_thread.coroutine);
    main_thread.finished = 0;
    main_thread.next = &main_thread;
    main_thread.status = READY;
    main_thread.id = -1;
    
    current_thread = &main_thread;
    return 1;
}

int mythreads_create(mythr_t *thr, void (body)(void *), void *arg){

    thr->id = idCounter;
    idCounter++;

    mycoroutines_create(&thr->coroutine, body, arg);
    thr->finished = 0; 
    thr->status = READY;
    thr->sleepTime = 0;
    thr->next = current_thread;
    thr->blocked = NULL;

    mythr_t *loopthr;
    loopthr = current_thread;
    
    while(loopthr->next != current_thread ){
        loopthr = loopthr->next;
    }
    loopthr->next = thr;

    return 1;
}

int mythreads_yield(){
   // pthread_mutex_lock(&lock);

    //print_chain();
    if(current_thread->next != current_thread){
        
        while(current_thread->next->status != READY){
            current_thread = current_thread->next;
        }
    
        co_t *toThread = &current_thread->next->coroutine;

        current_thread = current_thread->next;

        //pthread_mutex_unlock(&lock);

        mycoroutines_switchto(toThread);

        return 1;
    }else{
        pthread_mutex_unlock(&lock);
        printf("No other thread on the list");
        return -1;
    }
}

void print_chain(){
    
    mythr_t *loop;
    loop = current_thread;

    do{
        printf("%d status: %d --> ",loop->id, loop->status);
        //printf("%d(%d) context: %p, status: %d --> \n",loop->id, loop->sleepTime, &loop->coroutine, loop->status);
        
        loop = loop->next;
    }while(loop != current_thread);
    
    printf("%d\n\n",current_thread->id);
}

void update_sleep(){
    pthread_mutex_lock(&lock);
   
    mythr_t *loop = current_thread;
    do{     
        if(loop->status == SLEEP && loop->sleepTime != 0){
            loop->sleepTime = loop->sleepTime - 1000000/* MIN_SLEEP_CHECK */;
            //printf("\n In update: -> id: %d \n",loop->id);
            printf("\n In update: -> id: %d sleep: %d\n",loop->id,loop->sleepTime);
            if(loop->sleepTime <= 0){
                loop->status = READY;
                loop->sleepTime = 0;
            }
        }
        loop = loop->next;
    }while(loop != current_thread);
   
    pthread_mutex_unlock(&lock);
}

int mythreads_sleep(int secs) {
    pthread_mutex_lock(&lock);

    current_thread->status = SLEEP;
    current_thread->sleepTime = secs * 1000000;

    pthread_mutex_unlock(&lock);
    mythreads_yield();
    
    return 1;
}

int mythreads_join(mythr_t *thr){
    printf("finished: %d\n",thr->finished);
    while(!thr->finished){
        //wait;
        //yeild?
    }
    return 1 ;
}
int mythreads_destroy(mythr_t *thr){
    mythr_t *loop = thr;
    int id = thr->id;

    while(loop->next != thr){
        loop = loop->next;
    }
    loop->next = thr->next;

    mycoroutines_destroy(&thr->coroutine);
    free(thr);

    printf("thread %d destroyed\n",id);
    return 1;
}

int mythreads_sem_create(mysem_t *s, int val){
    s->value=val;
    s->blocked = NULL;
    return 1;
}

int mythreads_sem_down(mysem_t *s){
    pthread_mutex_lock(&lock);
    
    if(!s->value){
        if(s->blocked != NULL){
            mythr_t *loop = s->blocked;
            while(loop->blocked != NULL){
                loop = loop->blocked;
            }
            loop->blocked = current_thread;
        }else{
            s->blocked = current_thread;
        }
        current_thread->status = BLOCK;
        
        pthread_mutex_unlock(&lock);
        mythreads_yield();
    }else{
        s->value--;
    }
    
    pthread_mutex_unlock(&lock);
    return 1;
}

int mythreads_sem_up(mysem_t *s){
    pthread_mutex_lock(&lock);
    

    if(s->value){
        printf("lost call of up in sem\n");
    }else{
        s->value++;
        if(s->blocked!=NULL){
            mythr_t *thr = s->blocked;
            s->blocked = s->blocked->blocked;
            thr->status = READY;
            thr->blocked = NULL;
            s->value--;
        }
    }
    pthread_mutex_unlock(&lock);
    return 1;
}

int mythreads_sem_destroy(mysem_t *s){
    if(s->blocked!= NULL){
        printf("threads still blocked in the semaphore\n");
    }
    free(s);
    printf("semaphore destroyed\n");
    return 1;
}
