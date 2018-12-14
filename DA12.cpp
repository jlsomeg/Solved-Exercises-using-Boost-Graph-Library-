#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/array.hpp>

#include <array>
#include <utility>
#include <algorithm>
#include <iostream>

using namespace boost;
using namespace std;

//Custom vertex data; it will contain a char in order to identify the type of cell
struct VertexData {
	char car;
};

typedef boost::adjacency_list<boost::vecS, boost::vecS,
	boost::undirectedS, VertexData> Graph;
typedef graph_traits < Graph >::vertex_descriptor Vertex;

/*We needed extra Vertex descriptors so we know where we should start and where is the goal
 Also we needed a vector of booleans to know which one are conected or not*/
vector<bool> boolMatrix;
Vertex beginV, endV;

//function that connect vertex vertically and horizontally
void createGraph(vector<vector<char>> const &m, Graph &g, int f, int c);

//function that set sensors on matrix
void setSensors(vector<vector<char>> &m, int fila, int col, int s, int f, int c);


int solve(vector<vector<char>> const &matrix, int f, int c) {
	int sol = 0;
	Graph g;

	//O(f*c)
	//we need that for visitor, each vertex has row, column
	for (int i = 0; i < f; i++)
		for (int j = 0; j < c; j++) {
			Vertex aux = add_vertex(g);
			g[aux].car = matrix[i][j];
			if (matrix[i][j] == 'E')
				beginV = aux;
			if (matrix[i][j] == 'P')
				endV = aux;
		}

	createGraph(matrix, g, f, c);

	boost::array<int, 100000> distances;
	distances.assign(0);

	//O(E+V)
	boost::breadth_first_search(g, beginV, 
		boost::visitor(boost::make_bfs_visitor(boost::record_distances(distances.begin(),boost::on_tree_edge()))));

	if (distances[endV] == 0)
		cout << "NO" << endl;
	else
		cout << distances[endV] << endl;

	return 0;
}

bool solveCase() {

	int f, c;
	cin >> c;
	if (!cin)
		return true;
	cin >> f;

	vector<vector<char>> matrix;
	vector<char> auxV;
	pair<int, int> auxP;
	vector<pair<int, int>> sensors; //record where sensors are
	boolMatrix.assign(f*c, false);

	char auxC;
	for (int i = 0; i < f; i++) {
		for (int j = 0; j < c; j++) {
			cin >> auxC;
			if (auxC > '0' && auxC <= '9') { //record the sensors positions
				auxP = make_pair(i, j);
				sensors.push_back(auxP);
			}
			auxV.push_back(auxC);
		}
		matrix.push_back(auxV);
		auxV.clear();
	}

	for (int i = 0; i < sensors.size(); ) {
		auxP = sensors.back();
		int sensor = matrix[auxP.first][auxP.second] - '0';
		setSensors(matrix, f, c, sensor, auxP.first, auxP.second);
		sensors.pop_back();
	}

	sensors.clear(); //clear sensors for next example

	solve(matrix, f, c);

	return false;
}


void setSensors(vector<vector<char>> &m, int fila, int col, int s, int f, int c) {
	int i = c - 1;
	int cont = 0;
	bool muro = false;

	while (i > -1 && !muro && cont < s) {//Left
		if (m[f][i] == '#') {
			muro = true;
		}
		else {
			if (m[f][i] >= '0' && m[f][i] <= '9') {
				i--;
				cont++;
			}
			else {
				m[f][i] = 'X';
				cont++;
				i--;
			}
		}
	}

	cont = 0;
	i = f - 1;
	muro = false;
	while (i > -1 && !muro && cont < s) {//Up
		if (m[i][c] == '#') {
			muro = true;
		}
		else {
			if (m[i][c] >= '0' && m[i][c] <= '9') {
				i--;
				cont++;
			}
			else {
				m[i][c] = 'X';
				cont++;
				i--;
			}
		}
	}

	cont = 0;
	i = c + 1;
	muro = false;
	while (i < col && !muro && cont < s) {//Right
		if (m[f][i] == '#') {
			muro = true;
		}
		else {
			if (m[f][i] >= '0' && m[f][i] <= '9') {
				i++;
				cont++;
			}
			else {
				m[f][i] = 'X';
				cont++;
				i++;
			}
		}
	}

	cont = 0;
	i = f + 1;
	muro = false;
	while (i < fila && !muro && cont < s) {//Down
		if (m[i][c] == '#') {
			muro = true;
		}
		else {
			if (m[i][c] >= '0' && m[i][c] <= '9') {
				i++;
				cont++;
			}
			else {
				m[i][c] = 'X';
				cont++;
				i++;
			}
		}
	}
}

void createGraph(vector<vector<char>> const &m, Graph &g, int f, int c) {

	float weight = 1;

	for (int i = 0; i < f; i++) {
		for (int j = 0; j < c; j++) {
			if (m[i][j] == '.' || m[i][j] == 'E' || m[i][j] == 'P') {
				if (j != c - 1) {
					if (m[i][j + 1] == '.' || m[i][j + 1] == 'E' || m[i][j + 1] == 'P') {
						auto e = add_edge(i * c + j, i * c + j + 1, g);

					}
				}
				if (i != f - 1) {
					if (m[i + 1][j] == '.' || m[i + 1][j] == 'E' || m[i + 1][j] == 'P') {
						auto e = add_edge(i * c + j, (i + 1) * c + j, g);
					}
				}
			}
		}
	}

}

int main()
{
	
	solveCase();
	

	system("PAUSE");
}