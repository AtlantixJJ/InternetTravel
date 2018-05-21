#include "InternetTravel.h"

int main()
{
    InternetTravel* engine = new InternetTravel();

    engine->startup("../data");

    int srcId = 2333, dstId = 2334;
    const Node* src = engine->getMap()->getNode(srcId);
    const Node* dst = engine->getMap()->getNode(dstId);
    SolutionList res = engine->query(src, dst);

    for (auto sol : res)
        sol.print();

    delete engine;


    return 0;
}
