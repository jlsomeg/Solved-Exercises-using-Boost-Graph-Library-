
#include <boost/config.hpp>

#include <algorithm>
#include <vector>
#include <utility>
#include <iostream>

#include <boost/graph/visitors.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/graph_utility.hpp>

using namespace boost;
using namespace std;


/*Aux variable that stores the number of columns so we can set true the boolMatrix*/
int c;

/*Custom visitor for bfs that counts in global variable counter and set matrix as true for that vertex's position*/

class custom_bfs_visitor : public boost::default_bfs_visitor
{
public:

	template < typename Vertex, typename Graph >
	void discover_vertex(Vertex u, const Graph & g)
	{
		cont++;
		boolMatrix[g[u].f*c + g[u].c] = true;
	}
};

/*Custom vertex data; it will contain a char in order to identify the type of cell and two ints to identify the relative position of cell in the vector*/

struct VertexData {
	char car;
	int f, c;
};

typedef boost::adjacency_list<
	boost::mapS, boost::vecS, boost::undirectedS, VertexData
> Graph;

typedef graph_traits < Graph >::vertex_descriptor Vertex;

/*We needed extra counters, a max one (to store the overall greater counter) and auxiliar one
Also we needed a vector of booleans to know which one are conected or not*/

vector<bool> boolMatrix;
int cont = 0, contMax = 0;

void createGraph(vector<vector<char>> const &m, Graph &g, int fil, int col) {

	/*For each cell, we check if surrounding cells are water or not,  connect them if they are oil*/

	for (int i = 0; i < fil; i++) {
		for (int j = 0; j < col; j++) {
			if (m[i][j] == '#') {

				//right check and connection
				if (j != col - 1) {
					if (m[i][j + 1] == '#') {
						auto e = add_edge(i * col + j, i * col + j + 1, g);

					}
				}

				//below check and connection
				if (i != fil - 1) {
					if (m[i + 1][j] == '#') {

						auto e = add_edge(i * col + j, (i + 1) * col + j, g);
					}
				}
				//right-below check and connection
				if ((j != col - 1) && (i != fil - 1)) {
					if (m[i + 1][j + 1] == '#') {
						auto e = add_edge(i * col + j, (i + 1) * col + (j + 1), g);

					}
				}
				//left-below check and connection
				if (j > 0 && (i != fil - 1)) {
					if (m[i + 1][j - 1] == '#') {
						auto e = add_edge(i * col + j, (i + 1) * col + (j - 1), g);

					}
				}
			}
		}
	}

}

/*This function executes N times after the cin of number of drops; it connects that cell with every surrounding cells if they are also oil*/

void nSearch(vector<vector<char>> &m, Graph &g, custom_bfs_visitor vis, int fil, int col) {

	int i, j;
	cin >> i >> j;
	i--, j--;

	/*Updates the char matrix and the graph's vertex property*/
	m[i][j] = '#';
	g[i * col + j].car = '#';

	if (j != col - 1) {
		if (m[i][j + 1] == '#') {
			//right cell
			auto e = add_edge(i * col + j, i * col + j + 1, g);

		}
	}

	if (j > 0) {
		if (m[i][j - 1] == '#') {
			//left cell
			auto e = add_edge(i * col + j, i * col + j - 1, g);

		}
	}

	if (i != fil - 1) {
		if (m[i + 1][j] == '#') {
			//below cell
			auto e = add_edge(i * col + j, (i + 1) * col + j, g);
		}
	}

	if (i > 0) {
		if (m[i - 1][j] == '#') {
			//above cell
			auto e = add_edge(i * col + j, (i - 1) * col + j, g);
		}
	}

	if (i > 0 && j > 0) {
		if (m[i - 1][j - 1] == '#') {
			//left-above cell
			auto e = add_edge(i * col + j, (i - 1) * col + (j - 1), g);
		}
	}

	if (i > 0 && (j != col - 1)) {
		if (m[i - 1][j + 1] == '#') {
			//right-above cell
			auto e = add_edge(i * col + j, (i - 1) * col + (j + 1), g);
		}
	}


	if ((j != col - 1) && (i != fil - 1)) {
		if (m[i + 1][j + 1] == '#') {
			//right-below cell
			auto e = add_edge(i * col + j, (i + 1) * col + (j + 1), g);

		}
	}

	if (j > 0 && (i != fil - 1)) {
		if (m[i + 1][j - 1] == '#') {
			//left-below cell
			auto e = add_edge(i * col + j, (i + 1) * col + (j - 1), g);

		}
	}
	/*all this checkings are cte time*/

	//now we have the graph updated with the new edges, so we have to do another bfs from that point in order to seize it;
	// if it's greater than the maximum stored, we have a new maximum

	//O(oilCells+E), since it only will, in worst case, go through all oil cells ; where oilcells is <= f*c and E the edges between them
	breadth_first_search(g, vertex(i*col + j, g), visitor(vis));
	cont > contMax ? contMax = cont, cont = 0 : cont = 0;

	cout << contMax << " ";
}

int firstSearch(vector<vector<char>> &matrix, int fil, int col) {
	int sol = 0;
	Graph g;
	int newDrops;

	//O(f*c)
	//need to add manually the vertex so I can set the properties such as the ints that records the relative position as row and columns and the char itself for that position
	for (int i = 0; i < fil; i++)
		for (int j = 0; j < col; j++) {
			Vertex aux = add_vertex(g);
			g[aux].car = matrix[i][j];
			g[aux].f = i;
			g[aux].c = j;
		}

	custom_bfs_visitor vis;

	createGraph(matrix, g, fil, col);

	/*The bfs entry condition is only here, where we could do more searches than needed (because we will do it in many vertices). 
	Later on nSearch we just don't check the boolMatrix since it will visit all posible (connected) vertices just one time with the search from a point we know.
	Here is different because we need to find first the biggest of them all*/

	/*O(oilCells)*/
	for (int i = 0; i < num_vertices(g); i++) {
		if (boolMatrix[i] == false) //remember that water cells are marked as TRUE, so no bfs is launched for them, neither for oil's cells already visited
			breadth_first_search(g, i, visitor(vis));
		cont > contMax ? contMax = cont, cont = 0 : cont = 0;
	}

	cout << contMax << " ";

	cin >> newDrops;

	for (int i = 0; i < newDrops; i++)
		nSearch(matrix, g, vis, fil, col);

	return cont;
}

bool resolverCaso() {

	int f;
	cin >> f;
	if (!cin)
		return true;
	cin >> c;

	/*these are some variables and their init*/
	vector<vector<char>> matrix;
	vector<char> auxV;
	boolMatrix.assign(f*c, false);
	cont = 0;

	char auxC;
	getchar();
	/*reading the chars...*/
	for (int i = 0; i < f; i++) {
		for (int j = 0; j < c; j++) {
			auxC = getchar();
			auxV.push_back(auxC);
			if (auxC != '#')
				boolMatrix[i*c + j] = true; //we set this position as TRUE so the bfs on firstSearch() only starts if it's an oil's cell
		}
		matrix.push_back(auxV);
		auxV.clear();
		getchar();
	}

	firstSearch(matrix, f, c);

	return false;
}

int main(int, char*[])
{

	resolverCaso();


	system("PAUSE");
	return 0;
}