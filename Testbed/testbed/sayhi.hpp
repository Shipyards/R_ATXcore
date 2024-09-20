#pragma once

#include "..\core\task.hpp"

class sayhi :
    public JATX::_task
{
public:
    sayhi();
    ~sayhi();
    void _execute();
};

