	
/* 
Added in addition to the solution, 
additional information on the way traveled and distances from it.
*/
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

	typedef boost::adjacency_list < boost::listS, boost::vecS, boost::directedS,
		boost::no_property, EdgeWeightProperty > Graph;

	typedef boost::graph_traits < Graph >::vertex_descriptor vertex_descriptor;
	typedef boost::graph_traits < Graph >::edge_descriptor edge_descriptor;
	typedef std::pair<int, int> Edge;

	int num_region, num_ways, weight;
	int intersection1, intersection2, office, packages, addressee;
	bool impossible = false;
	int totalDistance = 0;
	vector<int> addressees;


	//Graph creation
	Graph g;

	cin >> num_region;
	cin >> num_ways;

	for (int i = 0; i < num_ways; i++) {
		cin >> intersection1;

		cin >> intersection2;
		cin >> weight;
		EdgeWeightProperty weight(weight);
		boost::add_edge(intersection1, intersection2, weight, g);

	}
	cin >> office;
	cin >> packages;
	for (int i = 0; i < packages; i++)
	{
		cin >> addressee;
		addressees.push_back(addressee);
	}
	for (int i = 0; i < packages && (impossible == false); i++) {

		std::vector<vertex_descriptor> parents(boost::num_vertices(g)); // To save the predecessors
		std::vector<int> distance(boost::num_vertices(g)); // To save the distances

		boost::dijkstra_shortest_paths(g, office, boost::predecessor_map(&parents[0]).distance_map(&distance[0]));
		if (distance[addressees[i]] > 20000) impossible = true;
		
		if(!impossible){
			//GOING
			int j = addressees[i];
			std::cout << "Way in gone (inverted):" << std::endl;
			while (j != office)
			{
				std::cout << j << std::endl;
				j = parents[j];

			}
			std::cout << office << std::endl;

			std::cout << "Distance = " << distance[addressees[i]] << std::endl;
			totalDistance += distance[addressees[i]];

			boost::dijkstra_shortest_paths(g, addressees[i], boost::predecessor_map(&parents[0]).distance_map(&distance[0]));

			//Return
			j = office;
			std::cout << "Way back (inverted):" << std::endl;
			while (j != addressees[i])
			{
				std::cout << j << std::endl;
				j = parents[j];

			}
			std::cout << addressees[i] << std::endl;
			std::cout << "Distance = " << distance[office] << std::endl;
			totalDistance += distance[office];

			std::cout << std::endl;
		}


		
	}
	if (impossible)
		cout << "impossible" << endl;
	else
		cout << "TOTAL DISTANCE = " << totalDistance << endl;

	system("pause");
	return EXIT_SUCCESS;
}