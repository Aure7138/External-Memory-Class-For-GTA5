#pragma once

#include "Common.hpp"

#define CHUNK_SIZE 0x1000

namespace Aure
{
	class Pattern
	{
		std::vector<std::optional<uint8_t>> compiled;
		std::string name;
		uintptr_t   address = 0;
	public:
		inline Pattern(std::string name, std::string x) :
			name(name)
		{
			compile(x);
			scan();
		}

		inline Pattern(std::string name_, uintptr_t addr) {
			name = name_;
			address = addr;
		}

		void compile(const std::string& pat);

		Pattern& scan();

		inline Pattern add(size_t n) {
			return Pattern(name, address + n);
		}

		inline Pattern sub(size_t n) {
			return Pattern(name, address - n);
		}

		Pattern rip();

		inline operator std::uint64_t() const
		{
			return address;
		}
	};
}


