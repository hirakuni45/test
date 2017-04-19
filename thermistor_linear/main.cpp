#include <iostream>
#include <cmath>

namespace {

		// VCC --- TH -(A/D)- R --- GND
	static const float B    = 3452.0f;   ///< サーミスタＢ定数
	static const float T0   = 298.15f;   ///< 絶対温度
	static const float TR25 = 10.0f;     ///< R25 サーミスタ２５℃基準抵抗値
	static const float R    = 10.0f;     ///< 分圧抵抗 

	float thermistor_upper_(uint16_t raw)
	{
		float thr = (R * 4095.0f * (1.0f / static_cast<float>(raw))) - R;
		float t = 1.0f / (std::log(thr / TR25) / B + (1.0f / T0));
		return t - 273.15f;
	}

	float thermistor_lower_(uint16_t raw)
	{
		float thr = R * static_cast<float>(raw) / static_cast<float>(4095 - raw);
		float t = 1.0f / (std::log(thr / TR25) / B + (1.0f / T0));
		return t - 273.15f;
	}
}

int main(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	{
		auto k = thermistor_lower_(1);
		std::cout << "L   1: " << k << std::endl;
	}

	{
		auto k = thermistor_lower_(1024);
		std::cout << "L 1024: " << k << std::endl;
	}

	{
		auto k = thermistor_lower_(2048);
		std::cout << "L 2048: " << k << std::endl;
	}

	{
		auto k = thermistor_lower_(3072);
		std::cout << "L 3072: " << k << std::endl;
	}

	{
		auto k = thermistor_lower_(4094);
		std::cout << "L 4094: " << k << std::endl;
	}



	{
		auto k = thermistor_upper_(512);
		std::cout << "U 512: " << k << std::endl;
	}

	{
		auto k = thermistor_upper_(1024);
		std::cout << "U 1024: " << k << std::endl;
	}

	{
		auto k = thermistor_upper_(2048);
		std::cout << "U 2048: " << k << std::endl;
	}

	{
		auto k = thermistor_upper_(3072);
		std::cout << "U 3072: " << k << std::endl;
	}

	{
		auto k = thermistor_upper_(4094);
		std::cout << "U 4094: " << k << std::endl;
	}
}
