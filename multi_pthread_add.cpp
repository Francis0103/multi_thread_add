#include <pthread.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <thread>

#define ARR_SIZE 800000
#define NUM_PTHREAD 16

int array_random[ARR_SIZE];

void* multiThreadSum(void* tid) {
    // std::cout << "Hello pthread " << *(int*)tid << std::endl;
    int len = ARR_SIZE / NUM_PTHREAD;
    int start_index = len * *(int*)tid;
    int sum = 0;
    for (int i = 0; i < len; i++) {
       sum += array_random[start_index + i];
    }
    *(int*)tid = sum;
    pthread_exit(nullptr);
}

void sayHello(int i) {
    std::cout << "Hello std::thread!" << std::endl;
}

int main(int argc, char** argv) {
    std::thread t(sayHello, 7);
    std::cout << "max thread at this computer: " << t.hardware_concurrency << std::endl;

    clock_t start, end;
    for (int i = 0; i < ARR_SIZE; i++) {
        array_random[i] = rand() % 20;
    }
    int sum = 0;
    start = clock();
    for (int i = 0; i < ARR_SIZE; i++) {
        sum += array_random[i];
    }
    end = clock();
    std::cout << "time in serial code: " << double(end - start) / CLOCKS_PER_SEC << std::endl;
    std::cout << "sum in main: " << sum << std::endl;

    int index[NUM_PTHREAD];
    pthread_t tid[NUM_PTHREAD];
    start = clock();
    for (int i = 0; i < NUM_PTHREAD; i++) {
        index[i] = i;
        int ret = pthread_create(&tid[i], nullptr, multiThreadSum, (void*)&index[i]);
        if (ret != 0) {
            std::cout << "Error create thread!" << std::endl;
            exit(-1);
        }
    }
    
    void* status;
    for (int i = 0; i < NUM_PTHREAD; i++) {
        pthread_join(tid[i], &status);
    }
    end = clock();
    std::cout << "time in parallel code: " << double(end - start) / CLOCKS_PER_SEC << std::endl;
    
    //pthread_exit(nullptr);
    std::cout << "sum in multi thread: ";
    int tmp_sum = 0;
    for (int i = 0; i < NUM_PTHREAD; i++) {
        tmp_sum += index[i];
    }
    std::cout << tmp_sum << std::endl;
    if (tmp_sum != sum) {
        std::cout << "Multi thread Error!" << std::endl;
    } else {
        std::cout << "OK Done!" << std::endl;
    }
    return 1;
}