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


int main()
{
    int x;
    promise<int> p;
    future<int> f = p.get_future();
    shared_future<int> sf = f.share();
    future<int> fu = std::async(std::launch::async, factorial, sf);
    future<int> fu2 = std::async(std::launch::async, factorial, sf);
    future<int> fu3 = std::async(std::launch::async, factorial, sf);

    this_thread::sleep_for(chrono::milliseconds(20));
    p.set_value(4);

    return 0;
};