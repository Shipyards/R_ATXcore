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

#include "R_ATXcore.hpp"
#include "task.hpp"
#include <mutex>
#include <thread>
#include <condition_variable>

namespace R_ATX
{
    R_ATXcore::R_ATXcore(int tnum)
    {
        this->taskflag = false;
        this->Tpool = new ATXthreadPool(tnum, &this->taskQueue, &this->TQm, &this->TQcv, &this->taskflag);
    }
    bool R_ATXcore::add_task(_task* newtask)
    {
        //lock on to task queue mutex
        { 
            std::lock_guard<std::mutex> ulk(this->TQm); 

            //workhere
            this->taskQueue.push(newtask);

            //signal go
            this->taskflag = true;
        }
        this->TQcv.notify_all();
        return true;
    }
    bool R_ATXcore::add_data(_data* newdata)
    {
        this->datamtx.lock();
        this->dataStack[&newdata->localUID] = newdata;
        this->datamtx.unlock();
        return true;
    }
    _data* R_ATXcore::fetch_data(UID ID)
    {
        std::map<UID*, _data*>::iterator datait;
        for (datait = this->dataStack.begin(); datait != this->dataStack.end(); datait++)
        {
            if (*datait->first == ID)
            {
                return datait->second;
            }
        }
    }
    bool R_ATXcore::remove_data(UID ID)
    {
        std::map<UID*, _data*>::iterator datait;
        for (datait = this->dataStack.begin(); datait != this->dataStack.end(); datait++)
        {
            if (*datait->first == ID)
            {
                this->dataStack.erase(datait->first);
                try { delete datait->second; }
                catch (std::exception e) { std::cout << e.what() << std::endl; }
                return true;
            }
        }
    }
    R_ATXcore::~R_ATXcore()
    {
        delete this->Tpool; // delete threadpool before deleting data
    }
}