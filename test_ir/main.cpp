#include <iostream>
#include <boost/format.hpp>

#include "ir_send.hpp"

namespace {

	uint32_t	nnn_ = 0;

	class out_func {
	public:
		void operator () (bool f) {
			std::cout << boost::format("%d") % static_cast<int>(f);
			++nnn_;
			if(nnn_ >= 4) {
				std::cout << ' ';
				nnn_ = 0;
			}
		}
	};


	typedef chip::ir_send<out_func> IR_SEND;
	IR_SEND		ir_send_;
}

int main(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	ir_send_.send_data(0x45);

	for(int i = 0; i < 200; ++i) {
		ir_send_.service();
	}
	std::cout << std::endl;
}
