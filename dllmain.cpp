// dllmain.cpp : Defines the entry point for the DLL application.
#include <thread>
#include <sstream>
#include <fstream>
#include <iostream>
#include <chrono>

#include "wingui.h"


uintptr_t baseMod = (uintptr_t)GetModuleHandleA("StateOfDecay2-Win64-Shipping.exe") + 0x04310348;
uintptr_t positionMod = (uintptr_t)GetModuleHandleA("StateOfDecay2-Win64-Shipping.exe") + 0x04310348;

int* ammoPtr = nullptr;
float* posPtr = nullptr;
float saveVal[3];
float wayPoint[3];
void recalculate() {
    baseMod = (uintptr_t)GetModuleHandleA("StateOfDecay2-Win64-Shipping.exe") + 0x04310348;
    baseMod = (uintptr_t) * ((uintptr_t*)baseMod);
    baseMod = (uintptr_t) * ((uintptr_t*)baseMod);
    baseMod = (uintptr_t) * ((uintptr_t*)(baseMod + 0x7A8));
    baseMod = (uintptr_t) * ((uintptr_t*)(baseMod + 0x3E8));
    baseMod = (uintptr_t) * ((uintptr_t*)(baseMod + 0xF0));
    baseMod = (uintptr_t) * ((uintptr_t*)(baseMod + 0x50));
    baseMod = (uintptr_t) * ((uintptr_t*)(baseMod + 0x0));
    baseMod = (uintptr_t)((uintptr_t*)(baseMod + 0x50));
    ammoPtr = (int*)baseMod;
}


void calculatePositionAddress() {
    //std::ofstream file("C:\\Users\\LydianJay\\Desktop\\sodLog.txt");
    //file << "[Log]" << '\n' << std::hex;
    positionMod = (uintptr_t)GetModuleHandleA("StateOfDecay2-Win64-Shipping.exe") + 0x04310348;
    positionMod = (uintptr_t) * ((uintptr_t*)positionMod);
    //file << "Address: " << positionMod << '\n';
    positionMod = (uintptr_t) * ((uintptr_t*)positionMod);
    //file << "Address: " << positionMod << '\n';
    positionMod = (uintptr_t) * ((uintptr_t*)(positionMod + 0x3E0));
    //file << "Address: " << positionMod << '\n';
    positionMod = (positionMod + 0x178);
    //file << "Address: " << positionMod << '\n';
    positionMod -= 8;
    //file << "Address: " << positionMod << '\n';
   // file.close();
    posPtr = (float*)positionMod;
    
    for (int i = 0; i < 3; i++) {
        saveVal[i] = posPtr[i];
        wayPoint[i] = posPtr[i];
    }
}


DWORD WINAPI ThreadCall(HMODULE Hmodule) {
    bool isUnliAmmo = false;
    
    //AllocConsole();
    //FILE* f;
   // freopen_s(&f, "CONOUT$", "w", stdout);
    bool showWindow = true;
    

    CWindow window(500, 400, "State Of Decay Trainer", WG_WINDOW_NO_RESIZE, { 0.6f, 0.6f, 0.6f });
    CComponents components;
    window.InitComponents(&components);
    CStaticText text(100,25, 20, 20, "ADDRESS:");
    CStaticText text2(200, 25, 160, 20, "0x00000000");
    CStaticText text3(100, 25, 20, 90, "VALUE:");
    CStaticText text4(100, 25, 160, 90, "N/A");
    CStaticText text5(100, 25, 20, 125, "STATE:");
    CStaticText text6(100, 25, 160, 125, "FALSE");

    CStaticText xlabel(50, 21, 0, 165, "X:");
    CStaticText xvalue(75, 21, 51, 165, "NA");
    CStaticText ylabel(50, 21, 0, 165 + (21 * 1), "Y:");
    CStaticText yvalue (75, 21, 51, 165 + (21 * 1), "NA");
    CStaticText zlabel(50, 21, 0, 165 + (21 * 2), "Z:");
    CStaticText zvalue(75, 21, 51, 165 + (21 * 2), "NA");

    components.AddComponent((ICompInterface*)&text); // 0
    components.AddComponent((ICompInterface*)&text2);
    components.AddComponent((ICompInterface*)&text3);
    components.AddComponent((ICompInterface*)&text4);
    components.AddComponent((ICompInterface*)&text5);
    components.AddComponent((ICompInterface*)&text6);

    components.AddComponent((ICompInterface*)&xlabel);
    components.AddComponent((ICompInterface*)&xvalue); // 7
    components.AddComponent((ICompInterface*)&ylabel);
    components.AddComponent((ICompInterface*)&yvalue); // 9
    components.AddComponent((ICompInterface*)&zlabel);
    components.AddComponent((ICompInterface*)&zvalue); // 11

    components.Enable();
    unsigned long long milli = 0, accum = 0, accum2 = 0;

    while (!window.WindowShouldClose()) {


        auto t1 = std::chrono::high_resolution_clock::now();

        if (GetAsyncKeyState(VK_HOME) & 1) {
            showWindow = !showWindow;

            showWindow ? window.ShowMyWindow() : window.HideWindow();



        }


        if (GetAsyncKeyState(VK_NUMPAD0) & 0x1) {
            isUnliAmmo = !isUnliAmmo;
            if(isUnliAmmo)
             recalculate();

            if (showWindow) {

                std::stringstream ss;
                (isUnliAmmo) ? (ss << "ENABLED") : (ss << "DISABLED");
                components.SetText(5, ss.str().c_str());
            }
        
        }

        if (GetAsyncKeyState(VK_DELETE) & 0x1) {
           
            FreeLibraryAndExitThread(Hmodule, 0);
            return 0;
        }
       
        if (GetAsyncKeyState(VK_NUMPAD1) & 0x1) {
            calculatePositionAddress();
        }

        


        if (GetAsyncKeyState(VK_F5) & 1) {

            if (posPtr == nullptr) {
                calculatePositionAddress();
            }
     
            else {
                for (int i = 0; i < 3; i++) {
                    saveVal[i] = posPtr[i];
                }
            }
        }
        if (GetAsyncKeyState(VK_TAB) & 1) {
            for (int i = 0; i < 3; i++) {
                 posPtr[i] = saveVal[i];
            }
        }

        if (GetAsyncKeyState(VK_F4) & 1) {

            if (posPtr == nullptr) {
                calculatePositionAddress();
            }

            else {
                for (int i = 0; i < 3; i++) {
                    wayPoint[i] = posPtr[i];
                }
            }
        }
        if ( (GetAsyncKeyState(VK_F1) & 1) && posPtr != nullptr) {

            for (int i = 0; i < 3; i++) {
                posPtr[i] = wayPoint[i];
            }
        }


        if (GetAsyncKeyState(VK_LEFT) & 1) {

            baseMod -= 0x70;
            ammoPtr = (int*)baseMod;
        }

        if (GetAsyncKeyState(VK_RIGHT) & 1) {

            baseMod -= 0x70;
            ammoPtr = (int*)baseMod;
        }








        if (accum2 >= 1'000'000'000 && showWindow) {

            if (posPtr != nullptr) {
                for (int i = 0; i < 3; i++) {
                    std::stringstream ss;
                    ss << posPtr[i];

                    components.SetText(7 + (2 * i), ss.str().c_str());

                }
            }

           

            accum2 = 0;
        }


        

        

        if (isUnliAmmo) {
            
            if (ammoPtr != nullptr)
                *ammoPtr = 99;

            if (accum >= 1'000'000'000 && showWindow) {
                std::stringstream ss;
                ss << "0x" << std::hex << ammoPtr;

                components.SetText(1, ss.str().c_str());
                ss.clear();
                ss.str("");
                ss << std::dec;
                ammoPtr != nullptr ? ss << *ammoPtr : ss << 0;
                components.SetText(3, ss.str().c_str());
                accum = 0;
               
            }

            
           
        }


        
        






        window.SwapWindowBuffers();
        
        if (showWindow) {
            window.ProcessMessage();
            auto t2 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> duration = t2 - t1;
            milli = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
            accum += milli;
            accum2 += milli;
        }
        
    }
   
    
  
    FreeLibraryAndExitThread(Hmodule, 0);
    return 0;
}





BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
) {




    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH: {
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ThreadCall, hModule, 0, nullptr));

    }


    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
