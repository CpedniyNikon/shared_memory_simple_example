#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <cstring>
#include <cstdlib>
#include <string>
#include <iostream>


int main(int argc, char* argv[])
{
	using namespace boost::interprocess;
	shared_memory_object::remove("MySharedMemory");

	shared_memory_object shm(create_only, "MySharedMemory", read_write);
	shm.truncate(10);

	mapped_region region(shm, read_write);

	volatile int& semaphore = *(int*)region.get_address();
	volatile int& size = *((int*)(region.get_address()) + static_cast<int>(sizeof(int)));
	while (true) {
		std::string message;
		std::getline(std::cin, message);
		memcpy((int*)region.get_address() + static_cast<int>(2 * sizeof(int)), message.c_str(), message.size());
		size = message.size();
		semaphore++;
	}

	return 0;
}