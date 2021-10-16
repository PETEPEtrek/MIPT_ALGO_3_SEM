#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
#include <queue>
enum { INF = 2147483647 };
class Graph {
public:
    typedef size_t Vertex;
    bool IsDirected = false;
    Vertex num_of_edges = 0;
    Vertex num_of_verteses = 0;
    virtual void Add_Edge(Vertex a, Vertex b) = 0;
    virtual std::set<Vertex> Get_Neighbors(Vertex v) const = 0;
};
class AdjList : public Graph {
    std::unordered_map<Vertex, std::set<Vertex>> list_of_adjacency;

    std::set<Vertex> Get_Neighbors(Vertex v) const override {
        if (!list_of_adjacency.count(v)) {
            std::set<Vertex> empty_set = {};
            return empty_set;
        }
        return list_of_adjacency.at(v);
    }
public:
    AdjList(Vertex verteses, Vertex edges) {
        num_of_verteses = verteses;
        num_of_edges = edges;
    }
    void Add_Edge(Vertex a, Vertex b) override {
        list_of_adjacency[a].emplace(b);
        if (!IsDirected) {
            list_of_adjacency[b].emplace(a);
        }
    }
    AdjList(Vertex verteses, Vertex edges, std::vector<std::pair<Vertex, Vertex>>& graph) {
        num_of_verteses = verteses;
        num_of_edges = edges;
        for (auto& item: graph) {
            list_of_adjacency[item.first].emplace(item.second);
            if (!IsDirected) {
                list_of_adjacency[item.second].emplace(item.first);
            }
        }
    }
    ~AdjList() = default;

};

class AdjMatrix : public Graph {
    std::vector<std::vector<Vertex>> matrix_of_adjacency;
    std::set<Vertex> Get_Neighbors(Vertex v) const override {
        std::set<Vertex> neighbors;
        for (int32_t i = 1; i <= num_of_verteses; ++i) {
            if (matrix_of_adjacency[v - 1][i - 1]) {
                neighbors.emplace(i);
            }
        }
        return neighbors;
    }
public:
    AdjMatrix(Vertex verteses, Vertex edges) {
        num_of_verteses = verteses;
        num_of_edges = edges;
        matrix_of_adjacency.resize(num_of_verteses);
        for (auto& line: matrix_of_adjacency) {
            line.resize(num_of_verteses, 0);
        }
    }
    void Add_Edge(Vertex a, Vertex b) override {
        matrix_of_adjacency[a - 1][b - 1] = 1;
        if (!IsDirected) {
            matrix_of_adjacency[b - 1][a - 1] = 1;
        }
    }
    AdjMatrix(Vertex verteses, Vertex edges, std::vector<std::pair<Vertex, Vertex>>& graph) {
        num_of_verteses = verteses;
        num_of_edges = edges;
        matrix_of_adjacency.resize(num_of_verteses);
        for (auto& line: matrix_of_adjacency) {
            line.resize(num_of_verteses, 0);
        }
        for (auto& item: graph) {
            matrix_of_adjacency[item.first - 1][item.second - 1] = 1;
            if (!IsDirected) {
                matrix_of_adjacency[item.second - 1][item.first - 1] = 1;
            }
        }
    }
    ~AdjMatrix() = default;
};

void Shortest_Path_Between_Verteses(const Graph& graph, Graph::Vertex from, Graph::Vertex to) {
    std::vector<Graph::Vertex> distance(graph.num_of_verteses + 1, INF);
    std::vector<Graph::Vertex> parent(graph.num_of_verteses + 1, 0);
    std::queue<Graph::Vertex> queue;
    std::vector<bool> used(graph.num_of_verteses + 1, false);
    queue.push(from);
    used[from] = true;
    distance[from] = 0;
    parent[from] = 0;
    while (!queue.empty()) {
        Graph::Vertex v = queue.front();
        queue.pop();
        for (auto u: graph.Get_Neighbors(v)) {
            if (!used[u]) {
                used[u] = true;
                if (distance[u] == INF) {
                    distance[u] = distance[v] + 1;
                }
                parent[u] = v;
                queue.push(u);
            }
        }
    }
    if (distance[to] == INF) {
        std::cout << -1;
    } else {
        std::cout << distance[to] << std::endl;
        std::vector<Graph::Vertex> answer(1, to);
        Graph::Vertex vertex = parent[to];
        while (vertex != 0) {
            answer.push_back(vertex);
            vertex = parent[vertex];
        }
        for (size_t i = answer.size() - 1; i > 0; --i) {
            std::cout << answer[i] << " ";
        }
        std::cout << answer[0];
    }
}

int main() {
    int32_t num_of_verteses, num_of_edges;
    std::cin >> num_of_verteses >> num_of_edges;
    AdjList graph(num_of_verteses, num_of_edges);
    Graph::Vertex from, to;
    std::cin >> from >> to;
    for (int32_t i = 0; i < num_of_edges; ++i) {
        Graph::Vertex first_vertex, second_vertex;
        std::cin >> first_vertex >> second_vertex;
        graph.Add_Edge(first_vertex, second_vertex);
    }
    Shortest_Path_Between_Verteses(graph, from, to);
    return 0;
}
