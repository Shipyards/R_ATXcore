#pragma once

#include <iostream>
#include "task.h"

class testTask :
	public R_ATX::_task
{
	testTask()
	{

	}
	void _execute()
	{
		std::cout << "yo mama\n" << std::flush;
	}
	~testTask()
	{

	}
};