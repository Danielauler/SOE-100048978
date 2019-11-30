#include <thread>
#include <iostream>
#include <mutex>
#include <fstream>
#include <chrono>
#include <condition_variable>
#include <future>

using namespace std;

int factorial(shared_future<int> f)
{
    int res = 1;

    int N = f.get();
    for (int i=N; i>1; i--)
        res *=i;
    
    cout<<"Result is: "<<res<<endl;
    return res;
};

std::deque<packaged_task<int()> > task_q;
std::mutex mu;
std::condition_variable cond;

void thread_1() {
    packaged_task<int()> t;
    {
        unique_lock<mutex> locker(mu);
        cond.wait(locker, [](){ return !task_q.empty();});
        t = move(task_q.front());
        task_q.pop_front();
    }
    t();
};

int main()
{
    thread t1(thread_1);
    packaged_task<int()> t(bind(factorial, 6));
    future<int> fu = t.get_future();
    {
        lock_guard<mutex> locker(mu);
        task_q.push_back(move(t));
    }
    cond.notify_one();

    cout<<fu.get();
    t1.join();
    return 0;
};