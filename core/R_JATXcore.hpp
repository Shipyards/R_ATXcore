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

#pragma once

#include <vector>
#include <map>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "data.hpp"
#include "task.hpp"
#include "JATXthreadPool.hpp"

namespace JATX
{
    class R_JATXcore
    {
    private:
        JATXthreadPool* Tpool;
    public:
        //array with all data & gaurds for easy acess and save/load
        std::map<UID*, _data*> dataStack;
        //datastack mutex
        std::mutex datamtx;

        //queue with all tasks to be completed by threadpool
        std::queue<_task*> taskQueue;
        //queue mutex
        std::mutex TQm;
        //condition variable for the queue
        std::condition_variable TQcv;
        //flag for task ready
        bool taskflag;


        //add a task to the task stack and notify the thread pool
        bool add_task(_task*);

        //add data to the stack
        bool add_data(_data*);
    
        //fetch data from the stack
        _data* fetch_data(UID);

        //destroy data on the stack
        bool remove_data(UID);
        
        //constructor
        R_JATXcore(int);
        //deconstructor
        ~R_JATXcore();
    };
}