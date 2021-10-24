#include <iostream>
#include <vector>
enum { WHITE = 0, GREEN = 1, RED };
enum weight{UNWEIGHTED = 1};
class Graph {
public:
    typedef size_t Vertex;
    virtual void Add_Edge(Vertex a, Vertex b) = 0;
    virtual std::vector<Vertex> Get_Neighbors(Vertex v) const = 0;
    Vertex Get_Num_Of_Verteses() const {
        return num_of_verteses;
    }
    mutable bool IsBipart = true;
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
protected:
    bool IsDirected = false;
    int32_t num_of_edges = 0;
    Vertex num_of_verteses = 0;
};
class AdjList : public Graph {
    std::vector<std::vector<Vertex>> list_of_adjacency;
    std::vector<Vertex> Get_Neighbors(Vertex v) const override {
        return list_of_adjacency[v];
    }
public:
    AdjList(Vertex verteses, int32_t number_of_edges) {
        num_of_verteses = verteses;
        num_of_edges = number_of_edges;
        list_of_adjacency.resize(num_of_verteses + 1);
    }
    void Add_Edge(Vertex a, Vertex b) override {
        list_of_adjacency[a].push_back(b);
        if (!IsDirected) {
            list_of_adjacency[b].push_back(a);
        }
    }
    ~AdjList() = default;

};

class AdjMatrix : public Graph {
    std::vector<std::vector<Vertex>> matrix_of_adjacency;
    std::vector<Vertex> Get_Neighbors(Vertex v) const override {
        std::vector<Vertex> neighbors;
        for (int32_t i = 1; i <= num_of_verteses; ++i) {
            if (matrix_of_adjacency[v - 1][i - 1]) {
                neighbors.push_back(i);
            }
        }
        return neighbors;
    }
public:
    AdjMatrix(Vertex verteses, int32_t number_of_edges) {
        num_of_verteses = verteses;
        num_of_edges = number_of_edges;
        IsBipart = true;
        matrix_of_adjacency.resize(num_of_verteses);
        for (auto& line: matrix_of_adjacency) {
            line.resize(num_of_verteses, 0);
        }
    }
    void Add_Edge(Vertex a, Vertex b) override {
        matrix_of_adjacency[a - 1][b - 1] = UNWEIGHTED;
        if (!IsDirected) {
            matrix_of_adjacency[b - 1][a - 1] = UNWEIGHTED;
        }
    }
    ~AdjMatrix() = default;
};

bool Checking_for_bipartition(const Graph& graph) {
    std::vector<Graph::Vertex> colors(graph.Get_Num_Of_Verteses() + 1, WHITE);
    for (Graph::Vertex vertex = 1; vertex < graph.Get_Num_Of_Verteses(); ++vertex) {
        if (colors[vertex] == WHITE) {
            graph.DFS_Visit(graph, colors, vertex, GREEN);
            if (!graph.IsBipart) {
                return false;
            }
        }
    }
    return true;
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
    if (Checking_for_bipartition(graph)) {
        std::cout << "YES";
    } else {
        std::cout << "NO";
    }
    return 0;
}

