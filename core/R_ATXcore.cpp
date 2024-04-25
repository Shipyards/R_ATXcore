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

#include "R_ATXcore.h"
#include "task.h"
#include <mutex>
#include <thread>
#include <condition_variable>

namespace R_ATX
{
    R_ATXcore::R_ATXcore()
    {
        this->Tpool = new ATXthreadPool(5, &this->taskQueue, &this->TQm, &this->TQcv, &this->taskflag);
    }
    void R_ATXcore::add_task(_task newtask)
    {
        //lock on to task queue mutex
        { 
            std::lock_guard ulk(this->TQm); 

            //workhere
            this->taskQueue.push(newtask);

            //signal go
            this->taskflag = true;
        }
        this->TQcv.notify_all();
    }
    R_ATXcore::~R_ATXcore()
    {

    }
}