#include <algorithm>
#include <cstdio>
#include <assert.h>

#include "InternetTravel.h"

using namespace std;

const char CAR_FNAME[] = "car.txt";
const char NODE_FNAME[] = "road.cnode";
const char EDGE_FNAME[] = "road.nedge";
const char INDEX_FNAME[] = "index.data";

InternetTravel::InternetTravel() : m_map(new Map()) {}

InternetTravel::~InternetTravel()
{
    delete m_map;
    for(auto car : m_cars) delete car;
}

void InternetTravel::loadCars(const string& dataFile)
{
    printf("Loading cars data...\n");

    m_cars.clear();

    FILE* f = fopen(dataFile.c_str(), "r");

    int i, id, n, nodeId;
    double x, y;
    int flag = 0;
    for (i = 0;; i++)
    {
        flag = fscanf(f, "%d %d %lf,%lf,%d", &id, &n, &x, &y, &nodeId);
        if(flag == EOF) break;
        assert(i == id);

        const Node* node = m_map->getNode(nodeId);
        assert(node != nullptr && node->x == x && node->y == y);

        std::vector<const Node*> passengers;
        for (int j = 0; j < n; j++)
        {
            fscanf(f, "%lf,%lf,%d", &x, &y, &nodeId);
            const Node* node = m_map->getNode(nodeId);
            assert(node != nullptr && node->x == x && node->y == y);
            passengers.push_back(node);
        }

        Vehicle* car = new Vehicle(id, node, passengers);
        m_cars.push_back(car);
    }
    fclose(f);
}

void InternetTravel::init(const string& dataDir)
{
    printf("Internet traffic system is starting...\n");

    m_map->load(dataDir + "/" + NODE_FNAME, dataDir + "/" + EDGE_FNAME, dataDir + "/" + INDEX_FNAME);
    loadCars(dataDir + "/" + CAR_FNAME);
}


SolutionList InternetTravel::query(double st_x, double st_y, double ed_x, double ed_y) {
    const Node *src = NULL, *dst = NULL;
    src = m_map->getNode(st_x, st_y);
    dst = m_map->getNode(ed_x, ed_y);
    return query(src, dst);
}


SolutionList InternetTravel::query(const Node* src, const Node* dst)
{
    printf("Current passenger's route: %s -> %s\n", src->toString().c_str(),
           dst->toString().c_str());

    SolutionList all, res;
    for (auto car : m_cars) {
        Solution sol = car->query(src, dst, m_map);
        if (sol.isOk())
            all.push_back(sol);
        if (all.size() >= 100)
            break;
    }
    printf("%d\n", all.size());
    sort(all.begin(), all.end());

    for (int i = 0; i < 5 && i < all.size(); i++)
        res.push_back(all[i]);

    return res;
}
