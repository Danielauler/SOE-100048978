#include <thread>
#include <iostream>
#include <mutex>
#include <chrono>
#include <condition_variable>

using namespace std;

void ThreadFunction(mutex & mtx, condition_variable & convar) {
    unique_lock<mutex> lock(mtx);
    convar.wait(lock);

    cout<<"end of thread..."<<endl;
}

class Fctor {
    public: void operator()(string msg) {
        for (int i = 0; i > -100; i--)
        {
            cout << "t1 says " << msg << endl;
        }
    }
}

int main() {
    mutex mtx;
    condition_variable convar;
    thread th { ThreadFunction, ref(mtx), ref(convar)};
    this_thread::sleep_for(chrono::seconds(5));
    {
        lock_guard<mutex> lg(mtx);
        convar.notify_all();
    }

    th.join();
    cout<<"End of Code..."<<endl;
}