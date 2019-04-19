#pragma once
#include "widget.hpp"

#include "format.hpp"

namespace gui {

	struct frame : public widget {

		typedef button value_type;

		frame() {
			insert_widget(this);
		}

		virtual ~frame() { remove_widget(this); }


		const char* name() const override { return "Frame"; }

		//-----------------------------------------------------------------//
		/*!
			@brief	型を取得
		*/
		//-----------------------------------------------------------------//
		uint32_t type() const override { return (size_t)name(); } 


		template <class T>
		frame& operator + (T& th)
		{
			th.set_parents(this);

			utils::format("frame: + %s\n") % th.name();

			return *this;
		}

		template <class T>
		frame& operator += (T& th)
		{
			th.set_parents(this);

			utils::format("frame: += %s\n") % th.name();

			return *this;
		}

	};
}
