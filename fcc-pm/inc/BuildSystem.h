#ifndef __BUILDSYSTEM_FCCPM__
#define __BUILDSYSTEM_FCCPM__
#include <iostream>
#include <map>
#include <vector>
#include <thread>
inline void buildObj(std::vector<std::string> const incDirs, std::map<std::string, std::string> const settings, std::map<std::string, std::vector<std::string>> const obj) {
    
}
struct Submodule {
    std::map<std::string, std::string> settings;
    std::vector<std::map<std::string, std::vector<std::string>>> objs, outs;
    std::string module;
    std::vector<std::string> libDirs, incDirs;
    inline void build() {
        std::cout << "===> Started building " << module << "!\n";
        for (std::map<std::string, std::vector<std::string>> obj : objs) {
            std::cout << "===> Building " << obj["name"][0] << ".o\n";
            std::thread t (buildObj, incDirs, settings, obj);
            t.join();
        }
        std::cout << "===> Finished building " << module << "!\n";
    }
};
struct BuildSystem {
    std::vector<Submodule> submodules;
    std::string project;
    std::map<std::string, std::string> settings;
    inline void build() {
        std::cout << "===> Started building " << project << "!\n";
        for (Submodule submodule : submodules) {
            submodule.settings = settings;
            submodule.build();
        }
        std::cout << "===> Finished building " << project << "!\n";
    }
};
#endif //__BUILDSYSTEM_FCCPM__