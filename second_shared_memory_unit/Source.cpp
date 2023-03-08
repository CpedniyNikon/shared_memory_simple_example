#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <windows.h>
#include <cstring>
#include <cstdlib>
#include <string>
#include <iostream>


int main()
{
	using namespace boost::interprocess;
	shared_memory_object shm(open_only, "MySharedMemory", read_write);
	mapped_region region(shm, read_write);

	volatile int& semaphore = *(int*)region.get_address();
	volatile int& size = *((int*)region.get_address() + static_cast<int>(sizeof(int)));

	while (true) {
		if (semaphore > 0) {
			char* message = reinterpret_cast<char*>((int*)region.get_address() + static_cast<int>(2 * sizeof(int)));
			for (int i = 0; i < size; i++) {
				std::cout << *(message + i);
			}
			std::cout << std::endl;
			semaphore--;
		}
	}

	shared_memory_object::remove("MySharedMemory");
	return 0;
}