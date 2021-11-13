#include <iostream>
#include <vector>

enum COLORS { WHITE = 0, GRAY = 1, BLACK = 2 };

class Graph {
public:
    typedef int64_t Vertex;
    Vertex Get_Num_Of_Verteses() const {
        return num_of_verteses;
    }
    virtual void Add_Edge(Graph::Vertex first_vertex, Graph::Vertex second_vertex) = 0;
    virtual std::vector<Graph::Vertex> Get_Neighbors(Graph::Vertex v) const = 0;
protected:
    int32_t num_of_edges = 0;
    Vertex num_of_verteses = 1;
};
class AdjList : public Graph {
    std::vector<std::vector<Graph::Vertex>> list_of_adjacency;
    std::vector<Graph::Vertex> Get_Neighbors(Graph::Vertex v) const override {
        return list_of_adjacency[v];
    }
public:
    explicit AdjList(int32_t& verteses) {
        num_of_verteses = verteses;
        num_of_edges = 0;
        list_of_adjacency.resize(num_of_verteses + 1);
    }
    void Add_Edge(Graph::Vertex first_vertex, Graph::Vertex second_vertex) override {
        ++num_of_edges;
        list_of_adjacency[first_vertex].push_back(second_vertex);
    }
    ~AdjList() = default;
};
bool DFS_Visit(const Graph& graph,
               std::vector<Graph::Vertex>& colors,
               Graph::Vertex& v,
               Graph::Vertex& cycle_start) {
    colors[v] = GRAY;
    for (auto u: graph.Get_Neighbors(v)) {
        if (colors[u] == GRAY) {
            cycle_start = u;
            return true;
        } else if (colors[u] == WHITE) {
            if (DFS_Visit(graph, colors, u, cycle_start)) {
                return true;
            }
        }
    }
    colors[v] = BLACK;
    return false;
}
bool HasCycle(const Graph& graph) {
    Graph::Vertex cycle_start = 0;
    std::vector<Graph::Vertex> cycle(graph.Get_Num_Of_Verteses() + 1, 0);
    std::vector<Graph::Vertex> colors(graph.Get_Num_Of_Verteses() + 1, WHITE);
    for (Graph::Vertex vertex = 1; vertex <= graph.Get_Num_Of_Verteses(); ++vertex) {
        if (DFS_Visit(graph, colors, vertex, cycle_start)) {
            break;
        }
    }
    if (cycle_start == 0) {
        return false;
    }
    return true;
}
bool IsManyColors(Graph& graph) {
    return !HasCycle(graph);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int32_t num_of_verteses;
    std::cin >> num_of_verteses;
    AdjList graph(num_of_verteses);
    for (int32_t i = 1; i <= num_of_verteses; ++i) {
        int32_t count = 1;
        for (int32_t j = num_of_verteses - i; j > 0; --j) {
            char color;
            std::cin >> color;
            if (color == 'R') {
                graph.Add_Edge(i, i + count++);
            } else if (color == 'B') {
                graph.Add_Edge(i + count++, i);
            }
        }
    }
    if (IsManyColors(graph)) {
        std::cout << "YES";
    } else {
        std::cout << "NO";
    }
    return 0;
}
