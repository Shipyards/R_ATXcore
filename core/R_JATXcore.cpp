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

#define R_JATXcore_EXPORTS

#include "pch.h"
#include "R_JATXcore.hpp"
#include "task.hpp"
#include <mutex>
#include <thread>
#include <condition_variable>

namespace JATX
{
    JATXthreadPool* R_JATXcore::Tpool = nullptr;
    std::map<UID*, _data*> R_JATXcore::dataStack = std::map<UID*, _data*>();
    std::mutex* R_JATXcore::datamtx = new std::mutex();
    std::mutex* R_JATXcore::TQmtx = new std::mutex();
    std::condition_variable* R_JATXcore::TQcv = new std::condition_variable;
    std::queue<_task*> R_JATXcore::taskQueue = std::queue<_task*>();
    flag* R_JATXcore::taskflag = new flag();

    bool R_JATXcore::init_threads(int tnum)
    {
        Tpool = new JATXthreadPool(tnum, &taskQueue, TQmtx, TQcv, taskflag);
        return true;
    }
    bool R_JATXcore::add_task(_task* newtask)
    {
        //std::cout << "adress of new task " << newtask << std::endl;
        //lock on to task queue mutex
        { 
            std::lock_guard<std::mutex> ulk(*TQmtx); 

            //workhere
            taskQueue.push(newtask);

            //signal go
            taskflag->_on();
        }
        TQcv->notify_all();
        return true;
    }
    bool R_JATXcore::add_data(_data* newdata)
    {
        datamtx->lock();
        dataStack[&newdata->localUID] = newdata;
        datamtx->unlock();
        return true;
    }
    _data* R_JATXcore::fetch_data(UID ID)
    {
        std::map<UID*, _data*>::iterator datait;
        for (datait = dataStack.begin(); datait != dataStack.end(); datait++)
        {
            if (*datait->first == ID)
            {
                return datait->second;
            }
        }
    }
    bool R_JATXcore::remove_data(UID ID)
    {
        std::map<UID*, _data*>::iterator datait;
        for (datait = dataStack.begin(); datait != dataStack.end(); datait++)
        {
            if (*datait->first == ID)
            {
                dataStack.erase(datait->first);
                try { delete datait->second; }
                catch (std::exception e) { std::cout << e.what() << std::endl; }
                return true;
            }
        }
    }
    bool R_JATXcore::deinit_threads()
    {
        delete Tpool; // delete threadpool before deleting data

        delete datamtx, TQmtx, TQcv, taskflag;

        return true;
    }

    //functions to interface with the core

    //intialize all the threads on the core
    R_JATXcore_API void core_init(int threadcount)
    {
        R_JATXcore::init_threads(threadcount);
    }
    // Deintialize all the threads on the core
    R_JATXcore_API void core_deinit()
    {
        R_JATXcore::deinit_threads();
    }
    //add a task to the active queue on the core
    R_JATXcore_API void core_addtask(_task* newtask)
    {
        R_JATXcore::add_task(newtask);
    }
    //adds a datapacket to the data stack for global access and serialization
    R_JATXcore_API void core_adddata(_data* newdata)
    {
        R_JATXcore::add_data(newdata);
    }
    //retrwive a datapacket from the data stack
    R_JATXcore_API void core_fetchdata(UID access, _data*& returnptr)
    {
        returnptr = R_JATXcore::fetch_data(access);
    }

}