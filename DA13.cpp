#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <iostream>
#include <fstream>
#include <vector>

using namespace boost;
using namespace std;

/*Custom vertex data; it will contain two ints in order to identify the coordinate of cell*/

struct VertexData {
	int f, c;
};

typedef adjacency_list < vecS, vecS, undirectedS, VertexData, no_property> Graph;
typedef graph_traits < Graph >::vertex_descriptor Vertex;

/*We needed a global counter for sheeps
Also we needed a vector of booleans to know which ones are conected or not*/

vector<bool> boolMatrix;
int c;

/*Function that connects a vertex with the right and below vertices if they should be connected */

void createGraph(vector<vector<char>> const &matrix, int i, int j, Graph &g, int F, int C);

/*This is a custom visitor that turns true the value on boolMatrix that corresponds with the vertex on the graph*/

class custom_bfs_visitor : public default_bfs_visitor {
public:
	template < typename Vertex, typename Graph >
	void discover_vertex(Vertex u, const Graph & g) {
		boolMatrix[g[u].f*c + g[u].c] = true;
	}
};

int solve(vector<vector<char>> const &matrix, int f, int c) {
	int sol = 0;
	Graph g;
	boolMatrix.assign(f*c, false);

	//O(f*c)
	//storing in vertex the references for matrix
	for (int i = 0; i < f; i++)
		for (int j = 0; j < c; j++) {
			Vertex aux = add_vertex(g);
			g[aux].f = i;
			g[aux].c = j;
		}

	//connecting all vertices
	for (int i = 0; i < f; i++) {
		for (int j = 0; j < c; j++) {
			if (matrix[i][j] == '.') {
				createGraph(matrix, i, j, g, f, c);
			}
		}
	}

	custom_bfs_visitor vis;
	/*In this call we set as trye all vertices that are WHITE out of the sheeps; the X cells where not connected, so it doesn't go through them*/
	breadth_first_search(g, 0, visitor(vis));
	//O(f*c + n * v) wjere f = row, c = column, n = number of sheeps, v = vertices connected to that vertex
	for (int i = 0; i < f; i++) {
		for (int j = 0; j < c; j++) {
			if (matrix[i][j] == '.') {
				int pos = i*c + j;
				if (!boolMatrix[pos]) {
					sol++;
					breadth_first_search(g, pos, visitor(vis));
				}
			}
		}
	}
	return sol;
}

bool solveCase() {

	int f;
	cin >> c;
	if (!cin)
		return true;
	cin >> f;

	vector<vector<char>> matrix;
	vector<char> auxV;

	char auxC;
	for (int i = 0; i < f; i++) {
		for (int j = 0; j < c; j++) {
			cin >> auxC;
			auxV.push_back(auxC);
		}
		matrix.push_back(auxV);
		auxV.clear();
	}

	cout << solve(matrix, f, c) << endl;

	return false;
}

void createGraph(vector<vector<char>> const &matrix, int i, int j, Graph &g, int F, int C) {

	/*Both use the F and C as auxiliar since the container of vertices is a vector, so we need to calculate the position to connect them*/

	/*check if wheter we should connect the vertices or not with the right*/
	if (j != C - 1) {
		if (matrix[i][j + 1] == '.') {
			add_edge(i * C + j, i * C + j + 1, g);
		}
	}
	/*check if wheter we should connect the vertices or not with the below*/
	if (i != F - 1) {
		if (matrix[i + 1][j] == '.') {
			add_edge(i * C + j, (i + 1) * C + j, g);
		}
	}

}

int main() {

	solveCase();

	system("pause");
	return EXIT_SUCCESS;
}