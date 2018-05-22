#include <cstdio>
#include <assert.h>

#include "Map.h"

using namespace std;

Map::Map() {}

Map::~Map()
{
    for (auto node : m_nodes)
        delete node;
    for (auto edge : m_edges)
        delete edge;
}

void Map::load(const string& nodeDataFile, const string& edgeDataFile, const std::string& indexFile)
{
    printf("Loading map data...\n");

    m_nodes.clear();
    m_edges.clear();

    FILE* f = fopen(nodeDataFile.c_str(), "r");
    int id, len, a, b;
    double x, y;

    for (int i = 0; fscanf(f, "%d %lf %lf", &id, &x, &y) != EOF; i++)
    {
        assert(i == id);
        m_nodes.push_back(new Node(id, x, y));
    }
    fclose(f);

    f = fopen(edgeDataFile.c_str(), "r");

    fscanf(f, "%d %d", &m_node_count, &m_edge_count);
    while (fscanf(f, "%d %d %d", &a, &b, &len) != EOF)
        m_edges.push_back(new Edge(getNode(a), getNode(b), len));

    fclose(f);
    //tree_setup(edgeDataFile.c_str(), nodeDataFile.c_str(), NULL, false);
    tree_setup(edgeDataFile.c_str(), nodeDataFile.c_str(), indexFile.c_str(), true);
}

const Node* Map::getNode(double x, double y) const {
    int i;
    double mini = 10000000.0, dist= 10000000.0;
    const Node *res = NULL, *cur = NULL;
    for(i = 0; i < m_nodes.size(); i++) {
        cur = m_nodes[i];
        dist = ((x - cur->x)) * ((x - cur->x));
        dist +=((y - cur->y)) * ((y - cur->y));

        //dist = (x - cur.x) * (x - cur.x) + (y - cur.y) * (y - cury);
        if(mini > dist) {
            mini = dist;
            res = cur;
        }
    }
    //printf("%.10lf\n", mini);
    //printf("%.10lf, %.10lf\n", res->x, res->y);
    return res;
}

double Map::roadmap_distance(const Node* a, const Node* b) const
{
    double res = tree_search(a->id, b->id) / 1000.0;
    return res;
}

int Map::recover_roadmap_path(const Node* a, const Node* b,
                              std::vector<const Node*>& order) const
{
    vector<int> ider;
    int dist = tree_find_path(a->id, b->id, ider);
    for (auto iter = ider.begin() + 1; iter != ider.end(); iter++)
    {
        order.push_back(m_nodes[*iter]);
    }
}
