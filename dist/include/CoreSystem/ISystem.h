//
// Created by ricardo on 17/07/25.
//

#ifndef ISYSTEM_H
#define ISYSTEM_H

namespace Monny {
class CoreSystem;

class ISystem {
public:
    CoreSystem* onwer;

    ISystem() = default;
    virtual ~ISystem() = default;
    virtual void run() = 0;
};

} // Monny

#endif //ISYSTEM_H
