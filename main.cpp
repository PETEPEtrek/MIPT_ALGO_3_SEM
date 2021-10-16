#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
enum { WHITE = 0, GREEN = 1, RED };
class Graph {
public:
    typedef size_t Vertex;
    bool IsDirected = false;
    mutable bool IsBipart = true;
    Vertex num_of_edges = 0;
    Vertex num_of_verteses = 0;
    virtual void Add_Edge(Vertex a, Vertex b) = 0;
    virtual std::set<Vertex> Get_Neighbors(Vertex v) const = 0;
    void DFS_Visit(const Graph& graph, std::vector<Vertex>& colors, Vertex v, int32_t color) const {
        colors[v] = color;
        for (auto u: graph.Get_Neighbors(v)) {
            if (colors[u] == WHITE) {
                DFS_Visit(graph, colors, u, color == GREEN ? RED : GREEN);
            } else {
                if (colors[v] == colors[u]) {
                    IsBipart = false;
                }
            }
        }
    }
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
        IsBipart = true;
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
        IsBipart = true;
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

void Checking_for_bipartition(const Graph& graph) {
    std::vector<Graph::Vertex> colors(graph.num_of_verteses + 1, WHITE);
    for (Graph::Vertex vertex = 1; vertex < graph.num_of_verteses; ++vertex) {
        if (colors[vertex] == WHITE) {
            graph.DFS_Visit(graph, colors, vertex, GREEN);
            if (!graph.IsBipart) {
                std::cout << "NO";
                return;
            }
        }
    }
    std::cout << "YES";
}

int main() {
    int32_t num_of_verteses, num_of_edges;
    std::cin >> num_of_verteses >> num_of_edges;
    AdjList graph(num_of_verteses, num_of_edges);
    for (int32_t i = 0; i < num_of_edges; ++i) {
        Graph::Vertex first_vertex, second_vertex;
        std::cin >> first_vertex >> second_vertex;
        graph.Add_Edge(first_vertex, second_vertex);
    }
    Checking_for_bipartition(graph);
    return 0;
}

