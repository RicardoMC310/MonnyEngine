#include "../../include/Engine/Engine.hpp"

int main(int argc, char **argv)
{
    Tasq::Logger logger("Launcher");

    try
    {

        sol::state lua;

        logger.info("Iniciando o Launcher.");

        if (argc < 2)
        {
            logger.critical("Nenhum argumento fornecido. Uso: {} <caminho_para_arquivo_lua", argv[0]);
        }

        Tasq::Engine engine;

        engine.runArchive(argv[1]);

        engine.registerFuncs();

        engine.run();

    }
    catch(const std::exception& e)
    {
        logger.critical("Ocorreu um erro: {}", e.what());
        return 1;
    }
    

    return 0;
}