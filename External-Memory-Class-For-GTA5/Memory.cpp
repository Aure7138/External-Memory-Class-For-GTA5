#include "Memory.hpp"
#include "Pointers.hpp"

BOOL ListSystemProcesses(const wchar_t szExeFile[MAX_PATH], LPPROCESSENTRY32 pe32)
{
    HANDLE m_handleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (m_handleSnap == INVALID_HANDLE_VALUE)
    {
        MessageBox(
            NULL,
            (LPCWSTR)L"CreateToolhelp32Snapshot (of processes)",
            (LPCWSTR)L"Error",
            MB_ICONSTOP | MB_ICONERROR | MB_ICONHAND
        );
        return(FALSE);
    }
    pe32->dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(m_handleSnap, pe32))
    {
        MessageBox(
            NULL,
            (LPCWSTR)L"Process32First",
            (LPCWSTR)L"Error",
            MB_ICONSTOP | MB_ICONERROR | MB_ICONHAND
        );
        CloseHandle(m_handleSnap);
        return(FALSE);
    }
    do
    {
        if (lstrcmp(szExeFile, pe32->szExeFile) == 0)
        {
            CloseHandle(m_handleSnap);
            return(TRUE);
        }
    } while (Process32Next(m_handleSnap, pe32));
    CloseHandle(m_handleSnap);
    return(FALSE);
}

BOOL ListProcessModules(uint32_t dwProcessId, const wchar_t szModule[MAX_MODULE_NAME32 + 1], LPMODULEENTRY32 me32)
{
    HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwProcessId);
    if (hModuleSnap == INVALID_HANDLE_VALUE)
    {
        MessageBox(
            NULL,
            (LPCWSTR)L"CreateToolhelp32Snapshot (of modules)",
            (LPCWSTR)L"Error",
            MB_ICONSTOP | MB_ICONERROR | MB_ICONHAND
        );
        return(FALSE);
    }
    me32->dwSize = sizeof(MODULEENTRY32);
    if (!Module32First(hModuleSnap, me32))
    {
        MessageBox(
            NULL,
            (LPCWSTR)L"Module32First",
            (LPCWSTR)L"Error",
            MB_ICONSTOP | MB_ICONERROR | MB_ICONHAND
        );
        CloseHandle(hModuleSnap);
        return(FALSE);
    }
    do
    {
        if (lstrcmp(szModule, me32->szModule) == 0)
        {
            CloseHandle(hModuleSnap);
            return(TRUE);
        }
    } while (Module32Next(hModuleSnap, me32));
    CloseHandle(hModuleSnap);
    return(FALSE);
}

namespace Aure
{
	Memory::Memory(const wchar_t name[MAX_PATH]) :
		m_name(name)
	{
        PROCESSENTRY32 pe32;
        if (ListSystemProcesses(name, &pe32) == FALSE) throw std::exception("Failed to find process id");

        m_pid = pe32.th32ProcessID;
        m_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_pid);
        if (!m_handle) throw std::exception("Failed to open handle");

        MODULEENTRY32 me32;
        if (ListProcessModules(m_pid, name, &me32) == FALSE) throw std::exception("Failed to get model info");

        m_base_address = (uintptr_t)me32.modBaseAddr;
        m_size = me32.modBaseSize;
	}

    Memory::~Memory(){ CloseHandle(m_handle); }

    bool Memory::is_running(){ return WaitForSingleObject(m_handle, 0) == WAIT_TIMEOUT; }

    void Memory::set_paused(bool toggle) { toggle ? SuspendThread(m_handle) : ResumeThread(m_handle); }

    uint64_t Memory::allocate(size_t size){ return (uint64_t)VirtualAllocEx(m_handle, nullptr, size, MEM_COMMIT, PAGE_READWRITE); }

    std::string Memory::read_str(DWORD_PTR BaseAddress, SIZE_T nSize, std::vector<INT64> offsets)
    {
        char* str = new char[nSize];
        memset(str, 0, nSize);
        for (SIZE_T i = 0; i < offsets.size(); i++)
        { 
            if (BaseAddress == 0) { delete[] str; return std::string(); }
            read<DWORD_PTR>(LPCVOID(BaseAddress), &BaseAddress);
            BaseAddress += offsets[i];
        }
        read<char>(LPCVOID(BaseAddress), str, nSize);
        std::string ret = str;
        delete[] str;
        return ret;
    }
    
    void Memory::write_str(DWORD_PTR BaseAddress, std::string str, SIZE_T nSize, std::vector<INT64> offsets)
    {
        for (SIZE_T i = 0; i < offsets.size(); i++)
        {
            if (BaseAddress == 0) return;
            read<DWORD_PTR>(LPCVOID(BaseAddress), &BaseAddress);
            BaseAddress += offsets[i];
        }
        write<char>(LPVOID(BaseAddress), str.c_str(), nSize);
    }

    GTA5::GTA5() : Memory(L"GTA5.exe")
    {
        gta5 = this;
    }

    GTA5::~GTA5(){ gta5 = nullptr; }

    int64_t GTA5::GA(int index)
    {
        int a = 8 * ((index >> 0x12) & 0x3F);
        int b = 8 * (index & 0x3FFFF);
        return r<int64_t>(g_pointers->GlobalPTR + a) + b;
    }

    int64_t GTA5::LA(std::string name, int index)
    {
        for (int i = 0; i < 54; i++)
        {
            std::string str = read_str(g_pointers->LocalScriptsPTR, MAX_PATH, { i * 0x8, 0xD0 });
            if (str == name) { return r<int64_t>(g_pointers->LocalScriptsPTR, { i * 0x8, 0xB0 }) + 8 * index; }
        }
    }

    bool GTA5::get_blip(int64_t &blip, std::vector<int> icons, std::vector<int> colors)
    {
        for (int i = 0; i < 2000; i++)
        {
            int64_t p = r<int64_t>(g_pointers->BlipPTR + i * 0x8);
            if (p == 0) continue;
            int icon = r<int>(p + 0x40);
            int color = r<int>(p + 0x48);
            if (std::find(icons.begin(), icons.end(), icon) != icons.end())
            {
                if (colors.size() == 0 || std::find(colors.begin(), colors.end(), color) != colors.end())
                {
                    blip = p;
                    return true;
                }
            }
        }
        return false;
    }

    uint32_t joaat(std::string str)
    {
        unsigned int Uint32 = 0xffffffff;
        unsigned int hash = 0;
        std::transform(str.begin(), str.end(), str.begin(), tolower);
        for (int i = 0; i < str.length(); i++)
        {
            hash = hash + (unsigned char)str[i];
            hash = hash + (hash << 10);
            hash = hash ^ ((hash & Uint32) >> 6);
        }
        hash = hash + (hash << 3);
        hash = hash ^ ((hash & Uint32) >> 11);
        hash = hash + (hash << 15);
        return hash & Uint32;
    }

    Vector3 GTA5::get_blip_pos(int64_t blip) { return  r<Vector3>(blip + 0x10); }

    int64_t GTA5::get_local_ped() { return r<int64_t>(g_pointers->WorldPTR, { 0x8 }); }

    int64_t GTA5::ped_get_current_vehicle(int64_t ped) { return r<int64_t>(ped + 0xD30); }

    bool GTA5::ped_is_in_vehicle(int64_t ped) { return r<byte>(ped + 0xE52) == 0 ? false : true; }

    void GTA5::entity_set_position(int64_t entity, Vector3 pos)
    {
        w<Vector3>(entity + 0x30, { 0x50 }, pos);
        w<Vector3>(entity + 0x90, {}, pos);
    }

    void GTA5::to_waypoint(int64_t ped)
    {
        int64_t blip;
        if (!get_blip(blip, { 8 }, { 84 })) return;
        int64_t entity = ped_is_in_vehicle(ped) ? ped_get_current_vehicle(ped) : ped;
        Vector3 pos = get_blip_pos(blip);
        pos.z += 1;
        if (pos.z == 21.0) pos.z = -225.0;
        entity_set_position(entity, pos);
    }
}
