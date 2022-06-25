#include "Pointers.hpp"
#include "Pattern.hpp"

namespace Aure
{
	Pointers::Pointers() :
		WorldPTR(Pattern("WorldPTR", "48 8B 05 ? ? ? ? 45 ? ? ? ? 48 8B 48 08 48 85 C9 74 07").add(3).rip()),
		BlipPTR(Pattern("BlipPTR", "4C 8D 05 ? ? ? ? 0F B7 C1").add(3).rip()),
		ReplayInterfacePTR(Pattern("ReplayInterfacePTR", "48 8D 0D ? ? ? ? 48 8B D7 E8 ? ? ? ? 48 8D 0D ? ? ? ? 8A D8 E8").add(3).rip()),
		LocalScriptsPTR(Pattern("LocalScriptsPTR", "48 8B 05 ? ? ? ? 8B CF 48 8B 0C C8 39 59 68").add(3).rip()),
		GlobalPTR(Pattern("GlobalPTR", "4C 8D 05 ? ? ? ? 4D 8B 08 4D 85 C9 74 11").add(3).rip()),
		PlayerCountPTR(Pattern("PlayerCountPTR", "48 8B 0D ? ? ? ? E8 ? ? ? ? 48 8B C8 E8 ? ? ? ? 48 8B CF").add(3).rip()),
		PickupDataPTR(Pattern("PickupDataPTR", "48 8B 05 ? ? ? ? 48 8B 1C F8 8B").add(3).rip()),
		WeatherADDR(Pattern("WeatherADDR", "48 83 EC ? 8B 05 ? ? ? ? 8B 3D ? ? ? ? 49").add(6).rip().add(6)),
		SettingsPTRs(Pattern("SettingsPTRs", "44 39 05 ? ? ? ? 75 0D").add(3).rip().add(0x89 - 4)),
		AimCPedPTR(Pattern("AimCPedPTR", "48 8B 0D ? ? ? ? 48 85 C9 74 0C 48 8D 15 ? ? ? ? E8 ? ? ? ? 48 89 1D").add(3).rip()),
		FriendlistPTR(Pattern("FriendlistPTR", "48 8B 0D ? ? ? ? 8B C6 48 8D 5C 24 70").add(3).rip()),
		ThermalADDR(Pattern("ThermalADDR", "48 83 EC ? 80 3D ? ? ? ? 00 74 0C C6 81").add(6).rip().add(7)),
		NightvisionADDR(Pattern("NightvisionADDR", "48 8B D7 48 8B C8 E8 ? ? ? ? 80 3D").add(13).rip().add(14)),
		BlackoutADDR(Pattern("BlackoutADDR", "48 8B D1 8B 0D ? ? ? ? 45 8D 41 FC E9 ? ? ? ? 48 83").add(31).rip().add(31))
	{
		g_pointers = this;
	}

	Pointers::~Pointers()
	{
		g_pointers = nullptr;
	}
}
