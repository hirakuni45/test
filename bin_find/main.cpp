#include <string>
#include <vector>
#include "utils/format.hpp"
#include "utils/input.hpp"
#include "utils/string_utils.hpp"
#include "utils/file_io.hpp"

namespace {

	static const uint32_t VERSION = 75;

	uint32_t dump_length_ = 0;
	uint32_t dump_block_ = 1;
	std::string dump_form_text_;
	bool report_all_ = false;

	void help_(const char* cmd) {
		auto c = utils::get_file_base(cmd, true);
		utils::format("Find binary data (match for binary,text phrase)\n");
		utils::format("Version %d.%02d\n") % (VERSION / 100) % (VERSION % 100);
		utils::format("usage:\n");
		utils::format("    %s [options] input-file\n") % c.c_str();
		utils::format("    -report-all\n");
		utils::format("    -text text-phrase\n");
		utils::format("    -byte byte-phrase\n");
		utils::format("    -l16 little-endian-16-phrase\n");
		utils::format("    -b16 big-endian-16-phrase\n");
		utils::format("    -l32 little-endian-32-phrase\n");
		utils::format("    -b32 big-endian-32-phrase\n");
		utils::format("    -dump-length length             (Optional dump length)\n");
		utils::format("    -dump-block block               (Optional dump block)\n");
		utils::format("    -dump-form format               (Ascii, Byte, Word(16), Long(32))\n");
		utils::format("    -v verbose\n");
		utils::format("\n");
	}


	static uint32_t get_l16_(const void* top) {
		auto p = static_cast<const uint8_t*>(top);
		uint32_t val = 0;
		val  = static_cast<uint32_t>(p[0]);
		val |= static_cast<uint32_t>(p[1]) << 8;
		return val;
	}


	static uint32_t get_b16_(const void* top) {
		auto p = static_cast<const uint8_t*>(top);
		uint32_t val = 0;
		val  = static_cast<uint32_t>(p[1]);
		val |= static_cast<uint32_t>(p[0]) << 8;
		return val;
	}


	static uint32_t get_l32_(const void* top) {
		auto p = static_cast<const uint8_t*>(top);
		uint32_t val = 0;
		val  = static_cast<uint32_t>(p[0]);
		val |= static_cast<uint32_t>(p[1]) << 8;
		val |= static_cast<uint32_t>(p[2]) << 16;
		val |= static_cast<uint32_t>(p[3]) << 24;
		return val;
	}


	static uint32_t get_b32_(const void* top) {
		auto p = static_cast<const uint8_t*>(top);
		uint32_t val = 0;
		val  = static_cast<uint32_t>(p[3]);
		val |= static_cast<uint32_t>(p[2]) << 8;
		val |= static_cast<uint32_t>(p[1]) << 16;
		val |= static_cast<uint32_t>(p[0]) << 24;
		return val;
	}


	enum class phrase_type {
		none,
		text,
		byte,
		l16,
		l32,
		b16,
		b32
	};

	struct phrase_t {
		phrase_type	type;
		std::string value;
		phrase_t() : type(phrase_type::none), value() { }
		phrase_t(phrase_type t, const std::string& v) : type(t), value(v) { }
	};
	typedef std::vector<phrase_t> PHRASE;

	struct bin_t {
		phrase_type	type;
		uint32_t	value;
		bin_t() : type(phrase_type::none), value(0) { }
		bin_t(phrase_type t, uint32_t v) : type(t), value(v) { }
	};

	typedef std::vector<bin_t> BIN;
	bool parse_value_(const phrase_t& t, BIN& bin)
	{
		if(t.type == phrase_type::none || t.type == phrase_type::text) {
			for(auto ch : t.value) {
				bin.emplace_back(phrase_type::text, ch);
			}
		} else {
			auto ss = utils::split_text(t.value, " ,");
			for(auto s : ss) {
				uint32_t v = 0;
				if(!(utils::input("%a", s.c_str()) % v).status()) {
					return false;
				}
				bin.emplace_back(t.type, v);
			}
		}
		return true;
	}


	uint32_t total_bytes_(const BIN& bin)
	{
		uint32_t num = 0;

		for(auto b : bin) {
			switch(b.type) {
			case phrase_type::l16:
			case phrase_type::b16:
				num += 2;
				break;
			case phrase_type::l32:
			case phrase_type::b32:
				num += 4;
				break;

			case phrase_type::byte:
			case phrase_type::text:
			default:
				num += 1;
				break;
			}
		}

		return num;
	}


	void get_data_(const void* src, const BIN& bin, BIN& cmp)
	{
		auto p = static_cast<const uint8_t*>(src);
		for(auto b : bin) {
			switch(b.type) {
			case phrase_type::l16:
				cmp.emplace_back(b.type, get_l16_(p));
				p += 2;
				break;
			case phrase_type::b16:
				cmp.emplace_back(b.type, get_b16_(p));
				p += 2;
				break;
			case phrase_type::l32:
				cmp.emplace_back(b.type, get_l32_(p));
				p += 4;
				break;
			case phrase_type::b32:
				cmp.emplace_back(b.type, get_b32_(p));
				p += 4;
				break;
			case phrase_type::byte:
			case phrase_type::text:
			default:
				cmp.emplace_back(b.type, p[0]);
				p += 1;
				break;
			}
		}
	}


	bool cmp_data_(const BIN& bin, const BIN& cmp)
	{
		if(bin.size() != cmp.size()) return false;

		for(uint32_t i = 0; i < bin.size(); ++i) {
			if(bin[i].value != cmp[i].value) return false;
		}
		return true;
	}


	void dump_(const void* src, uint32_t num)
	{
		auto p = static_cast<const uint8_t*>(src);
		uint32_t n = 0;
		for(uint32_t i = 0; i < num; ++i) {
			utils::format(" %02X") % p[i];
			++n;
			if(n >= 16) {
				utils::format("\n");
				n = 0;
			}
		}
		if(n != 0) {
			utils::format("\n");
		}
	}


	bool scan_(utils::file_io& inp, const PHRASE& phrase, bool verbose)
	{
		BIN bin;
		for(auto t : phrase) {
			if(!parse_value_(t, bin)) {
				utils::format("Error: Illegual phrase: %s\n") % t.value.c_str();
				return false;
			}
		}

		if(bin.size() == 0) {
			utils::format("Error: Empty phrase text\n");
			return false;
		}

		uint32_t num = total_bytes_(bin);

		if(verbose) {
			utils::format("Find phrase (%d bytes): ") % num;
			for(auto b : bin) {
				switch(b.type) {
				case phrase_type::byte:
					utils::format("B:0x%02X ") % b.value;
					break;
				case phrase_type::l16:
					utils::format("L16:0x%04X ") % b.value;
					break;
				case phrase_type::l32:
					utils::format("L32:0x%08X ") % b.value;
					break;
				case phrase_type::b16:
					utils::format("B16:0x%04X ") % b.value;
					break;
				case phrase_type::b32:
					utils::format("B32:0x%08X ") % b.value;
					break;
				case phrase_type::text:
				default:
					utils::format("A:'%c' ") % b.value;
					break;
				}
			}
			utils::format("\n");
		}

		utils::array_uc array;
		if(!utils::read_array(inp, array)) {
			utils::format("Can't read data: '%s'\n") % inp.get_path().c_str();
			return false;
		}
		if(verbose) {
			utils::format("Input file size: %u (%s)\n") % array.size() % inp.get_path().c_str();
			utils::format("\n");
		}

		for(size_t pos = 0; pos < (array.size() - num); ++pos) {
			BIN cmp;
			get_data_(&array[pos], bin, cmp);
			if(cmp_data_(bin, cmp)) {
				auto bks = std::max(num, dump_length_);
				for(uint32_t i = 0; i < dump_block_; ++i) {
					utils::format("%08X:") % (pos + i * bks);
					dump_(&array[pos + i * bks], bks);
				}
				utils::format("\n");
				if(!report_all_) break;
			}
		}

		return true;
	}
}


int main(int argc, char *argv[])
{
	const char* inp_file = nullptr;

	bool error = false;
	bool verbose = false;
	auto pt = phrase_type::none;
	PHRASE phrase;
	bool dump_length = false;
	bool dump_block = false;
	bool dump_form = false;
	for(int i = 1; i < argc; ++i) {
		if(argv[i][0] == '-') {
			std::string opt = argv[i];
			if(opt == "-v" || opt == "--verbose") {
				verbose = true;
			} else if(opt == "-report-all") {
				report_all_ = true;
			} else if(opt == "-text") {
				pt = phrase_type::text;
			} else if(opt == "-byte") {
				pt = phrase_type::byte;
			} else if(opt == "-l16") {
				pt = phrase_type::l16;
			} else if(opt == "-l32") {
				pt = phrase_type::l32;
			} else if(opt == "-b16") {
				pt = phrase_type::b16;
			} else if(opt == "-b32") {
				pt = phrase_type::b32;
			} else if(opt == "-dump-length") {
				dump_length = true;
			} else if(opt == "-dump-block") {
				dump_block = true;
			} else if(opt == "-dump-form") {
				dump_form = true;
			} else {
				utils::format("Illegual options: '%s'\n") % opt.c_str();
				error = true;
			}
		} else {
			if(pt != phrase_type::none) {
				phrase.emplace_back(pt, argv[i]);
				pt = phrase_type::none;
			} else if(dump_length) {
				if(!(utils::input("%a", argv[i]) % dump_length_).status()) {
					utils::format("Parse value fail for dump length: '%s'\n") % argv[i];
				}
				dump_length = false;
			} else if(dump_block) {
				if(!(utils::input("%a", argv[i]) % dump_block_).status()) {
					utils::format("Parse value fail for dump block: '%s'\n") % argv[i];
				}
				dump_block = false;
			} else if(dump_form) {
				dump_form_text_ = argv[i];
				dump_form = false;
			} else {
				inp_file = argv[i];
			}
		}
	}

	if(inp_file == nullptr || error) {
		help_(argv[0]);
		return 0;
	}

	if(verbose) {
		utils::format("Input file: '%s'\n") % inp_file;

		utils::format("Report all: %s\n") % (report_all_ ? "Enable" : "Disable");

		for(auto t : phrase) {
			std::string ptt = "text";
			switch(t.type) {
			case phrase_type::byte: ptt = "byte"; break;
			case phrase_type::l16:  ptt = "l16";  break;
			case phrase_type::l32:  ptt = "l32";  break;
			case phrase_type::b16:  ptt = "b16";  break;
			case phrase_type::b32:  ptt = "b32";  break;
			default:
				break;
			}
			utils::format("Phrase %s: [%s]\n") % ptt.c_str() % t.value.c_str();
		}
		if(dump_length_ > 0) {
			utils::format("Dump optional length: %u\n") % dump_length_;
		}
		if(dump_block_ > 1) {
			utils::format("Dump optional block: %u\n") % dump_block_;
		}
	}

	utils::file_io inp;
	if(!inp.open(inp_file, "rb")) {
		utils::format("Can't open input file: %s\n") % inp_file;
		return -1;
	}

	auto ret = scan_(inp, phrase, verbose);

	inp.close();

	return ret ? 0 : -1;
}
