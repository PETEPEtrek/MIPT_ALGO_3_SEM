#include <iostream>
#include <queue>
#include <set>
#include <vector>
enum { INF = 2147483647 };
enum weight { UNWEIGHTED_EDGE = 1 };
class Graph {
public:
    typedef size_t Vertex;
    virtual void Add_Edge(Vertex first_vertex, Vertex second_vertex) = 0;
    virtual std::vector<Vertex> Get_Neighbors(Vertex v) const = 0;
    Vertex Get_Num_Of_Verteses() const {
        return num_of_verteses;
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
    std::vector<Vertex> Get_Neighbors(Vertex v) const override {
        std::vector<Vertex> neighbors;
        for (Vertex i = 1; i <= num_of_verteses; ++i) {
            if (matrix_of_adjacency[v - 1][i - 1]) {
                neighbors.push_back(i);
            }
        }
        return neighbors;
    }

public:
    AdjMatrix(Vertex verteses, int32_t edges) {
        num_of_verteses = verteses;
        num_of_edges = edges;
        matrix_of_adjacency.resize(num_of_verteses);
        for (auto& line: matrix_of_adjacency) {
            line.resize(num_of_verteses, 0);
        }
    }
    void Add_Edge(Vertex first_vertex, Vertex second_vertex) override {
        matrix_of_adjacency[first_vertex - 1][second_vertex - 1] = UNWEIGHTED_EDGE;
        if (!IsDirected) {
            matrix_of_adjacency[second_vertex - 1][first_vertex - 1] = UNWEIGHTED_EDGE;
        }
    }
    ~AdjMatrix() = default;
};
std::vector<Graph::Vertex> Answer_And_Making_Path(std::vector<Graph::Vertex>& parent,
                                                  std::vector<Graph::Vertex>& distance,
                                                  Graph::Vertex to) {
    std::cout << distance[to] << std::endl;
    std::vector<Graph::Vertex> answer(1, to);
    Graph::Vertex vertex = parent[to];
    while (vertex != 0) {
        answer.push_back(vertex);
        vertex = parent[vertex];
    }
    return answer;
}
std::vector<Graph::Vertex> Shortest_Path_Between_Verteses(const Graph& graph, Graph::Vertex from,
                                                          Graph::Vertex to) {
    std::vector<Graph::Vertex> distance(graph.Get_Num_Of_Verteses() + 1, INF);
    std::vector<Graph::Vertex> parent(graph.Get_Num_Of_Verteses() + 1, 0);
    std::queue<Graph::Vertex> queue;
    std::vector<bool> used(graph.Get_Num_Of_Verteses() + 1, false);
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
        return {};
    } else {
        std::vector<Graph::Vertex> answer = Answer_And_Making_Path(parent, distance, to);
        return answer;
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
    std::vector<Graph::Vertex> answer = Shortest_Path_Between_Verteses(graph, from, to);
    if (answer.empty()) {
        std::cout << -1;
    } else {
        for (size_t i = answer.size() - 1; i > 0; --i) {
            std::cout << answer[i] << " ";
        }
        std::cout << answer[0];
    }
    return 0;
}
