#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <stdlib.h>

long long int a, b;
int c, d;
double global_sqrt_sum = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void method1();
void method2();
void method3();
void *calculateMethod1(void *threadID);
void *calculateMethod2(void *threadID);
void *calculateMethod3(void *threadID);

int main(int argc, char *argv[]) {
    if (argc < 5) {
        fprintf(stderr, "Please enter 4 parameters!\n");
        return 1;
    }

    if (a >= b) {
        fprintf(stderr, "Starting point should be lower than ending point.\n");
        exit(0);
    }

    if (a < 0 || b < 0 || c < 0 || d < 0) {
        fprintf(stderr, "Arguments should be positive.\n");
        exit(0);
    }

    a = atoll(argv[1]);
    b = atoll(argv[2]);
    c = atoi(argv[3]);
    d = atoi(argv[4]);

    switch(d) {
        case 1: method1(); break;
        case 2: method2(); break;
        case 3: method3(); break;
        default: fprintf(stderr, "Invalid d.");
    }
    return 0;
}

void method1() {
    pthread_t threads[c];
    for(int i = 0; i < c; i++){
        int *id = malloc(sizeof(int));
        *id = i;
        if(pthread_create(
                threads + i,
                NULL,
                calculateMethod1,
                id)){
            fprintf(stderr, "Failed to create thread.\n");
        }
    }
    for (int i = 0; i < c; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Sum: %.5e\n", global_sqrt_sum);
}

void method2() {
    pthread_t threads[c];
    for(int i = 0; i < c; i++){
        int *id = malloc(sizeof(int));
        *id = i;
        if(pthread_create(
                threads + i,
                NULL,
                calculateMethod2,
                id)){
            fprintf(stderr, "Failed to create thread.\n");
        }
    }
    for (int i = 0; i < c; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Sum: %.5e\n", global_sqrt_sum);
}

void method3() {
    pthread_t threads[c];
    for(int i = 0; i < c; i++){
        int *id = malloc(sizeof(int));
        *id = i;
        if(pthread_create(
                threads + i,
                NULL,
                calculateMethod3,
                id)){
            fprintf(stderr, "Failed to create thread.\n");
        }
    }
    for (int i = 0; i < c; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Sum: %.5e\n", global_sqrt_sum);
}

void* calculateMethod1(void *threadID) {
    int threadIDint = *(int*)threadID;
    long long int range = (b - a + 1) / c ;
    long long start = a + (threadIDint * range);
    long long int end;
    if(threadIDint == c -1){
        end = b;
    } else{
        end = start + range  - 1;
    }
    for(long long int i = start ; i <= end  ; i++){
        global_sqrt_sum += sqrt((double)i);
    }
}

void *calculateMethod2(void *threadID) {
    int threadIDint = *(int*)threadID;
    long long int range = (b - a + 1) / c ;
    long long start = a + (threadIDint * range);
    long long int end;
    if(threadIDint == c -1){
        end = b;
    } else{
        end = start + range  - 1;
    }
    pthread_mutex_lock(&mutex);
    for(long long int i = start ; i <= end  ; i++){
        global_sqrt_sum += sqrt((double)i);
    }
    pthread_mutex_unlock(&mutex);
}

void *calculateMethod3(void *threadID) {
    double local_sqrt_sum = 0;
    int threadIDint = *(int*)threadID;
    long long int range = (b - a + 1) / c ;
    long long start = a + (threadIDint * range);
    long long int end;
    if(threadIDint == c -1){
        end = b;
    } else {
        end = start + range  - 1;
    }
    for(long long int i = start ; i <= end  ; i++){
        local_sqrt_sum += sqrt((double)i);
    }
    pthread_mutex_lock(&mutex);
    global_sqrt_sum += local_sqrt_sum;
    pthread_mutex_unlock(&mutex);
}