#include "sayhi.hpp"

sayhi::sayhi()
{
	//std::cout << "sayhi initialized" << std::endl;
}
sayhi::~sayhi()
{
	//std::cout << "sayhi deconstructed" << std::endl;
}
void sayhi::_execute()
{
	std::cout << "hello" << std::flush;
}