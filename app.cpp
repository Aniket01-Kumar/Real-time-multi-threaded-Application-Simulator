#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
#include <atomic>
#include <condition_variable>
#include <fstream>
#include <string>
#include <random>
#include <sstream>

using namespace std;

// Global variables and mutex for synchronization
mutex mtx;
condition_variable cv;
atomic<bool> stopThreads(false);  // Flag to stop threads
atomic<int> activeThreads(0);     // Counter for active threads

// Enum for task types
enum TaskType {
    COMPUTATION,
    IO_OPERATION,
    DATA_PROCESSING,
    IDLE
};

// Function to generate random task type
TaskType getRandomTask() {
    return static_cast<TaskType>(rand() % 4);
}

// Function to log messages to a file
void logMessage(const string& message) {
    lock_guard<mutex> lock(mtx);
    ofstream logFile("thread_log.txt", ios::app);
    if (logFile.is_open()) {
        logFile << message << endl;
        logFile.close();
    }
}

// Function to simulate task execution
void performTask(TaskType task, int id) {
    string taskName;
    int duration = 100; // Default task duration (milliseconds)

    switch (task) {
        case COMPUTATION:
            taskName = "Computation";
            duration = 300;
            break;
        case IO_OPERATION:
            taskName = "I/O Operation";
            duration = 500;
            break;
        case DATA_PROCESSING:
            taskName = "Data Processing";
            duration = 700;
            break;
        case IDLE:
            taskName = "Idle";
            duration = 200;
            break;
    }

    // Log task start
    stringstream ss;
    ss << "Thread " << id << " started " << taskName << " task.";
    logMessage(ss.str());

    // Simulate task duration
    this_thread::sleep_for(chrono::milliseconds(duration));

    // Log task completion
    ss.str("");
    ss << "Thread " << id << " completed " << taskName << " task.";
    logMessage(ss.str());
}
