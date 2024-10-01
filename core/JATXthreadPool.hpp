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

#ifdef R_JATXcore_EXPORTS
#define R_JATXcore_API __declspec(dllexport)
#else
#define R_JATXcore_API __declspec(dllimport)
#endif

#pragma once

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "task.hpp"
#include "JATXthread.h"
#include "jarray.hpp"
#include "flag.hpp"

namespace JATX
{
    /*R_JATXcore_API*/ class JATXthreadPool
    {
    private:
        R_jarray<JATXthread*>* threads;
        int size;
    public:
        //arguements of the threads
        t_args _args;
        //constructor
        JATXthreadPool(int, std::queue<_task*>*, std::mutex*, std::condition_variable*, flag*);
        //deconstructor
        ~JATXthreadPool();
    };
}