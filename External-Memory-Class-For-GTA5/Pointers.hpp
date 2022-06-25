#pragma once

#include "Common.hpp"

namespace Aure
{
	class Pointers
	{
	public:
		explicit Pointers();
		~Pointers();
	public:
        uint64_t WorldPTR;
        uint64_t BlipPTR;
        uint64_t ReplayInterfacePTR;
        uint64_t LocalScriptsPTR;
        uint64_t GlobalPTR;
        uint64_t PlayerCountPTR;
        uint64_t PickupDataPTR;
        uint64_t WeatherADDR;
        uint64_t SettingsPTRs;
        uint64_t AimCPedPTR;
        uint64_t FriendlistPTR;
        uint64_t ThermalADDR;
        uint64_t NightvisionADDR;
        uint64_t BlackoutADDR;
	};
	inline Pointers* g_pointers;
}


