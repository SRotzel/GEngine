#include <iostream>
#ifdef G_ENGINE_MAIN 
int main(int argc, char** argv);

#ifdef G_ENTRY_POINT

#include "gEngine.h"
int main(int argc, char** argv) {
    std::cout << "Starting Engine...\n";
    G_ENTRY_POINT *entry = new G_ENTRY_POINT();
    {
        gEngine *instance = new gEngine(entry);
	    instance->start();
	    instance->run();
	    instance->end();
        delete instance;
    }

    delete entry;
}

#else

int main(int argc, char** argv) {
    std::cout << "[WARNING] NO G_ENTRY_POINT DEFINED\n";
}

#endif
#endif