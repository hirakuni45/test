#pragma once
//=====================================================================//
/*!	@file
	@brief	Widget クラス
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2019 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RX/blob/master/LICENSE
*/
//=====================================================================//
#include "vtx.hpp"
// #include "graphics/color.hpp"

namespace gui {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief	Widget クラス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	class widget {
	public:


//		typedef graphics::def_color DEF_COLOR;

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief	ステート
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		enum class STATE : uint8_t {
			DISABLE,	///< 無効状態
			ENABLE,		///< 有効
			STALL,		///< 表示されているが、ストール状態
		};

	private:
		widget*		parents_;	///< 親 widget

		vtx::srect	location_;	///< 位置とサイズ

		const char*	title_;		///< タイトル

		STATE		state_;		///< 状態
		bool		focus_;		///< フォーカスされている場合「true」
		bool		touch_;		///< タッチされている場合「true」

	public:
		//-----------------------------------------------------------------//
		/*!
			@brief	コンストラクター
			@param[in]	loc		ロケーション
			@param[in]	title	タイトル
		*/
		//-----------------------------------------------------------------//
		widget(const vtx::srect& loc = vtx::srect(0), const char* title = nullptr) noexcept :
			parents_(nullptr),
			location_(loc),
			title_(title),
			state_(STATE::DISABLE), focus_(false), touch_(false)
		{ } 


		//-----------------------------------------------------------------//
		/*!
			@brief	コンストラクター
			@param[in]	size	サイズ
			@param[in]	title	タイトル
		*/
		//-----------------------------------------------------------------//
		widget(const vtx::spos& size, const char* title = nullptr) noexcept :
			parents_(nullptr),
			location_(vtx::spos(0), size),
			title_(title),
			state_(STATE::DISABLE), focus_(false), touch_(false)
		{ }


		virtual const char* name() const = 0;


		//-----------------------------------------------------------------//
		/*!
			@brief	Widget 型を取得
			@return Widget 型に対する整数
		*/
		//-----------------------------------------------------------------//
		virtual uint32_t type() const = 0; 



		void set_parents(widget* w) { parents_ = w; }


		//-----------------------------------------------------------------//
		/*!
			@brief	ロケーションの取得
			@return ロケーション
		*/
		//-----------------------------------------------------------------//
		const vtx::srect& get_location() const noexcept { return location_; }


		//-----------------------------------------------------------------//
		/*!
			@brief	ロケーションの参照
			@return ロケーション
		*/
		//-----------------------------------------------------------------//
		vtx::srect& at_location() noexcept { return location_; }


		void set_title(const char* title) noexcept { title_ = title; }


		const char* get_title() const noexcept { return title_; }
	};
}

extern bool insert_widget(gui::widget* w);
extern void remove_widget(gui::widget* w);
