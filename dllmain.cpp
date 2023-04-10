// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "engine.h"


BOOL APIENTRY DllMain(HMODULE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		EngineInit();
		
	}
	return TRUE;
}
