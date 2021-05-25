#include "engine.h"

int main()
{   

    Engine engine(512, 512, "Conway's Game of Life");

    if(!engine.Initialize())
    {
        std::cout << std::endl << "Press any key to close program..." << std::endl;
        std::cin.get();
    }
}

