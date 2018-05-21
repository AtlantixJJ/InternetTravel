#ifndef MAP_H
#define MAP_H

#include <string>

#include "Node.h"

/// These functions are defined in GPTree
// 从index文件中
extern void tree_build_load(const char *fname);
extern void tree_build_save(const char *fname);
extern int tree_search(int S, int T);
extern void tree_setup(const char *edge_file, const char *node_file, const char *index_file, bool is_load);
extern int tree_find_path(int S, int T, std::vector<int> &order);

struct Edge
{
    Edge(const Node* a, const Node* b, int len) : a(a), b(b), len(len) {}

    const Node *a, *b;
    int len;
};

typedef std::vector<const Edge*> EdgeList;

class Map
{
public:
    Map();
    virtual ~Map();

    /// Get the nearest node by (x, y)
    const Node* getNode(double x, double y) const {
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

    const Node* getNode(size_t id) const
    {
        return id >= m_node_count ? nullptr : m_nodes[id];
    }

    double distance(const Node* a, const Node* b) const;

    double roadmap_distance(const Node *a, const Node *b) const;

    int recover_roadmap_path(const Node *a, const Node *b, std::vector<const Node*> &order) const;

    void load(const std::string& nodeDataFile, const std::string& edgeDataFile, const std::string& indexFile);

private:
    int m_node_count, m_edge_count;
    std::vector<const Node*> m_nodes;
    EdgeList m_edges;
};

#endif // MAP_H
