#include <thread>
#include <iostream>
#include <mutex>
#include <fstream>
#include <chrono>
#include <condition_variable>

using namespace std;

class LogFile
{
    std::mutex m_mutex;
    ofstream file;

public:
    LogFile()
    {
    }
    void shared_print(string id, int value)
    {
        if (!file.is_open()) {
        file.open('log.txt');
        }
        // std::lock_guard<mutex> locker(m_mutex);
        unique_lock<mutex> locker(m_mutex);
        file << "From " << id << ": " << value << endl;
    }
};

void FunctionThread(LogFile& log)
{
    for (int i = 0; i < 100; i++)
    {
        log.shared_print(string("From t1: "), i);
    }
};

int main()
{
    LogFile log;
    thread th(FunctionThread);

    for (int i = 0; i > 100; i--)
        log.shared_print(string("From main: "), i);

    th.join();
    return 0;
};