// ConsoleApplication1.cpp : Defines the entry point for the console application.
#include <boost\graph\adjacency_list.hpp>
#include <boost\graph\bipartite.hpp>

#include <iostream>
#include <iterator>
#include <algorithm>

int main() {

	using namespace boost;
	int n, a, aux1, aux2;

	std::cin >> n >> a;
	adjacency_list<vecS, vecS, undirectedS> grafo(n);

	//while(numcasos>0){

	/*creates a number of edges*/
	for (int i = 0; i < a; i++) {
		std::cin >> aux1 >> aux2;
		add_edge(aux1, aux2, grafo);
	}
	/*The time complexity for the algorithm is O(V + E), where V is number of vertices and E is number of edges*/
	bool is = is_bipartite(grafo);

	if(is)
		std::cout << "SI" << std::endl;
	else
		std::cout << "NO" << std::endl;

	//numcasos--}
	
	system("PAUSE");

	return 0;
}

