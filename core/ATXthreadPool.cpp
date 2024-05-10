/*
   Copyright 2024 Jacob T. Ray

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "ATXthreadPool.hpp"
#include <queue>
#include <thread>
#include <mutex>
#include <iostream>
#include "task.hpp"




namespace R_ATX
{
    void test(int b)
    {
        std::cout << "hi\n" << std::flush;
    }
    //thread function
    void worker_thread(std::queue<_task*>* targetQueue, std::mutex* targetQueuemtx, std::condition_variable* targetQueuecv, bool* _taskflag, bool* _killflag)
    {
        std::thread::id ID = std::this_thread::get_id();
        _task* _activetask;

        std::cout << ID << " initalized\n" << std::flush;

        while(!*_killflag)
        {
            {
                //std::cout << "waiting\n" << std::flush;

                //wait
                std::unique_lock<std::mutex> lk(*targetQueuemtx);
                targetQueuecv->wait(lk, [&]{ return *_taskflag || *_killflag; });

                //std::cout << "done waiting\n" << std::flush;

                //checks if the target is empty or the kill flag is on
                if(targetQueue->empty() || *_killflag) {continue;}

                //grab task
                _activetask = targetQueue->front();
                //pop task
                targetQueue->pop();

                //turn off flag
                *_taskflag = false;
            }
            //lock gone, task aquired

            //execute
            try { _activetask->_execute(); }
            catch(std::exception e) { }

            try { delete _activetask; std::cout << "task deleted\n"; }
            catch (std::exception e) { std::cout << e.what() << std::endl; };

            //done
        }
        std::cout << ID << " terminated\n" << std::flush;
    }
    //contructor
    ATXthreadPool::ATXthreadPool(int numofthreads, std::queue<_task*>* targetQueuein, std::mutex* targetQueuemtxin, std::condition_variable* targetQueuecvin, bool* _taskflagin)
    {
        std::cout << "begining thread core\n" << std::flush;

        this->_killflag = false;

        int tbuildit;
        for (tbuildit = 0; tbuildit != numofthreads; tbuildit++)
        {
            this->threads.push_back(new std::thread(worker_thread, targetQueuein, targetQueuemtxin, targetQueuecvin, _taskflagin, &this->_killflag));
        }

        this->_qcv = targetQueuecvin;
    }
    // deconstructor
    ATXthreadPool::~ATXthreadPool()
    {
        this->_killflag = true;

        this->_qcv->notify_all();

        std::vector<std::thread*>::iterator tdeconi;
        std::thread* holder;
        for(tdeconi = this->threads.begin(); tdeconi != this->threads.end(); tdeconi++)
        {
            holder = *tdeconi;
            holder->join();
        }
    }
}