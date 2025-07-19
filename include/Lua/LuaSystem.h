//
// Created by ricardo on 18/07/25.
//

#ifndef LUASYSTEM_H
#define LUASYSTEM_H
#include <sol/sol.hpp>
#include <utility>
#include "Core/ISystem.h"

namespace Monny {

class LuaSystem final : public ISystem {
    std::string pathLuaFile;
    sol::state lua;
    Logger luaLogger{"LuaFile"};

    void setNewUserTable();
    void createFunctions();
    void onDistribitionUserEvents();

public:
    LuaSystem(const std::string& path): pathLuaFile(path) {}
    ~LuaSystem() = default;

    void start() override;
    void run() override;
    void end() override;
};

} // Monny

#endif //LUASYSTEM_H
