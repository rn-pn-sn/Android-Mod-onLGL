#include <list>
#include <vector>
#include <cstring>
#include <pthread.h>
#include <thread>
#include <cstring>
#include <string>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <dlfcn.h>
#include "Includes/Logger.h"
#include "Includes/obfuscate.h"
#include "Includes/Utils.hpp"
#include "Includes/Macros.h"
#include "dobby.h"

//Target main lib here
#define targetLibName OBFUSCATE("libil2cpp.so")

int scoreMul = 1, coinsMul = 1;
bool btnPressed = false;

//CharacterPlayer
void (*StartInvcibility)(void *instance, float duration);

void (*old_Update)(void *instance);
void Update(void *instance) {
    if (instance != nullptr) {
        if (btnPressed) {
            StartInvcibility(instance, 30);
            btnPressed = false;
        }
    }
    return old_Update(instance);
}

/*
 void (*old_AddScore)(void *instance, int score);
 void AddScore(void *instance, int score) {
    //default any actions
    return old_AddScore(instance, score * scoreMul);
 }
*/
// === This function was completely replaced with `install_hook_name` from dobby.h ===
// (base name, return type, ... args)
install_hook_name(AddScore, void *, void *instance, int score) {
    // default any actions

    // use orig_ for call original function
    return orig_AddScore(instance, score + scoreMul);
}

void (*old_AddCoins)(void *instance, int count);
void AddCoins(void *instance, int count) {
    return old_AddCoins(instance, count * coinsMul);
}


// we will run our hacks in a new thread so our while loop doesn't block process main thread
void hack_thread() {
    while (!isLibraryLoaded(targetLibName)) {
        sleep(1); // Wait for target lib be loaded.
    }

    // In Android Studio, to switch between arm64-v8a and armeabi-v7a syntax highlighting,
    // You can modify the "Active ABI" in "Build Variants" to switch to another architecture for parsing.
#if defined(__aarch64__)
    // The patch have 2 options:
    // - (active) Dobby-Kitty implementation
    // - reworked KittyMemory implementation
    // if you encounter any problems:
    // - switch to Kitty implementation (uncomment code in Macros.h)
    // - uncommiting logging for detailed debug

    PATCH(targetLibName, "0x10709AC", "E05F40B2 C0035FD6");
    rPATCH(targetLibName, "0x107094D", "0x5F", "E05F40B2 C0035FD6");

    // alt possibles usage variants:
    // symbol, hex
    PATCH(targetLibName, "_example__sym", "E0 5F 40 B2 C0 03 5F D6");
    rPATCH(targetLibName, "_example__sym", "0x5F", "E0 5F 40 B2 C0 03 5F D6");
    // offset, asm
    PATCH(targetLibName, "0x10709AC", "mov x0, #0xffffff; ret");
    rPATCH(targetLibName, "0x107094D", "0x5F", "mov x0, #0xffffff; ret");
    // symbol, asm
    PATCH(targetLibName, "_example__sym", "mov x0, #0xffffff; ret");
    rPATCH(targetLibName, "_example__sym", "0x5F", "mov x0, #0xffffff; ret");

    //if need restore patch:
    RESTORE(targetLibName, "0x10709AC");
    rRESTORE(targetLibName, "0x10709AC", "0x5F");
    // or
    RESTORE(targetLibName, "_example__sym");
    rRESTORE(targetLibName, "_example__sym", "0x5F");


    //Il2Cpp: Use RVA offset
    StartInvcibility = (void (*)(void *, float)) getAbsoluteAddress(targetLibName, OBFUSCATE("0x107A3BC"));
    StartInvcibility = (void (*)(void *, float)) getAbsoluteAddress(targetLibName, OBFUSCATE("_characterPlayer_Update"));

    HOOK(targetLibName, "0x107A2FC", AddCoins, old_AddCoins);

    // HOOK(targetLibName, "0x107A2E0", AddScore, old_AddScore);
    // === This function was completely replaced with super-macro `install_hook_name` from dobby.h ===
    // don't forget set address for install_hook:
    // ! getAbsoluteAddress not have OBFUSCATE, so don't forget use his here
    install_hook_AddScore(getAbsoluteAddress(targetLibName,OBFUSCATE("0x107A2E0")));

    HOOK(targetLibName, "0x1078C44", Update, old_Update);
    //HOOK(targetLibName, "0x1079728", Kill, old_Kill);
    //HOOK(targetLibName, "_example__sym", Kill, old_Kill);
    //HOOK_NO_ORIG("libFileC.so", "0x123456", FunctionExample);
    //HOOK_NO_ORIG("libFileC.so", "_example__sym", FunctionExample);

    //PATCH(targetLibName, "0x10709AC", "E05F40B2C0035FD6");

    // you can use this for things as detect log, counting function calls, executing side code before the function is executed
    // now instrument wrapper implemented for detecting execution in logcat
    INST(targetLibName, "0x23558C", "AnyNameForDetect", true);

    // LOGI(OBFUSCATE("Test SYM: 0x%llx"), (uintptr_t)getAbsoluteAddress(OBFUSCATE("libil2cpp.so"), OBFUSCATE("il2cpp_init")));
#elif defined(__arm__)
    //Put your code here if you want the code to be compiled for armv7 only
#endif

    LOGI(OBFUSCATE("Done"));
}

__attribute__((constructor))
void lib_main() {
    std::thread(hack_thread).detach();
}