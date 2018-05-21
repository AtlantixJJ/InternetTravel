#include "InternetTravel.h"

/// Need to pass in : ./create_index ../data ../data/index.data
int main(int argc, char **argv)
{
    InternetTravel* engine = new InternetTravel();
    engine->init(argv[1]);
    tree_build_save(argv[2]);
    delete engine;
    return 0;
}
