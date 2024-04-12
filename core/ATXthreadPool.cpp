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

#include "ATXthreadpool.h"

#include <queue>
#include <thread>
#include <mutex>
#include "task.h"

namespace R_ATX
{
    ATXthreadPool::worker_thread(std::queue<_task>* targetQueue, std::mutex* targetQueuemtx, std::condition_variable* targetQueuecv, bool* _taskflag)
}