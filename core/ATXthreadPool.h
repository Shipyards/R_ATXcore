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

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "task.h"

namespace R_ATX
{
    class ATXthreadPool
    {
    private:
        std::vector<std::thread*> threads;
    public:
        // this is the function that will run in the threads
        //static void worker_thread(std::queue<_task>*, std::mutex*, std::condition_variable*, bool*, bool*);
        //kill flag
        bool _killflag;
        //constructor
        ATXthreadPool(int, std::queue<_task>*, std::mutex*, std::condition_variable*, bool*);
        //deconstructor
        ~ATXthreadPool();
    };
}