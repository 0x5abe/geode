#include <Geode/DefaultInclude.hpp>

#include <Geode/loader/Dirs.hpp>
#include "../load.hpp"
#include <dlfcn.h>
#include <mach-o/dyld.h>
#include <unistd.h>

#include <thread>

using namespace geode::prelude;

// void dynamicEntry() {
//     auto dylib = dlopen("GeodeBootstrapper.dylib", RTLD_NOLOAD);
//     dlclose(dylib);

//     auto workingDir = std::filesystem::path(dirs::getGameDir());
//     auto libDir = workingDir / "Frameworks";
//     auto updatesDir = workingDir / "geode" / "update";

//     auto error = std::error_code();

//     if (std::filesystem::exists(updatesDir / "GeodeBootstrapper.dylib", error) && !error) {
//         std::filesystem::rename(
//             updatesDir / "GeodeBootstrapper.dylib", libDir / "GeodeBootstrapper.dylib", error
//         );
//         if (error) return;
//     }

//     geodeEntry(nullptr);
// }

// extern "C" __attribute__((visibility("default"))) void dynamicTrigger() {
//     std::thread(&dynamicEntry).detach();
// }


// remove when we can figure out how to not remove it
// auto dynamicTriggerRef = &dynamicTrigger;


static void(*s_applicationDidFinishLaunchingOrig)(void*, SEL, void*, void*);

void applicationDidFinishLaunchingHook(void* self, SEL sel, void* p1, void* p2) {
    // updateGeode();

    int exitCode = geodeEntry(nullptr);
    if (exitCode != 0)
        return;

    return s_applicationDidFinishLaunchingOrig(self, sel, p1, p2);
}


bool loadGeode() {
    auto orig = geode::hook::replaceObjcMethod("AppController", "application:didFinishLaunchingWithOptions:", (void*)applicationDidFinishLaunchingHook);
    if (!orig)
        return false;

    s_applicationDidFinishLaunchingOrig = reinterpret_cast<void(*)(void*, SEL, void*, void*)>(orig.unwrap());
    return true;
}

__attribute__((constructor)) void _entry() {
    if (!loadGeode())
        return;
}
