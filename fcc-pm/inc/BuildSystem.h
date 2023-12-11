#ifndef __BUILDSYSTEM_FCCPM__
#define __BUILDSYSTEM_FCCPM__
#include <iostream>
#include <map>
#include <vector>
struct Submodule {
    std::map<std::string, std::string> settings;
    std::vector<std::map<std::string, std::vector<std::string>>> objs, outs;
};
struct BuildSystem {
    std::vector<Submodule> submodules;
    std::string project;
    std::map<std::string, std::string> settings;
};
#endif //__BUILDSYSTEM_FCCPM__