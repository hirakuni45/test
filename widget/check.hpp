#pragma once
#include "widget.hpp"

namespace gui {

	class check : public widget {
	public:

		typedef button value_type;


		check() {
			insert_widget(this);
		}


		virtual ~check() { remove_widget(this); }


		const char* name() const override { return "Check"; }

		//-----------------------------------------------------------------//
		/*!
			@brief	型を取得
		*/
		//-----------------------------------------------------------------//
		uint32_t type() const override { return (size_t)name(); } 




	};
}
