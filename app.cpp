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


// Real-time thread function
void realTimeTask(int id) {
    activeThreads++;  // Increment the active thread count

    while (!stopThreads) {
        TaskType task = getRandomTask();
        performTask(task, id);

        // Lock and print status
        {
            lock_guard<mutex> lock(mtx);
            cout << "Thread " << id << " is executing a task." << endl;
        }

        // Sleep before the next task
        this_thread::sleep_for(chrono::milliseconds(200));
    }

    // Log thread stopping
    {
        lock_guard<mutex> lock(mtx);
        cout << "Thread " << id << " has stopped." << endl;
        logMessage("Thread " + to_string(id) + " has stopped.");
    }

    activeThreads--;  // Decrement the active thread count
}

// Controller function to manage threads
void controller(vector<thread>& threads, int numThreads) {
    cout << "Starting " << numThreads << " threads..." << endl;
    logMessage("Starting " + to_string(numThreads) + " threads.");

    // Create and start threads
    for (int i = 0; i < numThreads; ++i) {
        threads.push_back(thread(realTimeTask, i + 1));
    }

    cout << "Threads are running. Type 'stop' to terminate or 'status' to check active threads." << endl;
    logMessage("Threads are running.");

    // Monitor user commands
    string command;
    while (true) {
        getline(cin, command);

        if (command == "stop") {
            stopThreads = true;
            cv.notify_all();
            break;
        } else if (command == "status") {
            cout << "Active Threads: " << activeThreads << endl;
            logMessage("Active Threads: " + to_string(activeThreads));
        } else if (command == "add") {
            threads.push_back(thread(realTimeTask, threads.size() + 1));
            cout << "Added a new thread. Total threads: " << threads.size() << endl;
            logMessage("Added a new thread. Total threads: " + to_string(threads.size()));
        } else if (command == "remove") {
            if (!threads.empty()) {
                stopThreads = true;
                threads.back().join();
                threads.pop_back();
                stopThreads = false;
                cout << "Removed one thread. Total threads: " << threads.size() << endl;
                logMessage("Removed one thread. Total threads: " + to_string(threads.size()));
            } else {
                cout << "No threads to remove!" << endl;
            }
        } else {
            cout << "Unknown command. Use 'stop', 'status', 'add', or 'remove'." << endl;
        }
    }

    // Join all threads before exiting
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    cout << "All threads stopped." << endl;
    logMessage("All threads stopped.");
}


// Main function
int main() {
    int numThreads;
    srand(time(0));  // Seed for random task generation

    cout << "Enter the number of threads to simulate: ";
    cin >> numThreads;
    cin.ignore();

    vector<thread> threads;
    controller(threads, numThreads);

    return 0;
}

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
