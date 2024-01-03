//Aksanur KONUK 150120049
//Duru BAŞTUNALI 150120075
//CSE 3033 - PROJECT 3

#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <stdlib.h>

long long int a, b; //Ending and starting points
int c, d; //Thread count & method
double global_sqrt_sum = 0; //Sum of square roots
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //Mutex variable

void method1(); //Create threads for method 1
void method2(); //Create threads for method 2
void method3(); //Create threads for method 3
void *sqrtSumMethod1(void *threadID); //Calculate sum of square roots for method 1
void *sqrtSumMethod2(void *threadID); //Calculate sum of square roots for method 2
void *sqrtSumMethod3(void *threadID); //Calculate sum of square roots for method 3

int main(int argc, char *argv[]) {
    if (argc < 5) { //Check if there are 4 arguments
        fprintf(stderr, "Please enter 4 parameters!\n");
        return 1;
    }

    //Set the global variables with the given arguments
    a = atoll(argv[1]);
    b = atoll(argv[2]);
    c = atoi(argv[3]);
    d = atoi(argv[4]);

    if (a >= b) { //Check if the starting point is less than the ending point
        fprintf(stderr, "Starting point should be lower than ending point.\n");
        exit(0);
    }

    if (a < 0 || b < 0 || c < 0 || d < 0) { //Check if any negative values entered
        fprintf(stderr, "Arguments should be positive.\n");
        exit(0);
    }

    //Run the method based on the given d argument
    switch(d) {
        case 1: method1(); break;
        case 2: method2(); break;
        case 3: method3(); break;
        default: fprintf(stderr, "Invalid d."); //Check if the d value is anything other than 1, 2, 3
    }
    return 0;
}

void method1() { //Method 1
    pthread_t threads[c]; //All threads
    for(int i = 0; i < c; i++){
        int *id = malloc(sizeof(int));
        *id = i; //Thread ID by its order
        if(pthread_create(
                threads + i, //New thread
                NULL, //Initialize the thread as null
                sqrtSumMethod1, //Set thread routine as method 1
                id)){ //Pass the ID as argument to the routine
            fprintf(stderr, "Failed to create thread.\n"); //Check if thread creation failed
        }
    }
    for (int i = 0; i < c; i++) { //Join all the thread results
        pthread_join(threads[i], NULL);
    }
    printf("Sum: %.5e\n", global_sqrt_sum); //Display the sum
}

void method2() { //Method 2
    pthread_t threads[c]; //All threads
    for(int i = 0; i < c; i++){
        int *id = malloc(sizeof(int));
        *id = i; //Thread ID by its order
        if(pthread_create(
                threads + i, //New thread
                NULL, //Initialize the thread as null
                sqrtSumMethod2, //Set thread routine as method 2
                id)){ //Pass the ID as argument to the routine
            fprintf(stderr, "Failed to create thread.\n"); //Check if thread creation failed
        }
    }
    for (int i = 0; i < c; i++) { //Join all the thread results
        pthread_join(threads[i], NULL);
    }
    printf("Sum: %.5e\n", global_sqrt_sum); //Display the sum
}

void method3() { //Method 3
    pthread_t threads[c]; //All threads
    for(int i = 0; i < c; i++){
        int *id = malloc(sizeof(int));
        *id = i; //Thread ID by its order
        if(pthread_create(
                threads + i, //New thread
                NULL, //Initialize the thread as null
                sqrtSumMethod3, //Set thread routine as method 3
                id)){ //Pass the ID as argument to the routine
            fprintf(stderr, "Failed to create thread.\n"); //Check if thread creation failed
        }
    }
    for (int i = 0; i < c; i++) { //Join all the thread results
        pthread_join(threads[i], NULL);
    }
    printf("Sum: %.5e\n", global_sqrt_sum); //Display the sum
}

void* sqrtSumMethod1(void *threadID) { //Calculate the sum of square roots for method 1
    int threadIDint = *(int*)threadID; //Cast the thread ID as int
    long long int range = (b - a + 1) / c ; //Set the range for each thread (almost equally)
    long long start = a + (threadIDint * range); //Set the starting point
    long long int end; //Declare the ending point
    if(threadIDint == c - 1){ //If the last range is processed, ending should be b
        end = b;
    } else {
        end = start + range  - 1; //If not, compute a mid-ending based on starting point and intervals
    }
    for(long long int i = start ; i <= end  ; i++){ //Compute all in global_sqrt_sum
        global_sqrt_sum += sqrt((double)i);
    }
    return NULL;
}

void *sqrtSumMethod2(void *threadID) { //Calculate the sum of square roots for method 2
    int threadIDint = *(int*)threadID; //Cast the thread ID as int
    long long int range = (b - a + 1) / c; //Set the range for each thread (almost equally)
    long long start = a + (threadIDint * range); //Set the starting point
    long long int end; //Declare the ending point
    if(threadIDint == c - 1) { //If the last range is processed, ending should be b
        end = b;
    } else { //If not, compute a mid-ending based on the starting point and intervals
        end = start + range  - 1;
    }
    for(long long int i = start ; i <= end  ; i++){ //Compute all in secured global_sqrt_sum
        pthread_mutex_lock(&mutex); //Mutex for the global_sqrt_sum to define the critical section
        global_sqrt_sum += sqrt((double)i);
        pthread_mutex_unlock(&mutex); //End the critical section
    }
    return NULL;
}

void *sqrtSumMethod3(void *threadID) { //Calculate the sum of square roots for method 3
    double local_sqrt_sum = 0; //Declare a local_sqrt_sum variable for local computation for each thread
    int threadIDint = *(int*)threadID; //Cast the thread ID as int
    long long int range = (b - a + 1) / c; //Set the range for each thread (almost equally)
    long long start = a + (threadIDint * range); //Set the starting point
    long long int end; //Declare the ending point
    if(threadIDint == c -1) { //If the last range is processed, ending should be b
        end = b;
    } else { //If not, compute a mid-ending based on the starting point and intervals
        end = start + range  - 1;
    }
    for(long long int i = start ; i <= end  ; i++) { //Compute all in local_sqrt_sum
        local_sqrt_sum += sqrt((double)i);
    }
    pthread_mutex_lock(&mutex); //Mutex for global_sqrt_sum to define the critical section
    global_sqrt_sum += local_sqrt_sum; //Add the local result of each thread to the secured global_sqrt_sum
    pthread_mutex_unlock(&mutex); //End the critical section
    return NULL;
}