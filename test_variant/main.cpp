#include <iostream>
#include <string>
#include <boost/variant.hpp>

#include "scene.hpp"

namespace {

	struct base_t {
		uint32_t	frame_;

		base_t() : frame_(0) { }
	};

	base_t base_;

	void select_scene_(int idx);

	class scene0 {
		int	cnt_;

	public:
		void init()
		{
			std::cout << '(' << base_.frame_ << ") ";
			std::cout << "scene0.init()" << std::endl;
			cnt_ = 0;
		}

		void service()
		{
			std::cout << '(' << base_.frame_ << ") ";
			std::cout << "scene0.service()" << std::endl;
			++cnt_;
			if(cnt_ >= 5) {
				select_scene_(1);
			}
		}
	};

	class scene1 {
	public:
		void init()
		{
			std::cout << '(' << base_.frame_ << ") ";
			std::cout << "scene1.init()" << std::endl;
		}

		void service()
		{
			std::cout << '(' << base_.frame_ << ") ";
			std::cout << "scene1.service()" << std::endl;
		}
	};

	class scene2 {
	public:
		void init()
		{
			std::cout << '(' << base_.frame_ << ") ";
			std::cout << "scene2.init()" << std::endl;
		}

		void service()
		{
			std::cout << '(' << base_.frame_ << ") ";
			std::cout << "scene2.service()" << std::endl;
		}
	};

	typedef utils::scene<scene0, scene1, scene2> SCENE;
	SCENE scene_;

	scene0 scene0_;
	scene1 scene1_;
	scene2 scene2_;

	void select_scene_(int idx)
	{
		switch(idx) {
		case 0:
			scene_.change(scene0_);
			break;
		case 1:
			scene_.change(scene1_);
			break;
		case 2:
			scene_.change(scene2_);
			break;
		}
	}

}


int main(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	scene_.change(scene0_);

	for(int i = 0; i < 10; ++i) {
		scene_.service();
		++base_.frame_;
	}
}
