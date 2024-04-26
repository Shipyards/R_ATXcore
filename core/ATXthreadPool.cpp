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

#include "ATXthreadPool.h"

#include <queue>
#include <thread>
#include <mutex>
#include <iostream>
#include "task.h"

namespace R_ATX
{
    //thread function
    void ATXthreadPool::worker_thread(std::queue<_task>* targetQueue, std::mutex* targetQueuemtx, std::condition_variable* targetQueuecv, bool* _taskflag, bool* _killflag)
    {
        _task _activetask;
        while(!*_killflag)
        {
            {
                std::cout << "waiting\n";
                //wait
                std::unique_lock lk(*targetQueuemtx);
                targetQueuecv->wait(lk, [&]{ return *_taskflag || *_killflag; });

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
            { try{ _activetask._execute(); } 
            catch(std::exception e) { } }

            //done
        }
        std::cout << std::this_thread::get_id() << "terminated\n";
    }
    //contructor
    ATXthreadPool::ATXthreadPool(int numofthreads, std::queue<_task>* targetQueuein, std::mutex* targetQueuemtxin, std::condition_variable* targetQueuecvin, bool* _taskflagin)
    {
        this->_killflag = false;

        int tbuildit;
        for (tbuildit = 0; tbuildit != numofthreads; tbuildit++)
        {
            this->threads.push_back(new std::thread(worker_thread, targetQueuein, targetQueuemtxin, targetQueuecvin, _taskflagin, this->_killflag));
        }
    }
    // deconstructor
    ATXthreadPool::~ATXthreadPool()
    {
        this->_killflag = true;

        std::vector<std::thread*>::iterator tdeconi;
        std::thread* holder;
        for(tdeconi = this->threads.begin(); tdeconi != this->threads.end(); tdeconi++)
        {
            holder = *tdeconi;
            holder->join();
        }
    }
}