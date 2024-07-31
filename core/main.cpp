// test


#include "R_JATXcore.hpp"
#include "testTask.hpp"

#include <iostream>

JATX::R_JATXcore* core = new JATX::R_JATXcore(7);

int main()
{
	using namespace JATX;
	using namespace std;

	cout << "hi world" << endl;

	this_thread::sleep_for(chrono::seconds(1));

	int i = 0;

	//test bed for new / delete
	testTask* test0 = new testTask(&i);
	test0->_execute();
	delete test0;


	//add task
	testTask* test1 = new testTask(&i);
	core->add_task(test1);
	this_thread::sleep_for(chrono::seconds(1));
	testTask* test2 = new testTask(&i);
	core->add_task(test2);
	this_thread::sleep_for(chrono::seconds(1));
	testTask* test3 = new testTask(&i);
	core->add_task(test3);

	this_thread::sleep_for(std::chrono::seconds(1));

	delete core;
}


//
//#include <iostream>
//#include <thread>
//#include <mutex>
//#include <condition_variable>
//
//std::mutex m;
//std::condition_variable cv;
//bool runningflag = true;
//bool goflag = false;
//int i = 0;
//
//void worker_thread()
//    {
//        while(runningflag)
//        {
//            {
//                //wait
//                std::unique_lock lk(m);
//                cv.wait(lk, [&]{ return goflag || !runningflag; });
//
//                if(!runningflag) {continue;}
//
//                i = i + 2;
//                std::cout << "added 2 to i\n";
//
//                goflag = false;
//            }
//        }
//        std::cout << i << " dead\n";
//    }
//
//int main()
//{
//    using namespace std;
//
//    thread t1(worker_thread), t2(worker_thread), t3(worker_thread);
//
//    this_thread::sleep_for(std::chrono::seconds(1));
//    
//    {
//        std::lock_guard<std::mutex> lk(m);
//        goflag = true;
//        std::cout << "Notifying...\n";
//    }
//    cv.notify_one();
// 
//    this_thread::sleep_for(std::chrono::seconds(1));
// 
//    {
//        std::lock_guard<std::mutex> lk(m);
//        goflag = true;
//        std::cout << "Notifying again...\n";
//    }
//    cv.notify_one();
//
//    this_thread::sleep_for(std::chrono::seconds(2));
//    runningflag = false;
//    cv.notify_all();
//
//    t1.join();
//    t2.join();
//    t3.join();
//
//    this_thread::sleep_for(std::chrono::seconds(1));
//}
