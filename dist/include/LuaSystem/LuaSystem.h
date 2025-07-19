//
// Created by ricardo on 17/07/25.
//

#ifndef LUASYSTEM_H
#define LUASYSTEM_H
#include "CoreSystem/ISystem.h"
#include "Logger/Logger.h"
#include <sol/sol.hpp>

namespace Monny {

class LuaSystem : public ISystem {
    Logger logger{"LuaSystem"};
    Logger luaLogger{"LuaFile"};
    std::string pathFileLua;
    sol::state lua;

public:
    explicit LuaSystem(const std::string& path);
    void run() override;

    void createUserTypes();
    void checkFunctionExists();
};

} // Monny

#endif //LUASYSTEM_H
