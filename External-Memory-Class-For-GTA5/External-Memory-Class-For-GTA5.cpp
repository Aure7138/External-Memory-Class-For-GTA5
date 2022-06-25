#include "Common.hpp"
#include "Memory.hpp"
#include "Pattern.hpp"
#include "Pointers.hpp"

using namespace Aure;

int main()
{
    auto gta5_instance = std::make_unique<GTA5>();
    auto pointers_instance = std::make_unique<Pointers>();

	std::cout << "WorldPTR:          " << std::hex << g_pointers->WorldPTR << "\n";
	std::cout << "BlipPTR:           " << std::hex << g_pointers->BlipPTR << "\n";
	std::cout << "ReplayInterfacePTR:" << std::hex << g_pointers->ReplayInterfacePTR << "\n";
	std::cout << "LocalScriptsPTR:   " << std::hex << g_pointers->LocalScriptsPTR << "\n";
    std::cout << "GlobalPTR:         " << std::hex << g_pointers->GlobalPTR << "\n";
    std::cout << "PlayerCountPTR:    " << std::hex << g_pointers->PlayerCountPTR << "\n";
    std::cout << "PickupDataPTR:     " << std::hex << g_pointers->PickupDataPTR << "\n";
    std::cout << "WeatherADDR:       " << std::hex << g_pointers->WeatherADDR << "\n";
    std::cout << "SettingsPTRs:      " << std::hex << g_pointers->SettingsPTRs << "\n";
    std::cout << "AimCPedPTR:        " << std::hex << g_pointers->AimCPedPTR << "\n";
    std::cout << "FriendlistPTR:     " << std::hex << g_pointers->FriendlistPTR << "\n";
    std::cout << "ThermalADDR:       " << std::hex << g_pointers->ThermalADDR << "\n";
    std::cout << "NightvisionADDR:   " << std::hex << g_pointers->NightvisionADDR << "\n";
    std::cout << "BlackoutADDR:      " << std::hex << g_pointers->BlackoutADDR << "\n";

	while (g_running && gta5->is_running())
	{
		Sleep(100);
	}
	gta5_instance.reset();
	pointers_instance.reset();
	return 0;
}
