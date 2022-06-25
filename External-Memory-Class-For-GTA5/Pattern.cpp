#include "Pattern.hpp"
#include "Memory.hpp"

namespace Aure
{
	void Pattern::compile(const std::string& pat) {
		int hexchar = 0;
		bool first = true;
		bool lastwaswc = false;

		auto to_hex = [&](char c) -> std::uint8_t
		{
			switch (std::toupper(c))
			{
			case '0':
				return static_cast<std::uint8_t>(0);
			case '1':
				return static_cast<std::uint8_t>(1);
			case '2':
				return static_cast<std::uint8_t>(2);
			case '3':
				return static_cast<std::uint8_t>(3);
			case '4':
				return static_cast<std::uint8_t>(4);
			case '5':
				return static_cast<std::uint8_t>(5);
			case '6':
				return static_cast<std::uint8_t>(6);
			case '7':
				return static_cast<std::uint8_t>(7);
			case '8':
				return static_cast<std::uint8_t>(8);
			case '9':
				return static_cast<std::uint8_t>(9);
			case 'A':
				return static_cast<std::uint8_t>(10);
			case 'B':
				return static_cast<std::uint8_t>(11);
			case 'C':
				return static_cast<std::uint8_t>(12);
			case 'D':
				return static_cast<std::uint8_t>(13);
			case 'E':
				return static_cast<std::uint8_t>(14);
			case 'F':
				return static_cast<std::uint8_t>(15);
			default:
				return 0;
			}
		};

		for (char c : pat) {
			if (c == '?' && !lastwaswc) {
				lastwaswc = true;
				compiled.push_back(std::nullopt);
			}
			else {
				lastwaswc = false;
				if (c == ' ') continue;
				hexchar += first ? to_hex(c) * 0x10 : to_hex(c);

				if (!first) {
					compiled.push_back(hexchar);
					hexchar = 0;
				}
				first = !first;
			}
		}
	}

	Pattern& Pattern::scan() {
		size_t i = gta5->m_base_address;
		std::uint8_t* membuf = new std::uint8_t[CHUNK_SIZE];

		address = 0;

		while (address == 0) {
			if (i >= (gta5->m_base_address + gta5->m_size))
				break;

			if (!gta5->read<uint8_t*>((void*)i, membuf, CHUNK_SIZE))
				goto next;

			for (int j = 0; j < CHUNK_SIZE; j++) {
				for (int k = 0; k < compiled.size(); k++) {
					if (!compiled[k].has_value()) continue;
					if (compiled[k].value() != membuf[j + k]) goto incorrect;
				}

				address = i + j;
				break;

			incorrect: continue;
			}

		next:
			i += CHUNK_SIZE;
		}

		delete[] membuf;

		if (address == NULL)
			throw std::exception(("cannot find pattern " + name).data());

		return *this;
	}

	Pattern Pattern::rip() {
		return add(gta5->r<int>(address)).add(4);
	}
}
