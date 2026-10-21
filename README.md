# Graph Algorithms

A C++ implementation of a **directed weighted graph** ADT, together with a set of classic graph algorithms, all accessible through a console menu.

## Features
**Graph operations**
- Read and write graphs from/to text files, or generate a random graph with *n* vertices and *m* edges
- Add/remove vertices and edges, check edges, get and modify edge costs
- In-degree, out-degree, and iteration over outbound/inbound neighbours

**Algorithms**
- **Connected components** using depth-first search
- **Lowest-cost walk** between two vertices with Ford's (Bellman-Ford) algorithm, including **negative-cycle detection**
- **Topological sorting** using DFS, with DAG check
- **Critical path scheduling** for a project of activities with durations and prerequisites: earliest/latest start and finish times and the critical activities
- **Hamiltonian cycle** search by backtracking

## Graph representation
Each vertex keeps lists of its outbound and inbound neighbours, and edge costs are stored in a map, so neighbour iteration in both directions is efficient.

```
outbound[x] = vertices y such that edge (x, y) exists
inbound[y]  = vertices x such that edge (x, y) exists
cost[x][y]  = cost of edge (x, y)
```

## Input format
```
n m        ← number of vertices and edges
x y c      ← one line per edge: from, to, cost
```
Sample graphs are in the `samples/` folder (`example.txt`, `hamilton.txt`, `graph1k.txt`), and `project.txt` is a sample project for the critical path option.

## How to build and run
Requires **Visual Studio 2026** with *Desktop development with C++*.

1. Open `Graph_a5.slnx`.
2. Run with **Ctrl+F5** and pick an option from the menu.
3. When asked for a file, use a path relative to the project folder, for example `../samples/example.txt`.
