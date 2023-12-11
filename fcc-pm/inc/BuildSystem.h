#ifndef __BUILDSYSTEM_FCCPM__
#define __BUILDSYSTEM_FCCPM__
#include <iostream>
#include <map>
#include <vector>
struct Submodule {
    std::map<std::string, std::string> settings;
    std::vector<std::map<std::string, std::vector<std::string>>> objs, outs;
    std::string module;
    std::vector<std::string> libDirs, incDirs;
    inline void build() {
        
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