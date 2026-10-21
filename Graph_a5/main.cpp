#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <algorithm>
#include <climits>
using namespace std;

/*
Graph ADT (directed graph with costs)

Representation:
- outbound[x] = list of vertices y such that edge (x, y) exists
- inbound[y]  = list of vertices x such that edge (x, y) exists
- cost[x][y]  = cost of edge (x, y)
- vertices are integers: 0 ... n-1
*/

class Graph {
private:
	unordered_map<int, vector<int>> outbound;
	unordered_map<int, vector<int>> inbound;
	unordered_map<int, unordered_map<int, int>> cost;
	int n; // number of vertices

public:
	// Constructor: creates graph with n isolated vertices
	Graph(int n)
	{
		this->n = n;

		for (int i = 0; i < n; i++)
		{
			outbound[i] = {};
			inbound[i] = {};
		}
	}

	// Checks if edge (x, y) exists
	bool is_edge(int x, int y)
	{
		if (cost.find(x) == cost.end())
			return false;

		if (cost[x].find(y) == cost[x].end())
			return false;

		return true;

	}

	// Adds edge (x, y) with cost c
	// Preconditions: x,y exist, edge does not exist
	void add_edge(int x, int y, int c)
	{
		if (outbound.find(x) == outbound.end())
			throw runtime_error("Source vertex does not exist");

		if (outbound.find(y) == outbound.end())
			throw runtime_error("Target vertex does not exist");

		if (is_edge(x, y))
			throw runtime_error("Edge already exists");

		cost[x][y] = c;
		outbound[x].push_back(y);
		inbound[y].push_back(x);
	}

	// Returns list of all vertices
	vector<int> parse_vertices()
	{
		vector <int> v;
		for (auto element : outbound)
		{
			v.push_back(element.first);
		}
		return v;
	}

	// Returns outbound neighbors of x
	vector<int> parse_out(int x)
	{

		if (outbound.find(x) == outbound.end())
			throw runtime_error("Vertex does not exist");

		return outbound[x];
	}

	// Returns inbound neighbors of x
	vector<int> parse_in(int x)
	{

		if (inbound.find(x) == inbound.end())
			throw runtime_error("Vertex does not exist");

		return inbound[x];

	}

	// Returns cost of edge (x, y)
	int get_cost(int x, int y)
	{
		if (!is_edge(x, y))
			throw runtime_error("Edge does not exist");

		return cost[x][y];
	}

	// Returns number of vertices
	int number_of_vertices()
	{
		return outbound.size();
	}

	// Returns number of edges
	int number_of_edges()
	{
		int sum = 0;
		for (auto element : outbound)
			sum += element.second.size();

		return sum;

	}

	// Returns out-degree of x
	int out_degree(int x)
	{
		if (outbound.find(x) == outbound.end())
			throw runtime_error("Vertex does not exist");

		return outbound[x].size();

	}

	// Returns in-degree of x
	int in_degree(int x)
	{
		if (inbound.find(x) == inbound.end())
			throw runtime_error("Vertex does not exist");

		return inbound[x].size();

	}

	// Removes edge (x, y)
	void remove_edge(int x, int y)
	{

		if (outbound.find(x) == outbound.end())
			throw runtime_error("Source vertex does not exist");

		if (inbound.find(y) == inbound.end())
			throw runtime_error("Target vertex does not exist");

		if (!is_edge(x, y))
			throw runtime_error("Edge does not exist");

		for (size_t i = 0; i < outbound[x].size(); i++)
		{
			if (outbound[x][i] == y)
			{
				outbound[x].erase(outbound[x].begin() + i);
				break;
			}
		}

		for (size_t i = 0; i < inbound[y].size(); i++)
		{
			if (inbound[y][i] == x)
			{
				inbound[y].erase(inbound[y].begin() + i);
				break;
			}
		}

		cost[x].erase(y);

	}

	// Modifies cost of edge (x, y)
	void set_cost(int x, int y, int new_cost)
	{
		if (!is_edge(x, y))
			throw runtime_error("Edge does not exist");

		cost[x][y] = new_cost;

	}

	// Adds a new vertex (id = n)
	void add_vertex()
	{
		outbound[n] = {};
		inbound[n] = {};
		n++;
	}

	// Removes vertex x and reindexes vertices
	void remove_vertex(int x)
	{
		if (outbound.find(x) == outbound.end())
			throw runtime_error("Vertex does not exist");

		Graph new_graph(n - 1);

		for (int a : parse_vertices())
		{
			for (int b : parse_out(a))
			{
				if (a == x || b == x)
					continue;

				int new_a;
				if (a > x)
					new_a = a - 1;
				else
					new_a = a;
				int new_b;
				if (b > x)
					new_b = b - 1;
				else
					new_b = b;

				int c = get_cost(a, b);
				new_graph.add_edge(new_a, new_b, c);
			}
		}

		*this = new_graph;
		//outbound = new_graph.outbound;
		//inbound = new_graph.inbound;
		//cost = new_graph.cost;
		//n = new_graph.n;
	}

};

struct Project {
	Graph graph;
	vector<string> name;
	vector<int> duration;
	int realActivities;

	Project(int vertices) : graph(vertices) {
		name.resize(vertices);
		duration.resize(vertices, 0);
		realActivities = vertices - 2;
	}
};

/*
Reads graph from file
Format:
n m
x y c (m lines)
*/
Graph read_graph_from_file(const string& filename)
{
	ifstream fin(filename);

	if (!fin)
		throw runtime_error("Cannot open input file");

	int n, m;
	fin >> n >> m;

	Graph g(n);

	for (int i = 0; i < m; i++)
	{
		int x, y, c;
		fin >> x >> y >> c;
		if (!g.is_edge(x, y))
			g.add_edge(x, y, c);

		if (!g.is_edge(y, x))
			g.add_edge(y, x, c); // for undirected
	}

	return g;

}

Project read_project_from_file(const string& filename)
{
	ifstream fin(filename);

	if (!fin)
		throw runtime_error("Cannot open project input file");

	int n;
	fin >> n;

	// vertices: 0 = X, 1..n = real activities, n+1 = Y
	Project p(n + 2);
	p.name[0] = "X";
	p.name[n + 1] = "Y";
	p.duration[0] = 0;
	p.duration[n + 1] = 0;

	vector<vector<int>> prerequisities(n + 1);

	for (int i = 1; i <= n; i++)
	{
		int id, dur, k;
		string activityName;

		fin >> id >> activityName >> dur >> k;

		p.name[id] = activityName;
		p.duration[id] = dur;

		for (int j = 0; j < k; j++)
		{
			int pred;
			fin >> pred;
			prerequisities[id].push_back(pred);
		}
	}

	// add precedence edge
	for (int activity = 1; activity <= n; activity++)
	{
		for (int pred : prerequisities[activity])
		{
			if (!p.graph.is_edge(pred, activity))
				p.graph.add_edge(pred, activity, 0);
		}
	}

	// add fake start X to activities without prerequisities
	for (int activity = 1; activity <= n; activity++)
	{
		if (p.graph.in_degree(activity) == 0)
			p.graph.add_edge(0, activity, 0);
	}

	// add fake end Y after activities without successor
	for (int activity = 1; activity <= n; activity++)
	{
		if (p.graph.out_degree(activity) == 0)
			p.graph.add_edge(activity, n + 1, 0);
	}

	return p;
}

// Writes graph to file (same format as input)
void write_graph_to_file(Graph& g, const string& filename)
{
	ofstream fout(filename);

	if (!fout)
		throw runtime_error("Cannot open output file");

	fout << g.number_of_vertices() << " " << g.number_of_edges() << "\n";

	for (int x : g.parse_vertices())
	{
		for (int y : g.parse_out(x))
		{
			fout << x << " " << y << " " << g.get_cost(x, y) << "\n";
		}
	}

}

// Generates random graph with n vertices and m edges
Graph random_graph(int n, int m)
{
	Graph g(n);

	if (m > n * n)
		throw runtime_error("Too many edges for the given number of vertices");

	srand(time(0));

	int count = 0;

	while (count < m)
	{
		int x = rand() % n;
		int y = rand() % n;

		if (!g.is_edge(x, y))
		{
			int c = rand() % 100 + 1;
			g.add_edge(x, y, c);
			count++;
		}
	}
	return g;
}

// dfs algorithm, goes through all reachable vertices from start, left, right
void dfs(Graph& g, int start, unordered_map<int, bool>& visited, vector<int>& component)
{
	visited[start] = true;
	component.push_back(start);

	for (int neighbor : g.parse_out(start))
	{
		if (!visited[neighbor])
		{
			dfs(g, neighbor, visited, component);
		}
	}
}

// Returns list of connected components, each component is a list of vertices
vector<vector<int>> connected_components(Graph& g)
{
	unordered_map<int, bool> visited;
	vector<vector<int>> all_components;

	for (int v : g.parse_vertices())
	{
		visited[v] = false;
	}

	for (int v : g.parse_vertices())
	{
		if (!visited[v])
		{
			vector<int> component;
			dfs(g, v, visited, component);
			all_components.push_back(component);
		}
	}

	return all_components;
}

// Builds a graph for a connected component given the original graph and the list of vertices in the component
Graph build_component_graph(Graph& g, vector<int>& component)
{
	Graph comp_graph(g.number_of_vertices());

	unordered_map<int, bool> in_component;
	for (int v : component)
	{
		in_component[v] = true;
	}

	for (int x : component)
	{
		for (int y : g.parse_out(x))
		{
			if (in_component[y])
			{
				if (!comp_graph.is_edge(x, y))
				{
					comp_graph.add_edge(x, y, g.get_cost(x, y));
				}
			}
		}
	}

	return comp_graph;
}

// Returns list of graphs, each graph is a connected component of the original graph
vector<Graph> connected_component_graphs(Graph& g)
{
	vector<vector<int>> comps = connected_components(g);
	vector<Graph> result;

	for (vector<int>& comp : comps)
	{
		result.push_back(build_component_graph(g, comp));
	}

	return result;
}

void ford_lowest_cost_walk(Graph& g, int start, int end)
{
	const int INF = INT_MAX / 2;

	int n = g.number_of_vertices();

	vector<int> dist(n, INF);
	vector<int> prev(n, -1);

	dist[start] = 0;

	// relax all edges n - 1 times
	for (int k = 1; k <= n - 1; k++)
	{
		bool changed = false;

		for (int x : g.parse_vertices())
		{
			if (dist[x] == INF)
				continue;

			for (int y : g.parse_out(x))
			{
				int c = g.get_cost(x, y);

				if (dist[x] + c < dist[y])
				{
					dist[y] = dist[x] + c;
					prev[y] = x;
					changed = true;
				}
			}
		}

		if (!changed)
			break;
	}

	// check for negative cost cycle accessible from start
	for (int x : g.parse_vertices())
	{
		if (dist[x] == INF)
			continue;

		for (int y : g.parse_out(x))
		{
			int c = g.get_cost(x, y);

			if (dist[x] + c < dist[y])
			{
				cout << "There is a negative cost cycle accessible from the starting vertex\n";
				return;
			}
		}
	}

	// no walk from start to end
	if (dist[end] == INF)
	{
		cout << "There is no walk from " << start << " to " << end << ".\n";
		return;
	}

	// reconstruct path
	vector<int> path;
	int current = end;

	while (current != -1)
	{
		path.push_back(current);
		current = prev[current];
	}

	reverse(path.begin(), path.end());

	cout << "Lowest cost walk from " << start << " to " << end << ": ";

	for (size_t i = 0; i < path.size(); i++)
	{
		cout << path[i];

		if (i + 1 < path.size())
			cout << " -> ";
	}

	cout << "\nCost: " << dist[end] << "\n";
}

bool dfs_topological_sort(Graph& g, int vertex, vector<int>& state, vector<int>& topo)
{
	// state:
	// 0 = invisited
	// 1 = currently visiting
	// 2 = fully visited

	state[vertex] = 1;

	for (int neighbour : g.parse_out(vertex))
	{
		if (state[neighbour] == 1)
		{
			// found back edge => cycle
			return false;
		}

		if (state[neighbour] == 0)
		{
			if (!dfs_topological_sort(g, neighbour, state, topo))
				return false;
		}
	}

	state[vertex] = 2;
	topo.push_back(vertex);

	return true;
}

bool topological_sort_tarjan(Graph& g, vector<int>& topo)
{
	int n = g.number_of_vertices();
	vector<int> state(n, 0);

	vector<int> vertices = g.parse_vertices();
	sort(vertices.begin(), vertices.end());

	for (int v : vertices)
	{
		if (state[v] == 0)
		{
			if (!dfs_topological_sort(g, v, state, topo))
				return false;
		}
	}

	reverse(topo.begin(), topo.end());
	return true;
}

void solve_critical_path_problem()
{
	string filename;
	cout << "Project input file: ";
	cin >> filename;

	try
	{
		Project p = read_project_from_file(filename);
		Graph& g = p.graph;

		vector<int> topo;

		bool isDAG = topological_sort_tarjan(g, topo);

		if (!isDAG)
		{
			cout << "The graph is NOT a DAG. The project cannot be scheduled.\n";
			return;
		}

		cout << "\nThe graph is a DAG.\n";

		cout << "\nTopological order:\n";
		for (int v : topo)
		{
			cout << v << "(" << p.name[v] << ") ";
		}
		cout << "\n";

		int vertices = g.number_of_vertices();

		vector<int> earliestStart(vertices, 0);
		vector<int> earliestFinish(vertices, 0);
		vector<int> latestStart(vertices, 0);
		vector<int> latestFinish(vertices, 0);

		// Forward pass: earliest start and earliest finish
		for (int v : topo)
		{
			int maxFinish = 0;

			for (int pred : g.parse_in(v))
			{
				maxFinish = max(maxFinish, earliestFinish[pred]);
			}

			earliestStart[v] = maxFinish;
			earliestFinish[v] = earliestStart[v] + p.duration[v];
		}

		int Y = p.realActivities + 1;
		int projectDuration = earliestFinish[Y];

		// Backward pass: latest start and latest finish
		for (int i = 0; i < vertices; i++)
		{
			latestFinish[i] = projectDuration;
			latestStart[i] = projectDuration - p.duration[i];
		}

		latestFinish[Y] = projectDuration;
		latestStart[Y] = projectDuration;

		for (int i = topo.size() - 2; i >= 0; i--)
		{
			int v = topo[i];

			int minStart = INT_MAX;

			for (int succ : g.parse_out(v))
			{
				minStart = min(minStart, latestStart[succ]);
			}

			latestFinish[v] = minStart;
			latestStart[v] = latestFinish[v] - p.duration[v];
		}

		cout << "\nBuilt graph:\n";
		for (int v : topo)
		{
			cout << v << "(" << p.name[v] << ") -> ";

			for (int succ : g.parse_out(v))
			{
				cout << succ << "(" << p.name[succ] << ") ";
			}

			cout << "\n";
		}

		cout << "\nActivity times:\n";
		cout << "ID\tName\t\t\tDuration\tES\tEF\tLS\tLF\tCritical\n";

		for (int v : topo)
		{
			cout << v << "\t";
			cout << p.name[v] << "\t";

			if (p.name[v].size() < 16)
				cout << "\t";

			cout << p.duration[v] << "\t\t";
			cout << earliestStart[v] << "\t";
			cout << earliestFinish[v] << "\t";
			cout << latestStart[v] << "\t";
			cout << latestFinish[v] << "\t";

			if (earliestStart[v] == latestStart[v])
				cout << "YES";
			else
				cout << "NO";

			cout << "\n";
		}

		cout << "\nMinimum project duration: " << projectDuration << "\n";

		cout << "\nCritical activities:\n";
		for (int v : topo)
		{
			if (v != 0 && v != Y && earliestStart[v] == latestStart[v])
			{
				cout << v << "(" << p.name[v] << ") starts at time "
					<< earliestStart[v] << "\n";
			}
		}

		cout << "\nCritical path:\n";

		int current = 0;
		cout << current << "(" << p.name[current] << ")";

		while (current != Y)
		{
			bool foundNext = false;

			for (int succ : g.parse_out(current))
			{
				if (earliestStart[succ] == latestStart[succ] &&
					earliestStart[succ] == earliestFinish[current])
				{
					cout << " -> " << succ << "(" << p.name[succ] << ")";
					current = succ;
					foundNext = true;
					break;
				}
			}

			if (!foundNext)
				break;
		}

		cout << "\n\n";
	}
	catch (const exception& e)
	{
		cout << e.what() << endl;
	}
}

// check if i can add the vertex to the path
bool is_safe_hamiltonian(Graph& g, int vertex, vector<int>& path, vector<bool>& visited)
{
	int lastVertex = path.back();

	// 1. The new vertex must be connected to the last vertex in the path
	if (!g.is_edge(lastVertex, vertex))
		return false;

	// 2. We can't visit the same vertex twice
	if (visited[vertex])
		return false;

	return true;
}

bool hamiltonian_backtracking(Graph& g, vector<int>& path, vector<bool>& visited)
{
	int n = g.number_of_vertices();

	// if path contains all vertices, check if we can return to the start
	if (path.size() == n)
	{
		int lastVertex = path.back();
		int startVertex = path[0];

		return g.is_edge(lastVertex, startVertex);
	}

	// try every vertex as the next vertex in the path
	for (int vertex = 0; vertex < n; vertex++)
	{
		if (is_safe_hamiltonian(g, vertex, path, visited))
		{
			path.push_back(vertex);
			visited[vertex] = true;

			if (hamiltonian_backtracking(g, path, visited))
				return true;

			// undo choice
			visited[vertex] = false;
			path.pop_back();
		}
	}

	return false;
}

void solve_hamiltonian_cycle(Graph& g)
{
	int n = g.number_of_vertices();

	if (n == 0)
	{
		cout << "The graph has no vertices.\n";
		return;
	}

	vector<int> path;
	vector<bool> visited(n, false);

	// start from vertex 0
	path.push_back(0);
	visited[0] = true;

	bool found = hamiltonian_backtracking(g, path, visited);

	if (!found)
	{
		cout << "No Hamiltonian cycle exists.\n";
		return;
	}

	cout << "Hamiltonain cycle found:\n";

	for (int v : path)
		cout << v << " -> ";

	cout << path[0] << "\n";
}

// Prints menu options
void print_menu()
{
	cout << "1. Read graph from file\n";
	cout << "2. Write graph to file\n";
	cout << "3. Generate random graph\n";
	cout << "4. Number of vertices\n";
	cout << "5. Number of edges\n";
	cout << "6. Check edge\n";
	cout << "7. Get edge cost\n";
	cout << "8. Print graph\n";
	cout << "9. Add edge\n";
	cout << "10. Remove edge\n";
	cout << "11. Add vertex\n";
	cout << "12. Remove vertex\n";
	cout << "13. In-degree of a vertex\n";
	cout << "14. Out-degree of a vertex\n";
	cout << "15. Modify cost of an edge\n";
	cout << "16. Parse outbound neighbours\n";
	cout << "17. Parse inbound neighbours\n";
	cout << "18. Parse vertices\n";
	cout << "19. Find the connected components\n";
	cout << "20. Lowest cost walk using Ford's algorithm\n";
	cout << "21. Critical path project scheduling problem\n";
	cout << "22. Hamiltonian cycle\n";
	cout << "0. Exit\n";
	cout << endl;
}

int main()
{
	//Graph g = read_graph_from_file("C:\\Users\\w11\\idk\\graph1k.txt");

	//Graph g = random_graph(5, 8);

	//cout << "Vertices: " << g.number_of_vertices() << endl;
	//cout << "Edges: " << g.number_of_edges() << endl;

	//for (int x : g.parse_vertices())
	//{
	//	cout << x << ": ";;
	//	for (int y : g.parse_out(x))
	//	{
	//		cout << y << "(" << g.get_cost(x, y) << ") ";
	//	}
	//	cout << endl;
	//}


	// Initializes random seed (needed for random graph)
	srand((unsigned)time(0));

	Graph g(0);
	int option;

	while (true)
	{
		print_menu();
		cin >> option;

		if (option == 0)
			break;

		if (option == 1)
		{
			string filename;
			cout << "File name: ";
			cin >> filename;

			try
			{
				g = read_graph_from_file(filename);
				cout << "Graph loaded\n";

			}
			catch (const exception& e)
			{
				cout << e.what() << endl;
			}
			cout << endl;
		}

		if (option == 2)
		{
			string filename;
			cout << "File name: ";
			cin >> filename;

			try
			{
				write_graph_to_file(g, filename);
				cout << "Graph written to file\n";
			}
			catch (const exception& e)
			{
				cout << e.what() << endl;
			}
			cout << endl;
		}

		if (option == 3)
		{
			int n, m;
			cout << "Number of vertices: ";
			cin >> n;
			cout << "Number of edges: ";
			cin >> m;

			try
			{
				g = random_graph(n, m);
				cout << "Random graph generated\n";
			}
			catch (const exception& e)
			{
				cout << e.what() << endl;
			}
			cout << endl;
		}

		if (option == 4)
		{
			cout << "Vertices: " << g.number_of_vertices() << endl;
			cout << endl;
		}

		if (option == 5)
		{
			cout << "Edges: " << g.number_of_edges() << endl;
			cout << endl;
		}

		if (option == 6)
		{
			int x, y;
			cout << "x = ";
			cin >> x;
			cout << "y = ";
			cin >> y;

			if (g.is_edge(x, y))
				cout << "Edge exists\n";
			else
				cout << "Edge does not exist\n";
			cout << endl;
		}

		if (option == 7)
		{
			int x, y;
			cout << "x = ";
			cin >> x;
			cout << "y = ";
			cin >> y;
			try
			{
				cout << "Cost: " << g.get_cost(x, y) << endl;
			}
			catch (const exception& e)
			{
				cout << e.what() << endl;
			}
		}

		if (option == 8)
		{
			for (int x : g.parse_vertices())
			{
				if (g.out_degree(x) == 0 && g.in_degree(x) == 0)
					continue;

				cout << x << ": ";
				for (int y : g.parse_out(x))
				{
					cout << y << "(" << g.get_cost(x, y) << ") ";
				}
				cout << endl;
			}
		}

		if (option == 9)
		{
			int x, y, c;
			cout << "x = ";
			cin >> x;
			cout << "y = ";
			cin >> y;
			cout << "cost = ";
			cin >> c;

			try
			{
				g.add_edge(x, y, c);
				cout << "Edge added\n";
			}
			catch (const exception& e)
			{
				cout << e.what() << endl;
			}
			cout << endl;
		}

		if (option == 10)
		{
			int x, y;
			cout << "x = ";
			cin >> x;
			cout << "y = ";
			cin >> y;

			try
			{
				g.remove_edge(x, y);
				cout << "Edge removed\n";
			}
			catch (const exception& e)
			{
				cout << e.what() << endl;
			}
			cout << endl;
		}

		if (option == 11)
		{
			g.add_vertex();
			cout << "Vertex added\n";
			cout << endl;
		}

		if (option == 12)
		{
			int x;
			cout << "x = ";
			cin >> x;

			try
			{
				g.remove_vertex(x);
				cout << "Vertex removed\n";
			}
			catch (const exception& e)
			{
				cout << e.what() << endl;
			}
			cout << endl;
		}

		if (option == 13)
		{
			int x;
			cout << "x = ";
			cin >> x;

			try
			{
				cout << "In-degree: " << g.in_degree(x) << endl;
			}
			catch (const exception& e)
			{
				cout << e.what() << endl;
			}
			cout << endl;
		}

		if (option == 14)
		{
			int x;
			cout << "x = ";
			cin >> x;

			try
			{
				cout << "Out-degree: " << g.out_degree(x) << endl;

			}
			catch (const exception& e)
			{
				cout << e.what() << endl;
			}
			cout << endl;
		}

		if (option == 15)
		{
			int x, y, c;
			cout << "x = ";
			cin >> x;
			cout << "y = ";
			cin >> y;
			cout << "new cost = ";
			cin >> c;

			try
			{
				g.set_cost(x, y, c);
				cout << "Cost updated\n";
			}
			catch (const exception& e)
			{
				cout << e.what() << endl;
			}
			cout << endl;
		}

		if (option == 16)
		{
			int x;
			cout << "x = ";
			cin >> x;

			try
			{
				for (int y : g.parse_out(x))
				{
					cout << y << " ";
				}
				cout << endl;
			}
			catch (const exception& e)
			{
				cout << e.what() << endl;
			}
			cout << endl;
		}

		if (option == 17)
		{
			int x;
			cout << "x = ";
			cin >> x;

			try
			{
				for (int y : g.parse_in(x))
				{
					cout << y << " ";
				}
				cout << endl;
			}
			catch (const exception& e)
			{
				cout << e.what() << endl;
			}
			cout << endl;
		}

		if (option == 18)
		{
			vector<int> vertices = g.parse_vertices();
			cout << "Vertices: ";
			for (int v : vertices)
			{
				cout << v << " ";
			}
			cout << endl << endl;
		}

		if (option == 19)
		{
			vector<vector<int>> comps = connected_components(g);

			cout << "Number of connected components: " << comps.size() << endl;

			for (size_t i = 0; i < comps.size(); i++)
			{
				cout << "Component " << i + 1 << ":\n";

				for (int x : comps[i])
				{
					cout << x << ": ";
					for (int y : g.parse_out(x))
					{
						bool ok = false;
						for (int v : comps[i])
						{
							if (v == y)
							{
								ok = true;
								break;
							}
						}

						if (ok)
						{
							cout << y << "(" << g.get_cost(x, y) << ") ";
						}
					}
					cout << endl;
				}

				cout << endl;
			}
		}

		if (option == 20)
		{
			int start, end;
			cout << "Start vertex: ";
			cin >> start;
			cout << "End vertex: ";
			cin >> end;
			try
			{
				ford_lowest_cost_walk(g, start, end);
			}
			catch (const exception& e)
			{
				cout << e.what() << endl;
			}
			cout << endl;
		}
		if (option == 21)
		{
			solve_critical_path_problem();
		}
		if (option == 22)
		{
			try
			{
				solve_hamiltonian_cycle(g);
			}
			catch (const exception& e)
			{
				cout << e.what() << endl;
			}

			cout << endl;
		}

	}

	return 0;
}
