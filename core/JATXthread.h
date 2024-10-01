#include <thread>
#include <queue>
#include <mutex>
#include "task.hpp"
#include "ticker.hpp"
#include "flag.hpp"

#define T_Type std::thread
#define T_ID std::thread::id

#pragma once

namespace JATX // this class will encapsulate the threads on the native system, uses posix when possible
{
	struct t_args
	{
		std::queue<_task*>* taskqueue;
		std::mutex* tqu_mtx;
		std::condition_variable* tqu_cv;
		flag* taskflag;
		ticker* active_threads;
	};
	class JATXthread
	{
	private:
		t_args* _args;
		bool _killflag;
		T_Type* OS_thread;
		T_ID thread_ID;
	public:
		JATXthread(t_args*);
		~JATXthread();
	};
}


