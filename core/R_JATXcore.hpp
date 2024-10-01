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

#ifdef R_JATXCORE_EXPORTS
#pragma message("MATHLIBEXPT is defined")
#define R_JATXcore_API __declspec(dllexport)
#else
#define R_JATXcore_API __declspec(dllimport)
#endif

#pragma once

#include <vector>
#include <map>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "data.hpp"
#include "pch.h"
#include "framework.h"
#include "task.hpp"
#include "JATXthreadPool.hpp"

namespace JATX
{
    /*R_JATXcore_API*/ class R_JATXcore
    {
    private:
        static JATXthreadPool* Tpool;
    public:
        //array with all data & gaurds for easy acess and save/load
        static std::map<UID*, _data*> dataStack;
        //datastack mutex
        static std::mutex* datamtx;

        //queue with all tasks to be completed by threadpool
        static std::queue<_task*> taskQueue;
        //queue mutex
        static std::mutex* TQmtx;
        //condition variable for the queue
        static std::condition_variable* TQcv;
        //flag for task ready
        static flag* taskflag;


        static bool init_threads(int);

        static bool deinit_threads();

        //add a task to the task stack and notify the thread pool
        static bool add_task(_task*);

        //add data to the stack
        static bool add_data(_data*);
    
        //fetch data from the stack
        static _data* fetch_data(UID);

        //destroy data on the stack
        static bool remove_data(UID);
        
        ////constructor
        //R_JATXcore(int);
        ////deconstructor
        //~R_JATXcore();
    };

    //functions to access the class
    
    //initalize the core with its threads
    extern "C" R_JATXcore_API void core_init(int);

    //deinitalize the core and join its threads
    extern "C" R_JATXcore_API void core_deinit();

    //add a task to the core
    extern "C" R_JATXcore_API void core_addtask(_task*);

    //add a datapack to the datastack
    extern "C" R_JATXcore_API void core_adddata(_data*);

    //retreive a datapack from the stack
    extern "C" R_JATXcore_API void core_fetchdata(UID, _data*&);
}