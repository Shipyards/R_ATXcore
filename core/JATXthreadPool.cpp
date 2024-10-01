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

#include "pch.h"
#include "JATXthreadPool.hpp"
#include <queue>
#include <thread>
#include <mutex>
#include <iostream>
#include "task.hpp"

namespace JATX
{
    //contructor
    JATXthreadPool::JATXthreadPool(int numofthreads, std::queue<_task*>* targetQueuein, std::mutex* targetQueuemtxin, std::condition_variable* targetQueuecvin, flag* _taskflagin) : threads(new R_jarray<JATXthread*>(numofthreads)), size(numofthreads)
    {
        std::cout << "starting thread pool\n" << std::flush;

        this->_args.taskqueue = targetQueuein;
        this->_args.tqu_mtx = targetQueuemtxin;
        this->_args.tqu_cv = targetQueuecvin;
        this->_args.taskflag = _taskflagin;
        this->_args.active_threads = new ticker();

        int tbuildit;
        for (tbuildit = 0; tbuildit != numofthreads; tbuildit++)
        {
            this->threads->operator[](tbuildit) = new JATXthread(&this->_args);
        }

        //wait until ticker matches {wait until all threads have checked in and are ready to proccess}
        while (this->_args.active_threads->_read() != numofthreads) { std::this_thread::sleep_for(std::chrono::milliseconds(10)); }

        std::cout << "thread pool initalized\n" << std::flush;
    }
    // deconstructor
    JATXthreadPool::~JATXthreadPool()
    {
        delete this->threads;

        //wait until ticker matches {wait until all threads have checked in and are teminated}
        while (this->_args.active_threads->_read() != 0) { std::this_thread::sleep_for(std::chrono::milliseconds(10)); }

        delete this->_args.active_threads;
    }
}