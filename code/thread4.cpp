#include <thread>
#include <iostream>
#include <mutex>
#include <chrono>
#include <condition_variable>

using namespace std;

class Fctor
{
public:
    void operator()(string & msg)
    {
        cout << "t1 says " << msg << endl;
    }
};

int main()
{
    thread::hardware_concurrency();
    cout << "End of Code..."<< thread::hardware_concurrency() << endl;
    return 0;
}