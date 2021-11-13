#include <iostream>
#include <vector>
#include <set>


class Graph {
public:
    typedef int64_t Vertex;
    Vertex Get_Number_Of_Verteses() const {
        return num_of_verteses;
    }
    virtual void Add_Edge(Graph::Vertex first_vertex, Graph::Vertex& second_vertex) = 0;
    virtual void Add_Trans_Edge(Graph::Vertex first_vertex, Graph::Vertex& second_vertex) = 0;
    virtual std::set<Graph::Vertex> Get_Neighbors(Graph::Vertex v) const = 0;
    virtual bool is_connection(Graph::Vertex first_vertex, Graph::Vertex second_vertex) = 0;
protected:
    bool IsDirected = true;
    int32_t num_of_edges = 0;
    Vertex num_of_verteses = 1;
};
class AdjList : public Graph {
    std::vector<std::set<Graph::Vertex>> list_of_adjacency;
    std::set<Graph::Vertex> Get_Neighbors(Graph::Vertex v) const override {
        return list_of_adjacency[v];
    }
public:
    AdjList(Vertex verteses) {
        num_of_verteses = verteses;
        num_of_edges = 0;
        list_of_adjacency.resize(num_of_verteses + 1);
    }
    void Add_Edge(Graph::Vertex first_vertex, Graph::Vertex& second_vertex) override {
        if (first_vertex != second_vertex) {
            ++num_of_edges;
            list_of_adjacency[first_vertex].emplace(second_vertex);
        }
    }
    void Add_Trans_Edge(Graph::Vertex first_vertex, Graph::Vertex& second_vertex) override {
        if (first_vertex != second_vertex) {
            ++num_of_edges;
            list_of_adjacency[second_vertex].emplace(first_vertex);
        }
    }
    ~AdjList() = default;
    bool is_connection(Graph::Vertex first_vertex, Graph::Vertex second_vertex) override {
        return list_of_adjacency[first_vertex].count(second_vertex);
    }
};

void dfs_visit_for_graph(Graph& graph, std::vector<bool>& used, Graph::Vertex& v, std::vector<Graph::Vertex>& special_order) {
    used[v] = true;
    for (auto u: graph.Get_Neighbors(v)) {
        if (!used[u]) {
            dfs_visit_for_graph(graph, used, u, special_order);
        }
    }
    special_order.push_back(v);
}
void dfs_visit_for_trans_graph(Graph& trans_graph,
                               std::vector<bool>& used,
                               Graph::Vertex& v,
                               std::vector<Graph::Vertex>& csc) {
    used[v] = true;
    csc.push_back(v);
    for (auto u: trans_graph.Get_Neighbors(v)) {
        if (!used[u]) {
            dfs_visit_for_trans_graph(trans_graph, used, u, csc);
        }
    }
}
std::vector<std::vector<Graph::Vertex>> Cosarayu(Graph& graph, Graph& trans_graph) {
    std::vector<std::vector<Graph::Vertex>> answer;
    std::vector<bool> used(graph.Get_Number_Of_Verteses() + 1, false);
    std::vector<Graph::Vertex> special_order;
    std::vector<Graph::Vertex> components;
    for (int64_t i = 1; i <= graph.Get_Number_Of_Verteses(); ++i) {
            if (!used[i]) {
                dfs_visit_for_graph(graph, used, i, special_order);
            }
    }
    used.clear();
    used.resize(graph.Get_Number_Of_Verteses() + 1, false);
    for (int64_t i = 1; i <= graph.Get_Number_Of_Verteses(); ++i) {
            if (!used[i]) {
                dfs_visit_for_trans_graph(trans_graph, used, i, components);
                answer.push_back(components);
                components.clear();
            }
    }
    return answer;
}
void Answer(Graph& graph, std::vector<std::vector<Graph::Vertex>>& answer) {
    int32_t n = 0;
    bool opt = false;
    for (int64_t i = 0; i < answer.size(); ++i) {
        for (int64_t j = i + 1; j < answer.size(); ++j) {
            for (const auto& item_first: answer[i]) {
                for (const auto& item_second: answer[j]) {
                    opt = graph.is_connection(item_first, item_second);
                    if (opt) {
                        break;
                    }
                }
                if (opt) {
                    break;
                }
            }
            if (opt) {
                break;
            }
        }
        if (!opt) {
            ++n;
        } else {
            opt = false;
        }
    }
    std::cout << n;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int32_t num_of_verteses;
    std::cin >> num_of_verteses;
    AdjList graph(num_of_verteses);
    AdjList trans_graph(num_of_verteses);
    for (int32_t i = 1; i <= num_of_verteses; ++i) {
        Graph::Vertex pig;
        std::cin >> pig;
        graph.Add_Edge(i, pig);
        trans_graph.Add_Trans_Edge(i, pig);
    }
    std::vector<std::vector<Graph::Vertex>> answer = Cosarayu(graph, trans_graph);
    Answer(graph, answer);
    return 0;
}
