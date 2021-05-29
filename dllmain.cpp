// dllmain.cpp : Defines the entry point for the DLL application.
#define WIN32_LEAN_AND_MEAN     

/*Windows Header Files*/
#include <windows.h>
#include <stdio.h>
#include <thread>

/*Include Files*/
#include "ClassPointers.hpp"
#include "CreatedThreads/threadtable.hpp"

/*Variables for Storing Thread Stuff*/
HANDLE gMainHandle{};
LPDWORD gMainThreadID{};

/*Enables the use of the time Operators like ms or s*/
using namespace std::chrono_literals; 

/*Adds a few Libraries to the Linker*/
#pragma comment(lib, "Libraries/PolyHook_2.lib")
#pragma comment(lib, "Libraries/capstone.lib")

/*Prints a couple of messages during Compile Time*/
#pragma message("++++++++++++++++++++++++++++++++++++")
#pragma message("+		 Compiling Hexen Base		 +")
#pragma message("++++++++++++++++++++++++++++++++++++")

/*DLL Main Entry Point*/
bool __stdcall DllMain(HMODULE hModule, DWORD callrs, LPVOID) {
	DisableThreadLibraryCalls(hModule);														/*Disables Thread Attach/Detach Calls*/

	if (callrs == 1) {
		gMainHandle = CreateThread(nullptr, NULL, [](LPVOID module_) -> DWORD {
		
			if (AllocConsole())																/*Allocates a Logging Console*/
				freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);		/*Opens the Standard Output Port*/
			SetConsoleTextAttribute(GetStdHandle(static_cast<DWORD>(-11)), 0x0004);			/*Colors the Console Red for the Output Handle*/


			printfnl("Welcome to Hexen Base");
			printfnl("Hexen Status Code: [102 -> Processing]");								/*All Status Codes are HTML Status Codes*/

			ClassPointers::cHelpers->GetGTAWindow();										/*Writes the result of FindWindowA into a variable*/

			ClassPointers::cPatterns->ScanPatterns();										/*Scanns Memory Regions for Pointers*/

			if (GetModuleHandleA("eossdk-win64-shipping.dll"))								/*Checks if Game has Epic's DLL Present*/
				printfnl("Playing On Epic Games Store Version");
			else if (GetModuleHandleA("steam_api64.dll"))									/*Checks if Game has Steam's DLL Present*/
				printfnl("Playing on Steam Store Version");
			else																			/*Returns false on both if Retail Version is Running*/
				printfnl("Playing on Rockstar Games Retail Version");

			ClassPointers::cDX->InitRender();												/*Starts Up the DirectX Render Function*/

			ClassPointers::cHelpers->add_script(CreatedThreads::NativeThread, "Native");	/*Launches Fiber for Executing Natives, you can launch more than one btw*/

			ClassPointers::cHooks->TriggerHook(true);										/*Creates all Execution Hooks*/

			printfnl("Hexen Status Code: [226 -> IM Used]");
			do {
				if (GetAsyncKeyState(VK_F10) & 0x8000)										/*Checks if Key is/was pressed to UnInject Hexen*/
					bUnInject = true;
				std::this_thread::sleep_for(30ms);
			} while (!bUnInject);															/*Sleeps Thread while DLL should not uninject*/
		
			printfnl("Unloading from the Grand Theft Auto V Module");

			ClassPointers::cHooks->TriggerHook(false);										/*Removes all Execution Hooks*/

			ClassPointers::cHelpers->remove_scripts();										/*Deletes All Scripts Created On Startup*/

			ClassPointers::ResetPointers();													/*Resets all Unqiue Pointer Created On Inject*/

			printfnl("Hexen Status Code: [410 -> Gone]");

			fclose(stdout);																	/*Closes the Standard Output Port*/
			FreeConsole();																	/*Detaches the Console from the Host Process*/
			CloseHandle(gMainHandle);														/*Closes Handle to Thread*/
			FreeLibraryAndExitThread(reinterpret_cast<HINSTANCE>(module_), EXIT_SUCCESS);	/*Frees Library Module for Uninjection*/

		}, hModule, NULL, gMainThreadID);
	}

	return true;
}

