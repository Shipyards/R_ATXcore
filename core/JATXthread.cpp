#include "JATXthread.h"

namespace JATX
{
    //thread function
    void worker_thread(t_args* args, bool* _killflag)
    {
        _task* _activetask;
        bool q_emtpy = true;

        args->active_threads->operator++();

        while (!*_killflag)
        {
            {
                //std::cout << "waiting\n" << std::flush;

                //check if queue is empty
                args->tqu_mtx->lock();
                q_emtpy = args->taskqueue->empty();
                args->tqu_mtx->unlock();

                std::unique_lock<std::mutex> lk(*(args->tqu_mtx));

                //wait unless the queue isn't empty
                if (q_emtpy) 
                {
                    args->tqu_cv->wait(lk, [&] { return (args->taskflag->_get()) || (*_killflag); });
                }

                //std::cout << " done waiting" << std::flush;

                //checks if the target is empty or the kill flag is on
                if (args->taskqueue->empty() || (*_killflag)) { continue; }

                //grab task
                _activetask = (args->taskqueue->front());

                //std::cout << " new task"<< std::flush;

                //pop task
                args->taskqueue->pop();

                //turn off flag
                args->taskflag->_off();
            }
            //lock gone, task aquired

            //execute
            try { _activetask->_execute(); }
            catch (std::exception e) { std::cout << e.what() << std::endl; }

            try { delete _activetask; /*std::cout << "task deleted\n";*/ }
            catch (std::exception e) { std::cout << e.what() << std::endl; };

            //done
        }

        args->active_threads->operator--();
    }

    JATXthread::JATXthread(t_args *argsin) : _killflag(false), _args(argsin)
    {
        this->OS_thread = new std::thread(worker_thread, this->_args, &this->_killflag);

        this->thread_ID = this->OS_thread->get_id();

        std::cout << this->thread_ID << "init\n" << std::flush;
    }

    JATXthread::~JATXthread()
    {
        this->_killflag = true;

        this->_args->tqu_cv->notify_all();

        this->OS_thread->join();

        delete this->OS_thread, _killflag;

        std::cout << this->thread_ID << "deinit\n" << std::flush;
    }
}