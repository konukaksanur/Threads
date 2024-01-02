#include <stdio.h>
#include <math.h>
#include <pthread.h>

long long int a, b;
int c, d;
double global_sqrt_sum = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    long long int begin;
    long long int end;
} ThreadArguments;

void method1();
double method2();
double method3();
void *calculateMethod1(void* arg);
void* calculateMethod3(void* arg);
void getInputs();

int main() {
    getInputs();
    switch(d) {
        case 1: method1(); break;
        case 2: method2(); break;
        case 3: method3(); break;
        default: fprintf(stderr, "Invalid d.");
    }
    return 0;
}

void getInputs() {
    printf("a = ");
    fflush(stdout);
    scanf("%llu", &a);
    printf("b = ");
    fflush(stdout);
    scanf("%llu", &b);
    printf("c = ");
    fflush(stdout);
    scanf("%d", &c);
    printf("d = ");
    fflush(stdout);
    scanf("%d", &d);
}

void method1() {
    pthread_t threads[c];
    ThreadArguments args[c];
    long long int range = (b - a + 1) / c;
    for (int i = 0; i < c; i++) {
        args[i].begin = a;
        if (i == c - 1) {
            args[i].end = b;
        } else {
            args[i].end = a + range - 1;
        }
        if (pthread_create(
                &threads[i],
                NULL,
                calculateMethod1,
                &args[i]
                )) {
            fprintf(stderr, "Thread couldn't be created.\n");
        }
        a += range;
    }
    for (int i = 0; i < c; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Sum of square roots for method 1 = %lf\n", global_sqrt_sum);
}

void* calculateMethod1(void* arg) {
    ThreadArguments *threadArg1 = (ThreadArguments*) arg;
    for (long long i = threadArg1->begin; i <= threadArg1->end; i++) {
        global_sqrt_sum += sqrt((double)i);
    }
    return NULL;
}

double method2() {
    return 0;
}

double method3() {
    pthread_t threads[c];
    ThreadArguments args[c];
    long long int range = (b - a + 1) / c;
    for (int i = 0; i < c; i++) {
        args[i].begin = a;
        if (i == c - 1) {
            args[i].end = b;
        } else {
            args[i].end = a + range - 1;
        }
        if (pthread_create(&threads[i], NULL, calculateMethod3, &args[i])) {
            fprintf(stderr, "Thread couldn't be created.\n");
        }
        a += range;
    }
    for (int i = 0; i < c; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Sum of square roots for method 3 = %lf\n", global_sqrt_sum);
    return global_sqrt_sum;
}

void* calculateMethod3(void* arg) {
    ThreadArguments *threadArg2 = (ThreadArguments*) arg;
    double local_sqrt_sum = 0;
    for (long long i = threadArg2->begin; i <= threadArg2->end; i++) {
        local_sqrt_sum += sqrt((double)i);
    }
    pthread_mutex_lock(&mutex);
    global_sqrt_sum += local_sqrt_sum;
    pthread_mutex_unlock(&mutex);
    return NULL;
}