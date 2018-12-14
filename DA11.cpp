#include <boost/graph/adjacency_list.hpp>
#include <boost\graph\breadth_first_search.hpp>
#include <boost\graph\depth_first_search.hpp>
#include <boost\graph\transpose_graph.hpp>
#include <iostream>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> Graph;
typedef boost::graph_traits<Graph>::adjacency_iterator AdjacencyIterator;

int cont;
bool isAr = true;

std::vector<bool> aux;

//default_bfs_visitor extended class that overwrites discover_vertex and examine_edge functions

class custom_bfs_visitor : public boost::default_bfs_visitor {
public:
    template < typename Vertex, typename Graph >
    void discover_vertex(Vertex u, Graph &g) { //all reachable
        cont++;
        aux[vertex(u, g)] = true;
    }

    template < typename Edge, typename Graph >
    void examine_edge(Edge e, Graph &g) { //only one way to each vertex
        auto ex = target(e, g);
        if (aux[ex] == true)
            isAr = false;

    }
};

int main() {
    Graph g, invG;

    int nodes, edges, v1, v2;

    std::cin >> nodes >> edges;
    
    for (int i = 0; i < edges; i++) {
        std::cin >> v1 >> v2;
        auto e = add_edge(v1, v2, g);
    }

    aux.assign(nodes, false);

    //O(V + E) 
    transpose_graph(g, invG);

    Graph::vertex_iterator i, end;
    AdjacencyIterator ai, a_end;

    int vertCount;
    int root = 0;
    int rootValue = -1, rootAux = 0;

    //Check adjacent_vertices looking for roots(a root has 0 neighbors)
    //Total O(n2)

    for (boost::tie(i, end) = boost::vertices(invG); (i != end) || (root > 1); i++, rootAux++) {
        vertCount = 0;
        boost::tie(ai, a_end) = boost::adjacent_vertices(*i, invG);
        for (; ai != a_end; ai++) {
            vertCount++;
        }

        if (vertCount == 0) {
            root++;
            rootValue = rootAux;
        }

    }

    //O(V + E) 
    if (root == 1) {
        custom_bfs_visitor vis;
        breadth_first_search(g, vertex(rootValue, g), visitor(vis));  

        if (cont == nodes) {
            if (isAr) std::cout << "SI " << rootValue << std::endl;
            else std::cout << "NO" << std::endl;
        }

        else std::cout << "NO" << std::endl;
        
    }

    else std::cout << "NO" << std::endl;

    system("PAUSE");
}