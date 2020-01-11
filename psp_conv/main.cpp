#include "utils/format.hpp"
#include "utils/input.hpp"
#include "utils/string_utils.hpp"
#include "utils/file_io.hpp"

#include "psp_adpcm.hpp"
#include "vag_conv.hpp"
#include "phd.hpp"

#include "snd_io/pcm.hpp"
#include "snd_io/wav_io.hpp"

namespace {

	static const uint32_t VERSION = 91;

	uint32_t adpcm_total_ = 0;

	const void* phd_org_ = nullptr;

	void help_(const char* cmd) {
		auto c = utils::get_file_base(cmd, true);
		utils::format("PSP Sound data converter (*.PHD, *.PBD to MVG [Multi VAG])\n");
		utils::format("  (--info) Input WAV (AT3) file loop infomations list\n");
		utils::format("Version %d.%02d\n") % (VERSION / 100) % (VERSION % 100);
		utils::format("usage:\n");
		utils::format("    %s [options] input-file [output-file]\n") % c.c_str();
		utils::format("    -v                  verbose\n");
		utils::format("    --info              input file infomations\n");
		utils::format("    --phd PHD-size      set a PHD size\n");
		utils::format("    --svl body-offset   set a PBD body offset\n");
///		utils::format("    -w make WAV file\n");
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


	bool parse_pptn_(const void* org, uint32_t len, bool verbose)
	{
		const char* p = static_cast<const char*>(org);
		if(strncmp(p, "PPTN", 4) != 0) {
			utils::format("PPTN ID: error '%c%c%c%c'\n") % p[0] % p[1] % p[2] % p[3];
			return false;
		}

		auto param_size = get32_(p + 8);
		auto index_low  = get32_(p + 16);
		auto index_high = get32_(p + 20);
		if(verbose) {
			utils::format("PPTN param size: %d\n") % param_size;
			utils::format("PPTN index low:  %d\n") % index_low;
			utils::format("PPTN index high: %d\n") % index_high;
		}
		p += 32;
		for(int i = index_low; i <= index_high; ++i) {
			if(verbose) {
				utils::format("  Index: %d\n") % i;
			}
			psp::toneParam t;
			t.load(p);
			if(verbose) t.list();
			p += sizeof(psp::toneParam);
		}

		return true;
	}


	bool parse_pppg_(const void* org, uint32_t len, bool verbose)
	{
		const char* p = static_cast<const char*>(org);
		if(strncmp(p, "PPPG", 4) != 0) {
			utils::format("PPPG ID: error '%c%c%c%c'\n") % p[0] % p[1] % p[2] % p[3];
			return false;
		}
		if(verbose) {
			utils::format("PPPG:\n");
		}

		p += 4 * 8;
		for(int i = 0; i < 128; ++i) {
			auto offset = get32_(p);
			p += 4;
			if(offset != 0xffffffff) {
				if(verbose) {
					utils::format("  bank: %3d\n") % i;
				}
				auto phd = static_cast<const uint8_t*>(phd_org_);
				auto pp = &phd[offset];
				auto count = get32_(pp);
				if(verbose) {
					utils::format("    toneParamCount: %d\n") % count;
				}
				pp += 16;
				for(int i = 0; i < count; ++i) {
					auto idx = get32_(pp);
					pp += 4;
					if(verbose) {
						utils::format("      (%3d) Index: %d\n") % i % idx;
					}
				}
			}
		}
		return true;
	}


	bool parse_ppva_(const char* name, const void* org, uint32_t len, const utils::array_uc& pbd,
		utils::file_io& fo, utils::file_io& wf, bool verbose)
	{
		const char* p = static_cast<const char*>(org);
		if(strncmp(p, "PPVA", 4) != 0) {
			utils::format("PPVA ID: error '%c%c%c%c'\n") % p[0] % p[1] % p[2] % p[3];
			return false;
		}

		uint32_t attr_size  = get32_(p + 4);
		uint32_t param_size = get32_(p + 8);
		uint32_t index_low  = get32_(p + 16);
		uint32_t index_high = get32_(p + 20);

		if(verbose) {
			utils::format("PPVA attribute size: %5d\n") % attr_size;
			utils::format("PPVA param size:     %5d\n") % param_size;
			utils::format("PPVA index low:      %5d\n") % index_low;
			utils::format("PPVA index high:     %5d\n") % index_high;
		}

		std::vector<ps4::vag_conv::OUT> outs;
		p += 24 + 8;
		for(uint32_t i = 0; i <= (index_high - index_low); ++i) {

			auto ofs  = get32_(p);
			auto fs   = get32_(p + 4);
			auto size = get32_(p + 8);
			p += 16;
			if(verbose) {
				utils::format("    Index%d: ofs: %6d, freq: %d [Hz], size(PSP PBD): %d\n")
					% i % ofs % fs % size;
			}

			al::audio aif(new al::audio_mno16);
			// 16 バイト単位、２８サンプル
			aif->create(fs, (size / 16) * 28);

			psp::adpcm dec;
			bool loop = true;
			dec.start(size, loop);
			auto ap = &pbd[ofs];
			int loop_start = -1;
			uint32_t loop_trg = 0;
			do {
				auto nap = dec.decode_block(ap);
				if(nap > ap) {
					dec.render_pcm(aif);
				}
				if(dec.is_loop()) {
					loop_trg = dec.get_current_block();
				}
				ap = nap;
			} while(!dec.is_fin()) ;

			loop_start = dec.get_loop_start();
			if(loop_start >= 0) {
				if(verbose) {
					utils::format("      Loop top: %d, loop trg: %d [block]\n")
						% loop_start % loop_trg;
				}
			}

			ps4::vag_conv vag;
			char id[16];
			utils::sformat("%s%03d", id, sizeof(id)) % name % i;
			auto data = vag.encode(id, aif, loop_start, loop_trg);
			outs.push_back(data);
		}
		adpcm_total_ += index_high - index_low;

		{  // make header tables: ID(VAGs), NUM, Offset[NUM]
			fo.put("VAGs");
			auto l = outs.size();
			fo.put32(l);
			uint32_t ofs = 8 + l * 4;
			if((ofs & 15) != 0) { ofs |= 15; ++ofs; }
			for(uint32_t i = 0; i < l; ++i) {
				fo.put32(ofs);
				ofs += outs[i].size();
			}
			l *= 4;
			l += 8;
			while((l & 15) != 0) {
				fo.put_char(0);
				++l;
			}
			for(int i = 0; i < outs.size(); ++i) {
				auto& data = outs[i];
				fo.write(&data[0], data.size());
			}
		}

		if(wf.is_open()) {  // Write WAV file..
		}

		return true;
	}


	int info_(const char* filename)
	{
		utils::file_io fin;
		if(!fin.open(filename, "rb")) {
			utils::format("Can't open input file: '%s'\n") % filename;
			return -1;
		}
		al::wav_io wav;
		al::audio_info info;
		auto ret = wav.info(fin, info);
		fin.close();

		if(ret) {
			const auto& t = wav.get_smpl_loop();
			utils::format("  Loop num: %d, SampleRate: %uHz (48000Hz), Chanel: %d\n")
				% t.size() % info.frequency % info.chanels;
			for(uint32_t i = 0; i < t.size(); ++i) {
				const auto& l = t[i];
				auto start48 = l.start * 48000 / info.frequency;
				auto end48   = l.end   * 48000 / info.frequency;
				utils::format("  (%u) ID: %08X\n") % i % l.id;
				utils::format("    Loop start: %9u (%9u)\n") % l.start % start48;
				utils::format("           end: %9u (%9u)\n") % l.end   % end48;
				utils::format("         count: %u\n") % l.play_count;
			}
		}

		return ret ? 0 : -1;
	}
}


int main(int argc, char *argv[])
{
	const char* inp_file = nullptr;
	const char* out_file = nullptr;

	bool error = false;
	bool wav = false;
	bool phds = false;
	bool svl = false;
	bool info = false;
	uint32_t phd_size = 0;
	uint32_t svl_offset = 0;
	bool verbose = false;
	for(int i = 1; i < argc; ++i) {
		if(argv[i][0] == '-') {
			std::string opt = argv[i];
			if(opt == "-v") verbose = true;
			else if(opt == "-w") wav = true;
			else if(opt == "--svl") svl = true;
			else if(opt == "--phd") phds = true;
			else if(opt == "--info") info = true;
			else if(opt == "--verbose") verbose = true;
			else {
				utils::format("Illegual options: '%s'\n") % opt.c_str();
				error = true;
			}
		} else {
			if(phds) {
				phd_size = 0;
				if(!(utils::input("%u", argv[i]) % phd_size).status()) {
					utils::format("Illegual PHD size: '%s'\n") % argv[i];
				}
				phds = false;
			} else if(svl) {
				svl_offset = 0;
				if(!(utils::input("%u", argv[i]) % svl_offset).status()) {
					utils::format("Illegual SVL offset: '%s'\n") % argv[i];
				}
				svl = false;
			} else {
				inp_file = out_file;
				out_file = argv[i];
			}
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
	std::string svl_name;

	std::string out_name;
	std::string wav_name;
	if(out_file != nullptr) {
		out_name = out_file;
	} else {
		out_name = base + ".MVG";
	}
	if(wav) {
		wav_name = base + ".WAV";
	}

	if(verbose) {
		if(info) {
			utils::format("Infomation mode:\n");
			utils::format("  Input file name: '%s'\n") % inp_file;
		} else {
			if(svl_offset > 0 && phd_size > 0 && phd_size <= svl_offset) {
				utils::format("SVL file name: '%s', PHD: %d, offset: %d\n")
					% svl_name.c_str() % phd_size % svl_offset;
			} else {
				utils::format("PHD file name: '%s'\n") % phd_name.c_str();
				utils::format("PBD file name: '%s'\n") % pbd_name.c_str();
			}
			utils::format("OUT file name: '%s'\n") % out_name.c_str();
			if(!wav_name.empty()) {
				utils::format("WAV file name: '%s'\n") % wav_name.c_str();
			}
		}
	}

	if(info) {
		return info_(inp_file);
	}

	utils::array_uc phd;
	if(svl_offset > 0) {
		svl_name = base + ".SVL";
		if(!utils::probe_file(svl_name)) {
			utils::format("Cant open SVL-file: '%s'\n") % svl_name.c_str();
			return -1;
		}
	} else {
		if(!utils::probe_file(phd_name)) {
			utils::format("Cant open PHD-file: '%s'\n") % phd_name.c_str();
			return -1;
		}
		if(!utils::probe_file(pbd_name)) {
			utils::format("Cant open PBD-file: '%s'\n") % pbd_name.c_str();
			return -1;
		}
	}


	utils::array_uc pbd;
	if(svl_offset > 0 && phd_size > 0 && phd_size <= svl_offset) {
		utils::file_io fin;
		if(!fin.open(svl_name, "rb")) {
			utils::format("Can't open SVL file: '%s'\n") % svl_name.c_str();
			return -1;
		}
		auto svl = utils::read_array(fin);
		fin.close();

		phd.resize(phd_size);
		memcpy(&phd[0],	&svl[0], phd_size);

		auto pbd_size = svl.size() - svl_offset;
		pbd.resize(pbd_size);
		memcpy(&pbd[0],	&svl[svl_offset], pbd_size);
	} else {
		// read PHD
		utils::file_io fin;
		if(!fin.open(phd_name, "rb")) {
			utils::format("Can't open PHD file: '%s'\n") % phd_name.c_str();
			return -1;
		}
		phd = utils::read_array(fin);
		fin.close();
	}
	phd_org_ = &phd[0];

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

	// Parse PPTN（トーンアトリビュート）
	if(!parse_pptn_(&phd[pptn_ofs], pptn_len, verbose)) {
		return -1;
	}

	// Parse PPPG（プログラムアトリビュート）
	if(!parse_pppg_(&phd[pppg_ofs], pppg_len, verbose)) {
		return -1;
	}

	// read PBD
	if(svl_offset > 0 && phd_size > 0 && phd_size <= svl_offset) {

	} else {
		utils::file_io fin;
		if(!fin.open(pbd_name, "rb")) {
			utils::format("Can't open PBD file: '%s'\n") % pbd_name.c_str();
			return -1;
		}
		pbd = utils::read_array(fin);
		fin.close();
	}

	if(verbose) {
		utils::format("PBD file size: %u\n") % pbd.size();
	}

	utils::file_io fo;
	if(!fo.open(out_name, "wb")) {
		utils::format("Can't open output file: '%s'\n") % out_name.c_str();
	}

	// PHD output (PPPG, PPTN)
	fo.write(&phd[0], phd.size());

	utils::file_io wf;
	if(!wav_name.empty()) {
//		if(!wf.open(wav_name, "wb")) {
//			utils::format("Can't open output WAV file: '%s'\n") % wav_name.c_str();
//		}		
	}

	auto fn = utils::get_file_base(base);
	if(!parse_ppva_(fn.c_str(), &phd[ppva_ofs], ppva_len, pbd, fo, wf, verbose)) {
		return -1;
	}

	if(!wav_name.empty()) {
//		wf.close();
	}

	fo.close();

	if(verbose) {
		utils::format("ADPCM Total: %u\n") % adpcm_total_;
	}
}
