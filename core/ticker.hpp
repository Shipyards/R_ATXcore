#pragma once

#include <mutex>

namespace JATX
{
	class ticker
	{
	private:
		int ticks;
		std::mutex mtx;
	public:
		ticker() : ticks(0)
		{

		}
		void operator++()
		{
			this->mtx.lock();
			this->ticks++;
			this->mtx.unlock();
		}
		void operator--()
		{
			this->mtx.lock();
			this->ticks--;
			this->mtx.unlock();
		}
		int _get()
		{
			int placeholder;
			this->mtx.lock();
			placeholder = this->ticks;
			this->mtx.unlock();
			return placeholder;
		}
	};
}