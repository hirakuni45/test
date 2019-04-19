#include <iostream>

#include "format.hpp"

#include "button.hpp"
#include "frame.hpp"
#include "check.hpp"

#include "widget_director.hpp"

namespace {


	typedef gui::widget_director<32> WD;
	WD		wd_;

}

bool insert_widget(gui::widget* w)
{
	return wd_.insert(w);
}

void remove_widget(gui::widget* w)
{
	wd_.remove(w);
}

int main(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	using namespace gui;

// 一つのフレームに、ボタンとチェックボックスがある GUI を想定する。

//	auto root = frame() + button() + button() + check();
// コンテキストをコピーしなければならないので、このやり方は却下。

// 以前に作成した Widget System では、Widget の生成は、マネージャーが行い、
// アプリ側は、そのポインターを保持して、個別の操作を行っていた。
// しかし、その方法は、Widget 部品の実態はマネージャーが握っており、
// 管理が乱雑になる。

// Widget 部品コンテキストの管理を自分で行い、
// 参照を受け取り登録する事が出来る。
// GUI の管理では、「実態」は、傍に置いて管理する方が作りやすい。
	frame root;
	button bt;
	check ck;
	button bt2;
	root + bt + ck + bt2;


	utils::format("\n");

	wd_.list_all();
   


	// 構造の表示
}
