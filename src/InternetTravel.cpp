#include <algorithm>
#include <cstdio>
#include <assert.h>
#include "Const.h"
#include "InternetTravel.h"

using namespace std;

const char CAR_FNAME[] = "car.txt";
const char NODE_FNAME[] = "road.cnode";
const char EDGE_FNAME[] = "road.nedge";
const char INDEX_FNAME[] = "index.data";

InternetTravel::InternetTravel() : m_map(new Map()) {
    for(int i = 0; i <= 5; i ++){
        Const::write_permutations(0, i);
        Const::fac_cnt = 0;
        /*
        for(int j = 0; j < Const::FACTORIAL[i]; j ++) {
            for(int k = 0; k < i; k ++) {
                printf("%d ", Const::PERMUTATIONS[i][j][k]);
            }
            printf("\n");
        }
        printf("-----\n");
        */
    }
}

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

        /// Car position must be consistent with node file
        const Node* node = m_map->getNode(nodeId);
        assert(node != nullptr && node->x == x && node->y == y);

        std::vector<const Node*> passengers;
        for (int j = 0; j < n; j++)
        {
            fscanf(f, "%lf,%lf,%d", &x, &y, &nodeId);
            /// Passenger pos must be consistent with node file
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
    printf("Internet travel system is initializing...\n");

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
    SolutionList tmp, res;
    for (auto car : m_cars) {
        //printf("Car query.\n");
        Solution sol = car->query(src, dst, m_map);
        if (sol.is_valid()) {
            tmp.push_back(sol);
        } else {
            //printf("Invalid.\n");
        }
        if (tmp.size() >= 100)
            break;
    }
    printf("Total solution numer: %d\n", tmp.size());
    sort(tmp.begin(), tmp.end());

    for (int i = 0; i < 5 && i < tmp.size(); i++) {
        //printf("Recover path.\n");
        vector<const Node*> path = {tmp[i].path[0]};
        for (int j = 0; j < tmp[i].path.size() - 1; j ++)
        m_map->recover_roadmap_path(tmp[i].path[j], tmp[i].path[j+1], path);
        //printf("Recover done.\n");
        tmp[i].path = path;
        res.push_back(tmp[i]);
    }
        

    return res;
}
