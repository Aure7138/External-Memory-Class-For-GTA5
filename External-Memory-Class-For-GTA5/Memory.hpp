#pragma once

#include "Common.hpp"

namespace Aure
{
	class Memory
	{
		std::wstring m_name;
		HANDLE m_handle;
		uint32_t m_pid;
	public:
		Memory(const wchar_t name[MAX_PATH]);
		~Memory();
		uintptr_t m_base_address;
		uint32_t m_size;
		bool is_running();
		void set_paused(bool toggle);
		uint64_t allocate(size_t size);
		template<typename T> BOOL read(LPCVOID lpBaseAddress, LPVOID lpBuffer, SIZE_T nSize = NULL, SIZE_T* lpNumberOfBytesRead = NULL)
		{
			nSize = (nSize == NULL) ? sizeof(T) : nSize;
			return ReadProcessMemory(m_handle, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesRead);
		}
		template<typename T> BOOL write(LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize = NULL, SIZE_T* lpNumberOfBytesWritten = NULL)
		{
			nSize = (nSize == NULL) ? sizeof(T) : nSize;
			return WriteProcessMemory(m_handle, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesWritten);
		}
		template<typename T> T r(DWORD_PTR BaseAddress, std::vector<INT64> offsets = {})
		{
			T ret;
			for (SIZE_T i = 0; i < offsets.size(); i++)
			{
				if (BaseAddress == 0) return 0;
				read<DWORD_PTR>(LPCVOID(BaseAddress), &BaseAddress);
				BaseAddress += offsets[i];
			}
			read<T>(LPCVOID(BaseAddress), &ret);
			return ret;
		}
		template<typename T> void w(DWORD_PTR BaseAddress, std::vector<INT64> offsets, T value)
		{
			for (SIZE_T i = 0; i < offsets.size(); i++)
			{
				if (BaseAddress == 0) return 0;
				read<DWORD_PTR>(LPCVOID(BaseAddress), &BaseAddress);
				BaseAddress += offsets[i];
			}
			write<T>(LPVOID(BaseAddress), &value);
		}
		std::string read_str(DWORD_PTR BaseAddress, SIZE_T nSize, std::vector<INT64> offsets = {});
		void write_str(DWORD_PTR BaseAddress, std::string str, SIZE_T nSize, std::vector<INT64> offsets = {});
	};
	class GTA5 : public Memory
	{
	public:
		GTA5();
		~GTA5();
		int64_t GA(int index);
		template<typename T> T GG(int index) { return r<T>(GA(index)); }
		template<typename T> void SG(int index, T value) { w<T>(GA(index), {}, value); }
		int64_t LA(std::string name, int index);
		template<typename T> T GL(std::string name, int index) { return r<T>(LA(name, index)); }
		template<typename T> void SL(std::string name, int index, T value) { w<T>(LA(name, index), {}, value); }
		bool get_blip(int64_t &blip, std::vector<int> icons, std::vector<int> colors = {});
	};
	inline GTA5* gta5;
	uint32_t joaat(std::string str);
}

