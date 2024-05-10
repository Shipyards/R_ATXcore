#pragma once

#include <iostream>
#include "task.hpp"

class testTask :
	public R_ATX::_task
{
public:
	int* i;
	testTask(int* i)
	{
		this->i = i;
		(*this->i)++;
	}
	char* _serialize() { return "1"; };
	void _deserialize(char* raw) {};
	void _execute()
	{
		std::cout << "yo mama\n" << *this->i << std::flush;
	}
	~testTask()
	{
		std::cout << "bye from task\n" << std::flush;
	}
};