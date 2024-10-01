#pragma once

#include <mutex>

namespace JATX
{
	class flag
	{
	private:
		bool internalflag;
		std::mutex flagmtx;
	public:
		flag() : internalflag(false) {};
		~flag() {};
		bool _get()
		{
			bool placeholder;
			this->flagmtx.lock();
			placeholder = this->internalflag;
			this->flagmtx.unlock();
			return placeholder;
		}
		void _on()
		{
			this->flagmtx.lock();
			this->internalflag = true;
			this->flagmtx.unlock();
		}
		void _off()
		{
			this->flagmtx.lock();
			this->internalflag = false;
			this->flagmtx.unlock();
		}
	};
}