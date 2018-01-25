#pragma once
//=====================================================================//
/*!	@file
	@brief	Fixed MAP (固定長 MAP) テンプレート
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2018 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RX/blob/master/LICENSE
*/
//=====================================================================//
#include <cstdint>

namespace utils {

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
    /*!
        @brief  fixed median クラス
		@param[in]	T		基本形
		@param[in]	SIZE	バッファサイズ（最大６５５３６）
    */
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <typename T, uint32_t SIZE>
	class fixed_map {

		struct core_t {
			T	value;
			T	count;
		};

		core_t		core_[SIZE];
		uint32_t	size_;


		void move_(uint32_t org)
		{
			if(org >= SIZE) return;

			T i = size_;
			while(i > org) {
				core_[i] = core_[i - 1];
				--i;
			}
		}

	public:

		fixed_map() : size_(0) { }


		uint32_t size() const { return size_; }


		void clear() { size_ = 0; }


		bool add(T value)
		{
			if(size_ >= SIZE) return false;

			uint32_t i = 0;
			while(i < size_) {
				if(value < core_[i].value) {
					move_(i);
					break;
				} else if(value == core_[i].value) {
					++core_[i].count;
					return true;
				}
				++i;
			}
			core_[i].value = value;
			core_[i].count = 1;
			++size_;
			return false;
		}


		T get_value(uint32_t idx) const { return core_[idx].value; }
		T get_count(uint32_t idx) const { return core_[idx].count; }

#if 0
		void list()
		{
			for(T i = 0; i < size_; ++i) {
			   std::cout << i << " V: " << core_[i].value << ", C: " << core_[i].count << std::endl;
			}
		}
#endif
	};
}
