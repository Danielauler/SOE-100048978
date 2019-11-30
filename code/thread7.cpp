#include <thread>
#include <iostream>
#include <mutex>
#include <fstream>
#include <chrono>
#include <condition_variable>
#include <future>


using namespace std;

int factorial(future<int>& f)
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
    future<int> fu = std::async(std::launch::async, factorial, std::ref(f));

    this_thread::sleep_for(chrono::milliseconds(20));
    p.set_value(4);

    x = fu.get();
    cout<<"Get from child: "<<x<<endl;
    return 0;
};