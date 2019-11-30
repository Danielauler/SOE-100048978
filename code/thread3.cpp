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
    string s = "Where there is no trust, there is no love";
    thread th((Fctor()), ref(s));
    cout<<this_thread::get_id()<<endl;
    try
    {
        cout << "from main: " << s << endl;
    }
    catch (...)
    {
        th.join();
        throw;
    }

    th.join();
    cout << "End of Code..." << endl;
    return 0;
}