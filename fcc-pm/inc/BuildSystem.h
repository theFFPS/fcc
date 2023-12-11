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
        settings = {
            { "cxx", JSON["settings"]["cxx"] },
            { "cxx_opts", JSON["settings"]["cxx_opts"] }, 
            { "linker", JSON["settings"]["linker"] }, 
            { "linker_opts", JSON["settings"]["linker_opts"] },
            { "static", JSON["settings"]["static"] }, 
            { "static_opts", JSON["settings"]["static_opts"] }
        };
        for (auto subproject : JSON["subprojects"]) {
            f.open(subproject + "/build.json");
            while (f.good() && getline(f, line)) buff += line + "\n";
            f.close();
            JSON = json::parse(buff);
            Submodule module;
            module.module = JSON["module"];
            for (auto val : JSON["incDirs"]) module.incDirs.push_back(val);
            for (auto val : JSON["libDirs"]) module.libDirs.push_back(val); 
            for (auto val : JSON["objs"])   module.objs.push_back({{"type",{val["type"][0]}}, {"ext",{val["ext"][0]}}, {"name",{val["name"][0]}}});
            for (auto val : JSON["outs"])                { 
                std::vector<std::string> objs, libs;
                for (auto val0 : val["objs"]) objs.push_back(val0);
                for (auto val0 : val["objs"]) libs.push_back(val0);
                 module.outs.push_back({{"type",{val["type"][0]},{"objs",objs, {"libs",libs, {"name",{val["name"][0]}}});
            }
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