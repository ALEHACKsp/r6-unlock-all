#include <Windows.h>
#include <iostream>
#include <cstdint>
constexpr ::std::ptrdiff_t function_off = 0x1138490;
bool __stdcall dllthread(HMODULE hModule) {


	//Allocating console
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	std::cout << "[UNLOCK ALL]" << std::endl;
	std::cout << "------------" << std::endl;

	//shell to stop Rainbow from terminating itself
	static const uint8_t shell[] = { 0xC3, 0x90, 0x90 };
	DWORD old;
	
	auto terminate = reinterpret_cast<uint64_t>(TerminateProcess);
	std::cout << "TerminateProcess at: " << std::hex << terminate << std::endl;
	VirtualProtect(reinterpret_cast<LPVOID> (terminate), sizeof(shell), PAGE_EXECUTE_READWRITE, &old);
	for (int idx = 0; idx < sizeof(shell); idx++)
		*(uint8_t*)(terminate + idx * 0x1) = shell[idx]; //Write shell
	VirtualProtect(reinterpret_cast<LPVOID> (terminate), sizeof(shell), old, 0);


	std::cout << "Function offset: " << std::hex << function_off << std::endl;
	uint64_t function = reinterpret_cast<uint64_t>(GetModuleHandleA(0)) + function_off;
	std::cout << "Function : " << std::hex << function << std::endl;
	uint64_t off1 = function + 0x23C;
	uint64_t off2 = function + 0x259;
	std::cout << "Swapped" << std::endl;

	VirtualProtect(reinterpret_cast<LPVOID>(off1), sizeof(uint8_t), PAGE_EXECUTE_READWRITE, &old);
	*(uint8_t*)off1 = 0x25;
	VirtualProtect(reinterpret_cast<LPVOID>(off1), sizeof(uint8_t), old, 0);

	VirtualProtect(reinterpret_cast<LPVOID>(off2), sizeof(uint8_t), PAGE_EXECUTE_READWRITE, &old);
	*(uint8_t*)off2 = 0x0;
	VirtualProtect(reinterpret_cast<LPVOID>(off2), sizeof(uint8_t), old, 0);
	std::cout << "Everything unlocked!" << std::endl;
	return true;
	
}



BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)dllthread, hModule, 0, 0));
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}