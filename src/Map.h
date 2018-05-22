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
    Edge(const Node* a, const Node* b, int len):
        a(a), b(b), len(len) {}

    const Node *a, *b;
    int len;
};

/**
 * Graph data structure
 * 1. Load from file.
 * 2. Get node by query nearest position.
 * 3. Find shortest path.
 * **/
class Map
{
public:
    Map();
    virtual ~Map();

    /// Get the nearest node by (x, y)
    const Node* getNode(double x, double y) const;
    /// Get node by id
    const Node* getNode(size_t id) const {
        return id >= m_node_count ? nullptr : m_nodes[id];
    }

    /// return a distance on a roadmap
    double roadmap_distance(const Node *a, const Node *b) const;
    
    /// recover a shortest path on roadmap given src and dst
    int recover_roadmap_path(const Node *a, const Node *b, std::vector<const Node*> &order) const;

    /// Load from file
    void load(const std::string& nodeDataFile, const std::string& edgeDataFile, const std::string& indexFile);

private:
    int m_node_count, m_edge_count;
    std::vector<const Node*> m_nodes;
    std::vector<const Edge*> m_edges;
};

#endif // MAP_H
