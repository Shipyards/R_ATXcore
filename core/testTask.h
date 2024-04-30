#pragma once

#include <iostream>
#include "task.h"

class testTask :
	public R_ATX::_task
{
public:
	testTask()
	{

	}
	char* _serialize() {};
	void _deserialize(char* raw) {};
	void _execute()
	{
		std::cout << "yo mama\n" << std::flush;
	}
	~testTask()
	{
		std::cout << "bye from task\n" << std::flush;
	}
};