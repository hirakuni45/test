#include <iostream>
#include <boost/format.hpp>

#include "ir_send.hpp"
#include "ir_recv.hpp"

// リモコン、NEC フレーム送信

namespace {

	uint32_t	nnn_ = 0;

	bool		inp_level_;

	class out_func {
	public:
		void operator () (bool f) {
			std::cout << boost::format("%d") % static_cast<int>(f);
			++nnn_;
			if(nnn_ >= 4) {
				std::cout << ' ';
				nnn_ = 0;
			}
			inp_level_ = f;
		}
	};


	typedef chip::ir_send<out_func> IR_SEND;
	IR_SEND		ir_send_;


	class inp_func {
	public:
		bool operator() () {
			return inp_level_;
		}
	};

	typedef chip::ir_recv<inp_func> IR_RECV;
	IR_RECV		ir_recv_;


}

int main(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	ir_send_.send_data(0x45);

	for(int i = 0; i < 200; ++i) {
		ir_send_.service();

		ir_recv_.service();
		ir_recv_.service();
	}
	std::cout << std::endl;

	int fc = ir_recv_.get_frame_count();
	std::cout << boost::format("Frame count: %d") % fc << std::endl;
	auto cc = ir_recv_.get_custom_code();
	std::cout << boost::format("Custom Code: %04X") % cc << std::endl;
	int ud = ir_recv_.get_user_data();
	std::cout << boost::format("User data: %02X") % ud << std::endl;
}
