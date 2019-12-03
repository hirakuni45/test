#include "utils/format.hpp"
#include "utils/string_utils.hpp"
#include "utils/file_io.hpp"

#include "psp_adpcm.hpp"

#include "snd_io/pcm.hpp"
#include "snd_io/wav_io.hpp"

namespace {

	void help_(const char* cmd) {
		auto c = utils::get_file_base(cmd, true);
		utils::format("PSP Sound data converter (*.PHD, *.PBD to WAV)\n");
		utils::format("usage:\n");
		utils::format("    %s [options] input-file [output-file]\n") % c.c_str();
		utils::format("\n");
	}


	uint32_t get32_(const void* top) {
		auto p = static_cast<const uint8_t*>(top);
		uint32_t val = 0;
		val  = static_cast<uint32_t>(p[0]);
		val |= static_cast<uint32_t>(p[1]) << 8;
		val |= static_cast<uint32_t>(p[2]) << 16;
		val |= static_cast<uint32_t>(p[3]) << 24;
		return val;
	}


	bool parse_ppva_(const void* org, uint32_t len, const utils::array_uc& pbd, utils::file_io& fo,
		bool verbose)
	{
		const char* p = static_cast<const char*>(org);
		if(strncmp(p, "PPVA", 4) != 0) {
			utils::format("PPVA ID: error '%c%c%c%c'\n") % p[0] % p[1] % p[2] % p[3];
			return false;
		}

		uint32_t attr_size = get32_(p + 4);
		uint32_t param_size = get32_(p + 8);
		uint32_t index_low = get32_(p + 16);
		uint32_t index_high = get32_(p + 20);

		if(verbose) {
			utils::format("PPVA attribute size: %5d\n") % attr_size;
			utils::format("PPVA param size:     %5d\n") % param_size;
			utils::format("PPVA index low:      %5d\n") % index_low;
			utils::format("PPVA index high:     %5d\n") % index_high;
		}

		p += 24 + 8;
		for(uint32_t i = 0; i <= (index_high - index_low); ++i) {
			uint32_t ofs  = get32_(p);
			uint32_t fs   = get32_(p + 4);
			uint32_t size = get32_(p + 8);
			p += 16;
			if(verbose) {
				utils::format("    Index%d: ofs: %6d, freq: %d [Hz], size: %d\n")
					% i % ofs % fs % size;
			}
			psp::adpcm dec;
			bool loop = false;
			dec.start(0, size, loop);
			auto ap = &pbd[ofs];
			do {
				auto nap = dec.decode_block(ap);
				if(nap > ap) {
					dec.render_raw(fo);
				}
				ap = nap;
			} while(!dec.is_fin()) ;
		}

		return true;
	}
}


int main(int argc, char *argv[])
{
	const char* inp_file = nullptr;
	const char* out_file = nullptr;

	bool error = false;
	bool verbose = false;
	for(int i = 1; i < argc; ++i) {
		if(argv[i][0] == '-') {
			std::string opt = argv[i];
			if(opt == "-v") verbose = true;
			else if(opt == "--verbose") verbose = true;
			else {
				utils::format("Illegual options: '%s'\n") % opt.c_str();
				error = true;
			}
		} else {
			inp_file = out_file;
			out_file = argv[i];
		}
	}
	if(inp_file == nullptr && out_file != nullptr) {
		inp_file = out_file;
		out_file = nullptr;
	}

	if(inp_file == nullptr || error) {
		help_(argv[0]);
		return 0;
	}

	std::string base = inp_file;
	std::string phd_name = base + ".PHD";
	std::string pbd_name = base + ".PBD";
	if(!utils::probe_file(phd_name)) {
		utils::format("Cant open PHD-file: '%s'\n") % phd_name.c_str();
		return -1;
	}
	if(!utils::probe_file(pbd_name)) {
		utils::format("Cant open PBD-file: '%s'\n") % pbd_name.c_str();
		return -1;
	}

	std::string out_name;
	if(out_file != nullptr) {
		out_name = out_file;
	} else {
		out_name = base + ".RAW";
	}

	if(verbose) {
		utils::format("PHD file name: '%s'\n") % phd_name.c_str();
		utils::format("PBD file name: '%s'\n") % pbd_name.c_str();
		utils::format("OUT file name: '%s'\n") % out_name.c_str();
	}

	// read PHD
	utils::file_io fin;
	if(!fin.open(phd_name, "rb")) {
		utils::format("Can't open PHD file: '%s'\n") % phd_name.c_str();
		return -1;
	}
	auto phd = utils::read_array(fin);
	fin.close();

	// check ID
	if(strncmp(reinterpret_cast<const char*>(&phd[0]), "PPHD8", 5) != 0) {
		utils::format("PHD ID miss match: '%s'\n") % phd_name.c_str();
		return -1;
	}

	uint32_t pppg_ofs = get32_(&phd[16]);
	uint32_t pptn_ofs = get32_(&phd[20]);
	uint32_t ppva_ofs = get32_(&phd[24]);
	uint32_t pppg_len = pptn_ofs - pppg_ofs;
	uint32_t pptn_len = ppva_ofs - pptn_ofs;
	uint32_t ppva_len = phd.size() - ppva_ofs;

	if(verbose) {
		utils::format("PHD file size: %u\n") % phd.size();
		utils::format("    PPPG offset: %5d (size: %5d)\n") % pppg_ofs % pppg_len;
		utils::format("    PPTN offset: %5d (size: %5d)\n") % pptn_ofs % pptn_len;
		utils::format("    PPVA offset: %5d (size: %5d)\n") % ppva_ofs % ppva_len;
	}

	// read PBD
	if(!fin.open(pbd_name, "rb")) {
		utils::format("Can't open PBD file: '%s'\n") % pbd_name.c_str();
		return -1;
	}
	auto pbd = utils::read_array(fin);
	fin.close();

	if(verbose) {
		utils::format("PBD file size: %u\n") % pbd.size();
	}

	utils::file_io fo;
	if(!fo.open(out_name, "wb")) {
		utils::format("Can't open output file: '%s'\n") % out_name.c_str();
	}

//	al::audio aif(new al::audio_mno16);

	if(!parse_ppva_(&phd[ppva_ofs], ppva_len, pbd, fo, verbose)) {
		return -1;
	}

//	al::wav_io wav;

	fo.close();
}
