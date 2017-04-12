#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>

namespace {

	typedef std::vector<uint16_t> MEM;

	MEM		mem_;

	typedef std::map<uint16_t, uint16_t> MAP;

	MAP		map_;

}

int main(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	uint32_t size = 10000;

	mem_.resize(size);
	for(int i = 0; i < size; ++i) {
		mem_[i] = rand();
	}

	// std::map「median」
	uint16_t min = 65535;
	uint16_t max = 0;
	uint32_t ave = 0;
	for(int i = 0; i < size; ++i) {
		if(min > mem_[i]) min = mem_[i];
		if(max < mem_[i]) max = mem_[i];
		ave += mem_[i];
		auto ret = map_.emplace(mem_[i], 1);
		if(!ret.second) {
			auto& v = ret.first;
			++v->second;
		}
	}
	ave /= size;
	std::cout << "Map size: " << map_.size() << std::endl;

	int sum = 0;
	int med = 0;
	for(MAP::iterator it = map_.begin(); it != map_.end(); ++it) {
		int cnt = it->second;
		sum += cnt;
		if(sum >= (size / 2)) {
			med = it->first;
			if((size & 1) == 0) {
				++it;
				med += it->first;
				med /= 2;
			}
			break;
		}
	}
	std::cout << "Total: " << sum << std::endl;


	// 通常の「median」
	std::sort(mem_.begin(), mem_.end());
	int sa = mem_[(size / 2) - 1];
	if((size & 1) == 0) {
		sa += mem_[(size / 2)];
		sa /= 2;
	}
	std::cout << "Median: " << sa << std::endl;


	std::cout << "Size: " << size << std::endl;
	std::cout << "Min: " << static_cast<int>(min) << std::endl;
	std::cout << "Max: " << static_cast<int>(max) << std::endl;
	std::cout << "Average: " << static_cast<int>(ave) << std::endl;
	std::cout << "SET Median: " << med << std::endl;



}
