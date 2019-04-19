#pragma once
#include "widget.hpp"

namespace gui {

	class button : public widget {
	public:

		typedef button value_type;


		button() {
			insert_widget(this);
		}


		virtual ~button() { remove_widget(this); }


		const char* name() const override { return "Button"; }

		//-----------------------------------------------------------------//
		/*!
			@brief	型を取得
		*/
		//-----------------------------------------------------------------//
		uint32_t type() const override { return (size_t)name(); }





	};
}
