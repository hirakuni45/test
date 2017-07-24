//=====================================================================//
/*! @file
    @brief  CSV ファイル検証
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2017 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RX/blob/master/LICENSE
*/
//=====================================================================//
#include "format.hpp"
#include "input.hpp"
#include "file_io.hpp"
#include <ctime>

namespace {

	bool time_date_(const std::string& date, const std::string& time, time_t& tt)
	{
		int d[3];
		if((utils::input("%d/%d/%d", date.c_str()) % d[0] % d[1] % d[2]).num() != 3) {
			return false;
		}

		int t[3];
		if((utils::input("%d:%d:%d", time.c_str()) % t[0] % t[1] % t[2]).num() != 3) {
			return false;
		}

		struct tm m;
		if(d[0] >= 1900 && d[0] < 2100) m.tm_year = d[0] - 1900; else return false;
		if(d[1] >= 1 && d[1] <= 12) m.tm_mon = d[1] - 1; else return false;
		if(d[2] >= 1 && d[2] <= 31) m.tm_mday = d[2]; else return false;
		if(t[0] >= 0 && t[0] < 24) m.tm_hour = t[0]; else return false;
		if(t[1] >= 0 && t[1] < 60) m.tm_min  = t[1]; else return false;
		if(t[2] >= 0 && t[2] < 60) m.tm_sec  = t[2]; else return false;

		tt = mktime(&m);
		return true;
	}


	bool check_file_(const std::string& fname, uint32_t& num)
	{
		utils::file_io fio;
		if(!fio.open(fname.c_str(), "rb")) {
			utils::format("Can't open input file: '%s'\n") % fname.c_str();
			return false;
		}

		uint32_t n = 0;
		std::string header;
		uint32_t header_num = 0;
		time_t first_time = 0;

		uint32_t error = 0;

		while(!fio.eof()) {
			auto line = fio.get_line();
			if(line.empty()) continue;
			auto ss = utils::split_text(line, ",");
			++n;
			if(n == 1) {
				header = line;
				header_num = ss.size();
			} else {
				if(header_num != ss.size()) {
					utils::format("CSV Header/Data num NG(%d -> %d): (%d)%s\n")
						% header_num % ss.size() % n % line.c_str();
					++error;
				}

				// 連続時間検査
				time_t tt;
				if(!time_date_(ss[0], ss[1], tt)) {
					utils::format("CSV Date/Time Conversion NG: (%d)%s\n") % n % line.c_str();
					++error;
				}
				if(n == 2) {
					first_time = tt;
				} else if((first_time + n - 2) != tt) {
					utils::format("CSV Date/Time Lost: (%d)%s\n") % n % line.c_str();
				}
			}
//			utils::format("%s\n") % line.c_str();
		}
		fio.close();

		num = n - 1;

///		utils::format("CSV Type: %s\n") % header.c_str();
///		utils::format("Sample data num: %u\n") % (n - 1);
		return error == 0;
	}
}


int main(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	std::string first;
	std::string last;

	bool verbose = false; 
	for(int i = 1; i < argc; ++i) {
		std::string p = argv[i];
		if(p == "-v" || p == "-V") verbose = true;
		else {
			first = last;
			last  = p;
		}
	}
	if(first.empty() && !last.empty()) std::swap(first, last);

	auto cmd = utils::get_file_base(argv[0]);

	if(first.empty()) {
		utils::format("SEEDA03 A/D data checker\n");
		utils::format("    -v      verbose\n");
		utils::format("    -W      create WAV file\n");
		utils::format("    %s first-file [last-file]\n") % cmd.c_str();
		return 0;
	}

	if(verbose) {
		if(!last.empty()) {
			utils::format("First file name: '%s'") % first.c_str();
			utils::format(" to Last file name: '%s'\n") % last.c_str();
		} else {
			utils::format("File name: '%s'\n") % first.c_str();
		}
	}

	std::string fname = first;
	while(fname != last) {
		uint32_t num;
		if(!check_file_(fname, num)) {
			return -1;
		}
		if(verbose) {
			utils::format("CSV OK (%d data): '%s'\n") % num % fname.c_str();
		}
		if(last.empty()) break;
//		auto nb = org_name.substr(org_name.size() - 6);
//		utils::input(
	}
}
