#include <iostream>
#include <vector>
#include <list>
class Graph {
public:
    typedef int64_t Vertex;
    Vertex Get_Number_Of_Verteses() const {
        return num_of_verteses;
    }
    virtual void Add_Edge(Graph::Vertex a, Graph::Vertex& b) = 0;
    virtual std::list<Graph::Vertex>& Get_Neighbors(Graph::Vertex v) = 0;
    virtual int32_t Num_of_neighbors(Graph::Vertex vertex) = 0;
protected:
    bool IsDirected = true;
    int32_t num_of_edges = 0;
    Vertex num_of_verteses = 1;
};
class AdjList : public Graph {
    std::vector<std::list<Graph::Vertex>> list_of_adjacency;
    std::list<Graph::Vertex>& Get_Neighbors(Graph::Vertex v) override {
        return list_of_adjacency[v];
    }
public:
    AdjList(int32_t verteses, int32_t edges) {
        num_of_verteses = verteses;
        num_of_edges = edges;
        list_of_adjacency.resize(num_of_verteses + 1);
    }
    void Add_Edge(Graph::Vertex first_vertex, Graph::Vertex& second_vertex) override {
        ++num_of_edges;
        list_of_adjacency[first_vertex].push_back(second_vertex);
        if (!IsDirected) {
            list_of_adjacency[second_vertex].push_back(first_vertex);
        }
    }
    int32_t Num_of_neighbors(Graph::Vertex vertex) override {
        return list_of_adjacency[vertex].size();
    }
    ~AdjList() = default;
};
void dfs_for_connection(Graph& graph, Graph::Graph::Vertex vertex, std::vector<bool>& used) {
    used[vertex] = true;
    for (const auto& neighbor: graph.Get_Neighbors(vertex)) {
        if (!used[neighbor]) {
            dfs_for_connection(graph, neighbor, used);
        }
    }
}
bool CheckForConnection(Graph& graph, std::vector<bool>& IsIsolated, std::vector<bool>& used) {
    Graph::Vertex not_empty_vertex = 1;
    for (Graph::Vertex vertex = 1; vertex <= graph.Get_Number_Of_Verteses(); ++vertex) {
        if (!IsIsolated[vertex]) {
            not_empty_vertex = vertex;
            break;
        }
    }
    dfs_for_connection(graph, not_empty_vertex, used);
    for (Graph::Vertex vertex = 1; vertex <= graph.Get_Number_Of_Verteses(); ++vertex) {
        if (!used[vertex] && !IsIsolated[vertex]) {
            return false;
        }
    }
    return true;
}
void Answer(Graph& graph, Graph::Vertex& vertex, std::vector<Graph::Vertex>& required_trail) {
    Graph::Vertex next_vertex;
    auto next = graph.Get_Neighbors(vertex).begin();
    while (!graph.Get_Neighbors(vertex).empty()) {
        next_vertex = *next;
        next = graph.Get_Neighbors(vertex).erase(next);
        Answer(graph, next_vertex, required_trail);
    }
    required_trail.push_back(vertex);
}
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int32_t num_of_verteses, num_of_trails;
    std::cin >> num_of_trails >> num_of_verteses;
    if (num_of_trails == 0) {
        std::cout << 0;
        return 0;
    }
    AdjList graph(num_of_verteses, 0);
    std::vector<bool> IsIsolated(num_of_verteses + 1, true);
    std::vector<bool> used(graph.Get_Number_Of_Verteses() + 1);
    for (int32_t trail = 0; trail < num_of_trails; ++trail) {
        int64_t verteses, first_vertex, second_vertex;
        std::cin >> verteses >> first_vertex;
        for (int32_t i = 1; i <= verteses; ++i) {
            std::cin >> second_vertex;
            graph.Add_Edge(first_vertex, second_vertex);
            IsIsolated[first_vertex] = false;
            IsIsolated[second_vertex] = false;
            first_vertex = second_vertex;
        }
    }
    if (!CheckForConnection(graph, IsIsolated, used)) {
        std::cout << 0;
        return 0;
    }
    std::vector<Graph::Vertex> answer;
    Graph::Vertex not_empty_vertex = 1;
    for (Graph::Vertex vertex = 1; vertex <= graph.Get_Number_Of_Verteses(); ++vertex) {
        if (!IsIsolated[vertex]) {
            not_empty_vertex = vertex;
            break;
        }
    }
    Answer(graph, not_empty_vertex, answer);
    std::cout << answer.size() << ' ';
    for (Graph::Vertex station = 1; station <= answer.size(); ++station) {
        std::cout << answer[answer.size() - station] << ' ';
    }
    return 0;
}
