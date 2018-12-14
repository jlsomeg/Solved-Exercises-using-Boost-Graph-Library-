#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost\graph\breadth_first_search.hpp>

#include <iostream>
#include <fstream>


int cont = 0;

//default_bfs_visitor extended class that overwrites discover_vertex function
class custom_bfs_visitor : public boost::default_bfs_visitor
{
public:

	template < typename Vertex, typename Graph >
	void discover_vertex(Vertex u, const Graph & g)
	{
		cont++;
	}
};


int main() {
	using namespace boost;
	using namespace std;

	typedef adjacency_list < vecS, vecS, undirectedS,
		no_property, property < edge_weight_t, int > > Graph;
	typedef graph_traits < Graph >::edge_descriptor Edge;

	Graph g;

	int v, e, v1, v2, w;

	cin >> v >> e;

	for (int i = 0; i < e; i++) {
		cin >> v1 >> v2 >> w;
		add_edge(v1, v2, w, g);
	}

	//if we want all vertices connected, at least we need v-1 edges
	if (e >= v - 2) {

		custom_bfs_visitor vis;

		//O(E+V) Check if all vertices are reachable
		breadth_first_search(g, 1, visitor(vis));

		if (cont == v) {

			property_map < Graph, edge_weight_t >::type weight = get(edge_weight, g);
			std::vector < Edge > spanning_tree;

			//O(E log E) find a minimum spanning tree
			kruskal_minimum_spanning_tree(g, std::back_inserter(spanning_tree));

			int totalCost = 0;
			std::cout << "Print the edges in the MST:" << std::endl;
			for (std::vector < Edge >::iterator ei = spanning_tree.begin();
				ei != spanning_tree.end(); ++ei) {
				std::cout << source(*ei, g) << " <--> " << target(*ei, g) << " with weight of " << weight[*ei] << std::endl;
				totalCost = totalCost + weight[*ei];
			}
			std::cout << totalCost << std::endl;
		}

		else
			std::cout << "Imposible" << std::endl;
	}

	else
		std::cout << "Imposible" << std::endl;


	system("pause");
	return EXIT_SUCCESS;
}