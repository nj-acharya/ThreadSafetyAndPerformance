// ConsoleApplication3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <chrono>
#include <mutex>

int notthreadsafe_counter = 0;

void notthreadsafe_increment() {
    for (int i = 0; i < 10000; ++i) {
        notthreadsafe_counter++; // not thread-safe increment
    }
}

std::atomic<int> counter(0);

void increment() {
    for (int i = 0; i < 10000; ++i) {
        counter++; // atomic increment
    }
}

std::mutex mutex_;
int mutex_counter = 0;

void mutex_increment() {
    for (int i = 0; i < 10000; ++i) {
        std::lock_guard<std::mutex> lock(mutex_);
        mutex_counter++; // increment with mutex lock
    }
}

int main() {
    std::vector<std::thread> threads;
    for (int i = 0; i < 1000; ++i) {
        threads.emplace_back(notthreadsafe_increment);
    }
    auto start0 = std::chrono::high_resolution_clock::now();

    for (auto& t : threads) {
        t.join();
	}
	auto end0 = std::chrono::high_resolution_clock::now();
	long long milliseconds0 = std::chrono::duration_cast<std::chrono::milliseconds>(end0 - start0).count();
	std::cout << "Final not thread-safe counter: " << notthreadsafe_counter << ", with time in ms: " << milliseconds0 << std::endl;

    threads.clear();
    for (int i = 0; i < 1000; ++i) {
        threads.emplace_back(increment);
    }
    auto start = std::chrono::high_resolution_clock::now();
    for (auto& t : threads) {
        t.join();
    }
    auto end = std::chrono::high_resolution_clock::now();

    long long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    std::cout << "Final counter (atomic): " << counter.load() << ", with time in ms: " << milliseconds << std::endl;

    threads.clear();
    for (int i = 0; i < 1000; ++i) {
        threads.emplace_back(mutex_increment);
	}
	auto start1 = std::chrono::high_resolution_clock::now();
    for (auto& t : threads) {
        t.join();
	}
	auto end1 = std::chrono::high_resolution_clock::now();
	long long milliseconds1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1).count();
	std::cout << "Final mutex counter: " << mutex_counter << ", with time in ms: " << milliseconds1 << std::endl;

    return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
