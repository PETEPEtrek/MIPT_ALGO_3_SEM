#include <iostream>
#include <vector>
#include <queue>
enum { INF = 2147483647 };

class Graph {
public:
    typedef size_t Vertex;
    bool IsDirected = true;
    Vertex num_of_edges = 0;
    Vertex num_of_verteses = 0;
    virtual void Add_Edge(Vertex first_vertex, Vertex second_vertex, int32_t value) = 0;
    virtual std::vector<std::pair<Vertex, int32_t>> Get_Neighbors(Vertex v) const = 0;
};
class AdjList : public Graph {
    typedef std::pair<Vertex, int32_t> Pair;
    std::vector<std::vector<Pair>> list_of_adjacency;
    std::vector<Pair> Get_Neighbors(Vertex v) const override {
        return list_of_adjacency[v];
    }
public:
    AdjList(Vertex verteses, Vertex edges) {
        num_of_verteses = verteses;
        num_of_edges = edges;
        list_of_adjacency.resize(num_of_verteses + 1);
    }
    void Add_Edge(Vertex first_vertex, Vertex second_vertex, int32_t value) override {
        list_of_adjacency[first_vertex].emplace_back(std::make_pair(second_vertex, value));
        if (!IsDirected) {
            list_of_adjacency[second_vertex].emplace_back(std::make_pair(first_vertex, value));
        }
    }
    ~AdjList() = default;

};

class AdjMatrix : public Graph {
    typedef std::pair<Vertex, int32_t> Pair;
    std::vector<std::vector<Vertex>> matrix_of_adjacency;
    std::vector<Pair> Get_Neighbors(Vertex v) const override {
        std::vector<Pair> neighbors;
        for (int32_t i = 1; i <= num_of_verteses; ++i) {
            if (matrix_of_adjacency[v - 1][i - 1]) {
                neighbors.emplace_back(i, matrix_of_adjacency[v - 1][i - 1]);
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
    void Add_Edge(Vertex first_vertex, Vertex second_vertex, int32_t value) override {
        matrix_of_adjacency[first_vertex - 1][second_vertex - 1] = value;
        if (!IsDirected) {
            matrix_of_adjacency[second_vertex - 1][first_vertex - 1] = value;
        }
    }
    ~AdjMatrix() = default;
};
std::vector<int32_t> Best_path(Graph& graph, Graph::Vertex from, Graph::Vertex to) {
    std::vector<int32_t> distance(graph.num_of_verteses + 1, INF);
    std::deque<Graph::Vertex> deque;
    deque.push_back(from);
    distance[from] = 0;
    while (!deque.empty()) {
        Graph::Vertex v = deque.front();
        deque.pop_front();
        for (auto u: graph.Get_Neighbors(v)) {
            if (distance[u.first] > distance[v] + u.second) {
                distance[u.first] = distance[v] + u.second;
                if (u.second == 0) {
                    deque.push_front(u.first);
                } else {
                    deque.push_back(u.first);
                }
            }
        }
    }
    return distance;
}
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int32_t num_of_verteses, num_of_edges;
    std::cin >> num_of_verteses >> num_of_edges;
    AdjList graph(num_of_verteses, num_of_edges);
    for (int32_t i = 0; i < num_of_edges; ++i) {
        Graph::Vertex first_vertex, second_vertex;
        std::cin >> first_vertex >> second_vertex;
        graph.Add_Edge(first_vertex, second_vertex, 0);
        graph.Add_Edge(second_vertex, first_vertex, 1);
    }
    int32_t count;
    std::cin >> count;
    std::vector<std::vector<int32_t>> distance(num_of_verteses + 1);
    for (int32_t i = 1; i <= num_of_verteses; ++i) {
        distance[i] = Best_path(graph, i, num_of_verteses);
    }
    for (int32_t i = 0; i < count; ++i) {
        Graph::Vertex from, to;
        std::cin >> from >> to;
        if (distance[from][to] == INF) {
            std::cout << "-1" << std::endl;
        } else {
            std::cout << distance[from][to] << std::endl;
        }
    }
    return 0;
}
