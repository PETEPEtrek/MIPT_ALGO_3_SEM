#include <iostream>
#include <vector>
#include <algorithm>
class Graph {
public:
    typedef int64_t Vertex;
    struct Connect {
        Vertex vertex;
        int64_t number_of_edge;
    };
    Vertex Get_Num_Of_Verteses() const {
        return num_of_verteses;
    }
    virtual void Add_Edge(Graph::Vertex a, Graph::Vertex& b) = 0;
    virtual std::vector<Connect>& Get_Neighbors(Graph::Vertex v) = 0;
protected:
    bool IsDirected = false;
    int32_t num_of_edges = 0;
    Vertex num_of_verteses = 1;
};
class AdjList : public Graph {
    std::vector<std::vector<Connect>> list_of_adjacency;
    std::vector<Connect>& Get_Neighbors(Graph::Vertex v) override {
        return list_of_adjacency[v];
    }
public:
    AdjList(int32_t verteses, int32_t edges) {
        num_of_verteses = verteses;
        num_of_edges = edges;
        list_of_adjacency.resize(num_of_verteses + 1);
    }
    void Add_Edge(Graph::Vertex first_vertex, Graph::Vertex& second_vertex) override {
        bool is_in = false;
        for (auto& elem: list_of_adjacency[first_vertex]) {
            if (elem.vertex == second_vertex) {
                is_in = true;
                break;
            }
        }
        if (is_in && !IsDirected) {
            for (auto& elem: list_of_adjacency[second_vertex]) {
                if (elem.vertex == first_vertex) {
                    return;
                }
            }
        }
        Connect connection;
        connection.vertex = second_vertex;
        connection.number_of_edge = num_of_edges;
        list_of_adjacency[first_vertex].push_back(connection);
        if (!IsDirected) {
            connection.vertex = first_vertex;
            connection.number_of_edge = num_of_edges;
            list_of_adjacency[second_vertex].push_back(connection);
        }

        ++num_of_edges;
    }
    ~AdjList() = default;
};
void dfs_visit(Graph& graph,
               Graph::Vertex vertex,
               Graph::Vertex backvertex,
               int32_t& time,
               std::vector<bool>& used,
               std::vector<Graph::Vertex>& answer,
               std::vector<int32_t>& time_up,
               std::vector<int32_t>& time_in) {
    time_up[vertex] = ++time;
    time_in[vertex] = time;
    used[vertex] = true;
    int64_t count = 0;
    for (const auto& neighbor: graph.Get_Neighbors(vertex)) {
        if (neighbor.vertex == backvertex) {
            continue;
        }
        if (used[neighbor.vertex]) {
            time_up[vertex] = std::min(time_up[vertex], time_in[neighbor.vertex]);
        } else {
            dfs_visit(graph, neighbor.vertex, vertex, time, used, answer, time_up, time_in);
            time_up[vertex] = std::min(time_up[vertex], time_up[neighbor.vertex]);
            if (time_up[neighbor.vertex] > time_in[vertex]) {
                answer.push_back(neighbor.number_of_edge + 1);
            }
        }
        ++count;
    }
}
void dfs(Graph& graph,
         int32_t& time,
         std::vector<bool>& used,
         std::vector<Graph::Vertex>& answer,
         std::vector<int32_t>& time_up,
         std::vector<int32_t>& time_in) {
    for (int32_t i = 1; i <= graph.Get_Num_Of_Verteses(); ++i) {
        if (!used[i]) {
            dfs_visit(graph, i, -1, time, used, answer, time_up, time_in);
        }
    }
}
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int32_t num_of_verteses, num_of_edges;
    std::cin >> num_of_verteses >> num_of_edges;
    std::vector<Graph::Vertex> answer;
    AdjList graph(num_of_verteses, 0);
    for (int32_t i = 1; i <= num_of_edges; ++i) {
        Graph::Vertex first_vertex, second_vertex;
        std::cin >> first_vertex >> second_vertex;
        graph.Add_Edge(first_vertex, second_vertex);
    }
    std::vector<bool> used(graph.Get_Num_Of_Verteses() + 1, false);
    std::vector<int32_t> time_up(graph.Get_Num_Of_Verteses() + 1, 0);
    std::vector<int32_t> time_in(graph.Get_Num_Of_Verteses() + 1, 0);
    int32_t time = 0;
    dfs(graph, time, used, answer, time_up, time_in);
    std::cout << answer.size() << std::endl;
    std::sort(answer.begin(), answer.end());
    for (const auto& ans: answer) {
        std::cout << ans << " ";
    }
    return 0;
}
