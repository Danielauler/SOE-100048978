#include <thread>
#include <iostream>
#include <mutex>
#include <fstream>
#include <chrono>
#include <condition_variable>

using namespace std;

std::mutex mu;
std::deque<int> q;
std::condition_variable cond;

void FunctionThread1()
{
    int count = 10;
    while (count > 0)
    {
        unique_lock<mutex> locker(mu);
        q.push_front(count);
        locker.unlock();
        cond.notify_one();
        this_thread::sleep_for(chrono::seconds(1));
        count--;
    }
};

void FunctionThread2()
{
    int data = 0;
    while (data != 1)
    {
        unique_lock<mutex> locker(mu);
        cond.wait(locker, [](){ return !q.empty();});
        data = q.back();
        q.pop_back();
        locker.unlock();
        cout << "t2 got a value from t1: " << data << endl;
    }
}

int main()
{
    thread t1(FunctionThread1);
    thread t2(FunctionThread2);
    t1.join();
    t2.join();
    return 0;
};