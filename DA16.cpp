/* 
This example couldn't be solved correctly
because we haven't found a way to return multiple short paths using this library.

Instead, we have approximated the solution to return the entire path of a path
*/
#include <boost/config.hpp>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/properties.hpp>

#include <boost/property_map/property_map.hpp>

#include <iostream>
#include <utility>
#include <vector>

using namespace std;

int main(int, char *[])
{
	typedef boost::property<boost::edge_weight_t, float> EdgeWeightProperty;

	typedef boost::adjacency_list < boost::listS, boost::vecS, boost::undirectedS,
		boost::no_property, EdgeWeightProperty > Graph;

	typedef boost::graph_traits < Graph >::vertex_descriptor vertex_descriptor;
	typedef boost::graph_traits < Graph >::edge_descriptor edge_descriptor;
	typedef std::pair<int, int> Edge;

	int num_intersections, num_streets, weight;
	int intersection1, intersection2;
	int lucasTown;

	//Graph Creation
	Graph g;


	cin >> num_intersections;
	cin >> num_streets;

	//time complexity is O(num_streets)
	for (int i = 0; i < num_streets; i++) {
		cin >> intersection1;
		if (i == 0) lucasTown = intersection1;
		cin >> intersection2;
		cin >> weight;
		EdgeWeightProperty weight(weight);
		boost::add_edge(intersection1, intersection2, weight, g);

	}

	// Properties to use Dijkstra
	std::vector<vertex_descriptor> parents(boost::num_vertices(g)); // To save the predecessors
	std::vector<int> distance(boost::num_vertices(g)); // To save the distances

	// From 0 to all vertex and save in parents && distances //time complexity is O(num_vertices * log num_vertices + num_streets).
	boost::dijkstra_shortest_paths(g, lucasTown, boost::predecessor_map(&parents[0]).distance_map(&distance[0]));
	
	int i = num_intersections;
	std::cout << "Short Way:" << std::endl;
	//time complexity is O(num_intersections)
	while ( i != lucasTown) {
		std::cout << i << std::endl;
		i = parents[i];
	}
	std::cout << lucasTown << std::endl;

	std::cout << std::endl;
	system("pause");
	return EXIT_SUCCESS;
}