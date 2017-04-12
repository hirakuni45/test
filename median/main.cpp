#include <iostream>
#include <vector>
#include <set>

#include <cstdlib>

namespace {

	typedef std::vector<uint16_t> MEM;

	MEM		mem_;

	typedef std::set<uint16_t> SET;

	SET		set_;

}

int main(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	uint32_t size = 10000;

	mem_.resize(size);
	for(int i = 0; i < size; ++i) {
		mem_[i] = rand();
	}

	uint16_t min = 65535;
	uint16_t max = 0;
	uint32_t ave = 0;
	for(int i = 0; i < size; ++i) {
		if(min > mem_[i]) min = mem_[i];
		if(max < mem_[i]) max = mem_[i];
		ave += mem_[i];
		set_.insert(mem_[i]);
	}
	ave /= size;

	auto s = set_.size();
	auto it = set_.cbegin();
	uint16_t n;
	for(int i = 0; i < (s / 2); ++i) ++it;
	if(s & 1) {
		++it;
		n = *it;
	} else {
		uint32_t nn = *it;
		++it;
		nn += *it;
		n = nn / 2;		
	}

	std::cout << "Size: " << size << std::endl;
	std::cout << "Min: " << static_cast<int>(min) << std::endl;
	std::cout << "Max: " << static_cast<int>(max) << std::endl;
	std::cout << "Average: " << static_cast<int>(ave) << std::endl;
	std::cout << "SET Size: " << s << std::endl;
	std::cout << "SET Median: " << n << std::endl; 
}
