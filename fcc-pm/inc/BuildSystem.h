#ifndef __BUILDSYSTEM_FCCPM__
#define __BUILDSYSTEM_FCCPM__
#include <iostream>
#include <map>
#include <vector>
#include <thread>
#include <fstream>
#include "json.h"
using namespace nlohmann;
inline void buildObj(std::string const  module, std::vector<std::string> const incDirs, std::map<std::string, std::string> const settings, std::map<std::string, std::vector<std::string>> const obj) {
    std::string incDirsS;
    for (sts::string const incDir : incDirs) incDirsS += " -I./" + incDir;
    if (obj["type"][0] == "cxx" ||  obj["type"][0] == "cpp" || obj["type"][0] == "c++") system((settings["cxx"] + " " + settings["cxx_opts"] + incDirsS + " -c -o build/objects/" + module + "/" + obj["name"][0] + ".o " + module + "/" + obj["name"][0] + "." + obj["ext"][0]).c_str());
}
struct Submodule {
    std::map<std::string, std::string> settings;
    std::vector<std::map<std::string, std::vector<std::string>>> objs, outs;
    std::string module;
    std::vector<std::string> libDirs, incDirs;
    inline void build() {
        system(("mkdir -p build/{objects,executables,libraries}/" + module).c_str());
        std::cout << "===> Started building " << module << "!\n";
        for (std::map<std::string, std::vector<std::string>> obj : objs) {
            std::cout << "===> Building " << obj["name"][0] << ".o\n";
            std::thread t (buildObj, module, incDirs, settings, obj);
            t.join();
        }
        std::cout << "===> Finished building " << module << "!\n";
    }
};
struct BuildSystem {
    std::vector<Submodule> submodules;
    std::string project;
    std::map<std::string, std::string> settings;
    inline void setup() {
        std::string buff, line;
        std::fstream f ("build/config.json");
        while (f.good() && getline(f, line)) buff += line + "\n";
        f.close();
        json JSON = json::parse(buff);
        project = JSON["project"];
        for (auto subproject : JSON["subprojects"]) {
            f.open(subproject + "/build.json");
            while (f.good() && getline(f, line)) buff += line + "\n";
            f.close();
            JSON = json::parse(buff);
            Submodule module;
            module.module = JSON["module"];
            submodules.push_back(module);
        }
    }
    inline void build() {
        system("mkdir -p build/{objects,executables,libraries}");
        std::cout << "===> Started building " << project << "!\n";
        for (Submodule submodule : submodules) {
            submodule.settings = settings;
            submodule.build();
        }
        std::cout << "===> Finished building " << project << "!\n";
    }
};
#endif //__BUILDSYSTEM_FCCPM__