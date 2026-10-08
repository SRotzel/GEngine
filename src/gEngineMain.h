#include <iostream>
#ifdef G_ENGINE_MAIN 
int main(int argc, char** argv);

#ifdef G_ENTRY_POINT

#include "gEngine.h"
int main(int argc, char** argv) {
    std::cout << "Starting gEngine...\n";
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
    std::cout << "[WARNING] (G_ENTRY_POINT) NOT DEFINED\n";
}

#endif
#endif