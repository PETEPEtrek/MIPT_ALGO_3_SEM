#include <iostream>
#include <vector>
enum { WHITE = 0, GRAY = 1, BLACK = 2 };
enum WEIGHT {UNWEIGHTED = 1};
class Graph {
public:
    typedef size_t Vertex;
    Vertex Get_Num_Of_Verteses() const {
        return num_of_verteses;
    }
    virtual void Add_Edge(Vertex first_vertex, Vertex second_vertex) = 0;
    virtual std::vector<Vertex> Get_Neighbors(Vertex& vertex) const = 0;
    bool DFS_Visit(const Graph& graph,
                   std::vector<Vertex>& colors,
                   Vertex v,
                   std::vector<Graph::Vertex>& cycle,
                   Vertex& cycle_start,
                   Vertex& cycle_end) const {
        colors[v] = GRAY;
        for (auto u: graph.Get_Neighbors(v)) {
            if (colors[u] == GRAY) {
                cycle_end = v;
                cycle_start = u;
                return true;
            } else if (colors[u] == WHITE) {
                cycle[u] = v;
                if (DFS_Visit(graph, colors, u, cycle, cycle_start, cycle_end)) {
                    return true;
                }
            }
        }
        colors[v] = BLACK;
        return false;
    }
protected:
    bool IsDirected = true;
    int32_t num_of_edges = 0;
    Vertex num_of_verteses = 0;
};
class AdjList : public Graph {
    std::vector<std::vector<Vertex>> list_of_adjacency;

    std::vector<Vertex> Get_Neighbors(Vertex& v) const override {
        return list_of_adjacency[v];
    }
public:
    AdjList(Vertex verteses, int32_t number_of_edges) {
        num_of_verteses = verteses;
        num_of_edges = number_of_edges;
        list_of_adjacency.resize(1 + num_of_verteses);
    }
    void Add_Edge(Vertex first_vertex, Vertex second_vertex) override {
        list_of_adjacency[first_vertex].push_back(second_vertex);
        if (!IsDirected) {
            list_of_adjacency[second_vertex].push_back(first_vertex);
        }
    }
    ~AdjList() = default;

};

class AdjMatrix : public Graph {
    std::vector<std::vector<Vertex>> matrix_of_adjacency;
    std::vector<Vertex> Get_Neighbors(Vertex& v) const override {
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
        matrix_of_adjacency.resize(num_of_verteses);
        for (auto& line: matrix_of_adjacency) {
            line.resize(num_of_verteses, 0);
        }
    }
    void Add_Edge(Vertex first_vertex, Vertex second_vertex) override {
        matrix_of_adjacency[first_vertex - 1][second_vertex - 1] = UNWEIGHTED;
        if (!IsDirected) {
            matrix_of_adjacency[second_vertex - 1][first_vertex - 1] = UNWEIGHTED;
        }
    }
    ~AdjMatrix() = default;
};

std::vector<Graph::Vertex> HasCycle(const Graph& graph) {
    Graph::Vertex cycle_end = 0;
    Graph::Vertex cycle_start = 0;
    std::vector<Graph::Vertex> cycle(graph.Get_Num_Of_Verteses() + 1, 0);
    std::vector<Graph::Vertex> colors(graph.Get_Num_Of_Verteses() + 1, WHITE);
    for (Graph::Vertex vertex = 1; vertex <= graph.Get_Num_Of_Verteses(); ++vertex) {
        if (graph.DFS_Visit(graph, colors, vertex, cycle, cycle_start, cycle_end)) {
            break;
        }
    }
    if (cycle_start == 0) {
        return {};
    }
    std::vector<Graph::Vertex> answer;
    while (cycle_end != cycle_start) {
        answer.push_back(cycle_end);
        cycle_end = cycle[cycle_end];
    }
    answer.push_back(cycle_start);
    return answer;
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
    std::vector<Graph::Vertex> answer = HasCycle(graph);
    if (answer.empty()) {
        std::cout << "NO";
    } else {
        std::cout << "YES" << std::endl;
        for (int32_t item = answer.size() - 1; item >= 0; --item) {
            std::cout << answer[item] << " ";
        }
    }
    return 0;
}
