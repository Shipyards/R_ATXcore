// test

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex m;
std::condition_variable cv;
bool goflag = false;
bool runningflag = true;

void worker_thread()
    {
        while(runningflag)
        {
            {
                std::cout << "waiting\n";
                //wait
                std::unique_lock lk(m);
                cv.wait(lk, [&]{ return goflag; });

                std::cout << "this is where i would grab task\n";

                goflag = false;
            }
            //lock gone?
            std::cout << "lock gone\n";
        }
    }

int main()
{
    using namespace std;
    cout << "test" << endl;

    thread t1(worker_thread), t2(worker_thread), t3(worker_thread);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        std::lock_guard<std::mutex> lk(m);
        std::cout << "Notifying...\n";
    }
    cv.notify_all();
 
    std::this_thread::sleep_for(std::chrono::seconds(1));
 
    {
        std::lock_guard<std::mutex> lk(m);
        goflag = true;
        std::cout << "Notifying again...\n";
    }
    cv.notify_all();

    t1.join();
    t2.join();
    t3.join();
}