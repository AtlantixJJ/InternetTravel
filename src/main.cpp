#include "InternetTravel.h"

/// Need to pass in : ./main <data_dir> <index_fname>
int main(int argc, char **argv)
{
    InternetTravel* engine = new InternetTravel();

    engine->init(argv[1]);

    int srcId = 12345, dstId = 12354;
    const Node* src = engine->getMap()->getNode(srcId);
    const Node* dst = engine->getMap()->getNode(dstId);
    SolutionList res = engine->query(src, dst);

    for (auto sol : res)
        sol.print();
    printf("Total sol: %d\n", res.size());
    delete engine;


    return 0;
}
