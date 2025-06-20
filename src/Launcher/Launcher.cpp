#include "../../include/Engine/Engine.hpp"

int main(int argc, char **argv)
{

    try
    {
        Tasq::Logger logger("Launcher");

        logger.info("Iniciando o Launcher.");

        Tasq::Engine engine;

        

    }
    catch(const std::exception& e)
    {
        return 1;
    }
    

    return 0;
}