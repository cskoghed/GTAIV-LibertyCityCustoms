#include "pch.h"
#include <Windows.h>
#include <cstdint>
#include <string>

using GetPlayerChar_t = void(__cdecl*)(int, int*);
using GetCharCar_t = void(__cdecl*)(int, int*);
using SetCharCarColour_t = void(__cdecl*)(int, int, int);

DWORD WINAPI MainThread(LPVOID)
{

    //0xba63a0; // Get car char is using
    //0xbcd540; // Change car color

    uintptr_t base =
        reinterpret_cast<uintptr_t>(GetModuleHandleA(nullptr));

    // 0x511454A9 -> 0xBB1770 -> 0xBB3050
    uintptr_t nativeGetPlayerCharAddr       = base - 0x400000 + 0xBB3050;
    // 0x1B067237 -> 0xB9E4D0 -> 0xBA5D70
    uintptr_t nativeGetCarCharIsUsingAddr   = base - 0x400000 + 0xBA5D70;
    // 0x06441EAF -> 0xBC4AE0 -> 0xBC8EB0
    uintptr_t nativeChangeCarColourAddr     = base - 0x400000 + 0xBC8EB0;


    auto GetPlayerChar = reinterpret_cast<GetPlayerChar_t>(nativeGetPlayerCharAddr);
    auto GetCharCar = reinterpret_cast<GetCharCar_t>(nativeGetCarCharIsUsingAddr);
    auto ChangeCarColour = reinterpret_cast<SetCharCarColour_t>(nativeChangeCarColourAddr);

    while (true)
    {
        if (GetAsyncKeyState(VK_F6) & 1)
        {
            int playerPed = 0;
            int playerVehicle = 0;

            GetPlayerChar(0, &playerPed);
            GetCharCar(playerPed, &playerVehicle);
            ChangeCarColour(playerVehicle, 21, 52);
        }

        Sleep(10);
    }

    return 0;
}

BOOL APIENTRY DllMain(
    HMODULE hModule,
    DWORD reason,
    LPVOID reserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);

        CreateThread(
            nullptr,
            0,
            MainThread,
            nullptr,
            0,
            nullptr
        );
    }

    return TRUE;
}