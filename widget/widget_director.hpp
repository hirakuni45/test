#pragma once
//=====================================================================//
/*!	@file
	@brief	Widget ディレクター・クラス
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2019 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RX/blob/master/LICENSE
*/
//=====================================================================//
#include "widget.hpp"

namespace gui {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief	Widget クラス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <uint32_t NUM>
	class widget_director {
	public:
		typedef std::array<widget*, NUM> WIDGETS; 

	private:
		WIDGETS	widgets_;


	public:
		widget_director() : widgets_{ nullptr }
		{ }


		bool insert(widget* wi)
		{
			for(auto& w : widgets_) {
				if(w == nullptr) {
					w = wi;
					break;
				}
			}
			return true;
		}


		void remove(widget* wi)
		{
			for(auto& w : widgets_) {
				if(w == wi) {
					w = nullptr;
					break;
				}
			}	
		}


		void list_all()
		{
			for(auto& w : widgets_) {
				if(w != nullptr) {
					utils::format("%s: %08X\n") % w->name() % w->type();
				}
			}
		}
	};
}
