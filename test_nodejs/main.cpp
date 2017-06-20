#include "format.hpp"
#include "file_io.hpp"

int main(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	const char* first = nullptr;
	const char* last  = nullptr;

	for(int i = 1; i < argc; ++i) {
		const char* p = argv[i];
		first = last;
		last  = p;
	}
	if(first == nullptr && last != nullptr) std::swap(first, last);

	auto cmd = utils::get_file_base(argv[0]);

	if(first == nullptr) {
		utils::format("SEEDA03 A/D data checker\n");
		utils::format("    %s first-file [last-file]\n") % cmd.c_str();
		return 0;
	}


	utils::format("First file name: '%s'") % first;
	if(last != nullptr) {
		utils::format(" to Last File name: '%s'") % last;
	}
	utils::format("\n");

	const char* fname = first;

	utils::file_io fio;
	if(fio.open(fname, "rb")) {
		uint32_t n = 0;
		std::string mark;
		while(!fio.eof()) {
			auto line = fio.get_line();
			if(n == 0) mark = line;
			else {

			}
			++n;
//			utils::format("%s\n") % line.c_str();
		}
		fio.close();

		utils::format("CSV Type: %s\n") % mark.c_str();
		utils::format("Sample data num: %u\n") % (n - 1);

	} else {
		utils::format("Can't open input file: '%s'\n") % fname;
	}

}
