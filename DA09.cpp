#include <boost\graph\adjacency_list.hpp>
#include <boost\graph\depth_first_search.hpp>
#include <boost\graph\breadth_first_search.hpp>
#include <boost\graph\undirected_dfs.hpp>

#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>

int cont = 0;
int contMax = 0;

//vector que marca a true los vértices visitados / false los no visitados
std::vector<bool> vec;

//clase extendida de default_bfs_visitor para sobreescribir la función que se ejecuta en el 
//evento de discorver_vertex

class custom_bfs_visitor : public boost::default_bfs_visitor
{
public:

	template < typename Vertex, typename Graph >
	void discover_vertex(Vertex u, const Graph & g)
	{
		cont++;
		std::cout << "vertice: " << vertex(u, g) << std::endl;
		vec[vertex(u, g) - 1] = true;
	}
};

int main()
{
	typedef boost::adjacency_list < boost::vecS, boost::vecS, boost::undirectedS > graph_t;

	graph_t g;

	int cases, people, pairsOfFriends, pairsOfFriends1, pairsOfFriends2;

	//creación del grafo
	std::cin >> cases;
	while (cases != 0) {
		int i = 0;
		std::cin >> people;
		std::cin >> pairsOfFriends;
		do {
			std::cin >> pairsOfFriends1;
			std::cin >> pairsOfFriends2;
			add_edge(pairsOfFriends1, pairsOfFriends2, g);
		} while (++i < pairsOfFriends);

		custom_bfs_visitor vis;
		vec.assign(people, false); //inicializamos todas las posiciones del vector a false


		//realizamos búsqueda en anchura O(V + E), comenzando en todos los vértices que no se han visitado
		//de esta forma recorremos todos una sola vez
		for (int j = 1; j <= people; j++) {
			if (vec[j - 1] == false) {
				breadth_first_search(g, vertex(j, g), visitor(vis));
			}

			if (cont > contMax) contMax = cont;
			cont = 0;
		}

		//Obtenemos el máximo numero de vértices conectados entre sí
		cases--;
		std::cout << "Total: " << contMax << std::endl;
		cont = 0;
	}
	system("pause");
	return 0;

}