#include <iostream>

#include "format.hpp"


		static uint32_t get_words(const char* src, char sch = ' ') noexcept
		{
			if(src == nullptr) return 0;

			uint32_t n = 0;
			char bc = sch;
			bool bsc = false;
			while(1) {
				char ch = *src++;
				if(ch == '\\') {
					bsc = true;
					continue;
				} else if(bsc) {
					bsc = false;
				} else {
					if(ch == 0) break;
					if(bc == sch && ch != sch) { 
						++n;
					}
					bc = ch;
				}
			}
			return n;
		}


		static bool get_word(const char* src, uint32_t argc, char* dst, uint32_t dstlen,
			char sch = ' ') noexcept 
		{
			if(src == nullptr || dst == nullptr || dstlen == 0) return false;

			uint32_t n = 0;
			char bc = sch;
			bool bsc = false;
			bool out = false;
			while(1) {
				char ch = *src++;
				if(ch == '\\') {
					bsc = true;
					continue;
				} else if(bsc) {
					bsc = false;
				} else {
					if(bc == sch && ch != sch) {
						if(n == argc) {
							out = true;
						}
						++n;
					} else if(bc != sch && ch == sch) {
						if(out) {
							break;
						}
					}
					if(ch == 0) break;
					bc = ch;
				}
				if(out && dstlen > 1) {
					*dst++ = ch;
					dstlen--;
				}
			}
			*dst = 0;
			return out;
		}


		static bool cmp_word(const char* src, uint32_t argc, const char* key, char sch = ' ') noexcept
		{
			if(src == nullptr || key == nullptr) return false;

			uint32_t n = 0;
			char bc = sch;
			bool bsc = false;
			bool out = false;
			while(1) {
				char ch = *src++;
				if(ch == '\\') {
					bsc = true;
					continue;
				} else if(bsc) {
					bsc = false;
				} else {
					if(bc == sch && ch != sch) {
						if(n == argc) {
							out = true;
						}
						++n;
					} else if(bc != sch && ch == sch) {
						if(out) {
							break;
						}
					}
					if(ch == 0) break;
					bc = ch;
				}
				if(out) {
					if(ch != *key) return false;
					++key; 
				}
			}
			if(*key == 0) return true;
			else return false;
		}


// 速度検査
// #define SPEED_TEST

int main(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	{
		const char* src = "";
		auto n = get_words(src);
		utils::format("words: '%s': %u\n\n") % src % n;

		char dst[256];
		n = 0;
		get_word(src, n, dst, sizeof(dst));
		utils::format("getw: '%s': %u\n\n") % src % n;
	}

	{
		const char* src = "aaa bbb ccc";
		auto n = get_words(src);
		utils::format("words: '%s': %u\n") % src % n;

		char dst[256];
		n = 0;
		get_word(src, n, dst, sizeof(dst));
		utils::format("getw: %d: '%s'\n") % n % dst;

		n = 1;
		get_word(src, n, dst, sizeof(dst));
		utils::format("getw: %d: '%s'\n") % n % dst;

		n = 2;
		get_word(src, n, dst, sizeof(dst));
		utils::format("getw: %d: '%s'\n\n") % n % dst;
	}

	{
		const char* src = "aaa bbb\\ ccc ddd";
		auto n = get_words(src);
		utils::format("words: '%s': %u\n") % src % n;

		char dst[256];
		n = 0;
		get_word(src, n, dst, sizeof(dst));
		utils::format("getw: %d: '%s'\n") % n % dst;

		n = 1;
		get_word(src, n, dst, sizeof(dst));
		utils::format("getw: %d: '%s'\n") % n % dst;

		n = 2;
		get_word(src, n, dst, sizeof(dst));
		utils::format("getw: %d: '%s'\n\n") % n % dst;
	}

	{
		const char* src = "aaa bbb\\ ccc ddd";
		int f0 = cmp_word(src, 1, "bbb cc");
		int f1 = cmp_word(src, 1, "bbb ccc");
		int f2 = cmp_word(src, 1, "bbb cccc");
		utils::format("cmp: %d, %d, %d\n") % f0 % f1 % f2;
	}
}
