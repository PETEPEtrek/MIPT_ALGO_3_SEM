#include <iostream>
#include <vector>
#include <set>
class Graph {
public:
    typedef int64_t Vertex;
    Vertex Get_Number_Of_Verteses() const {
        return num_of_verteses;
    }
    virtual void Add_Edge(Graph::Vertex a, Graph::Vertex& b) = 0;
    virtual std::set<Graph::Vertex> Get_Neighbors(Graph::Vertex v) const = 0;
    virtual bool is_connection(Graph::Vertex first_vertex, Graph::Vertex second_vertex) = 0;
    virtual int32_t Num_of_neighbors(Graph::Vertex vertex) = 0;
protected:
    bool IsDirected = false;
    int32_t num_of_edges = 0;
    Vertex num_of_verteses = 1;
};
class AdjList : public Graph {
    std::vector<std::set<Graph::Vertex>> list_of_adjacency;
    std::set<Graph::Vertex> Get_Neighbors(Graph::Vertex v) const override {
        return list_of_adjacency[v];
    }
public:
    AdjList(int32_t verteses, int32_t edges) {
        num_of_verteses = verteses;
        num_of_edges = edges * 2;
        list_of_adjacency.resize(num_of_verteses + 1);
    }
    void Add_Edge(Graph::Vertex first_vertex, Graph::Vertex& second_vertex) override {
        list_of_adjacency[first_vertex].emplace(second_vertex);
        if (!IsDirected) {
            list_of_adjacency[second_vertex].emplace(first_vertex);
        }
    }

    int32_t Num_of_neighbors(Graph::Vertex vertex) override {
        return list_of_adjacency[vertex].size();
    }
    bool is_connection(Graph::Vertex first_vertex, Graph::Vertex second_vertex) override {
        return list_of_adjacency[first_vertex].count(second_vertex);
    }
    ~AdjList() = default;
};
bool IsNotBridge(std::vector<Graph::Vertex>& component, Graph::Vertex current_vertex) {
    for (auto& vertex: component) {
        if (vertex == current_vertex) {
            return true;
        }
    }
    return false;
}
void Numeration(const Graph& graph,
                Graph::Vertex vertex,
                int32_t num,
                std::vector<Graph::Vertex>& nums,
                int32_t& Number_of_component,
                std::vector<int32_t>& time_up,
                std::vector<int32_t>& time_in,
                std::vector<std::vector<int64_t>>& components) {
    nums[vertex] = num;
    components[num - 1].push_back(vertex);
    for (auto& neighbor: graph.Get_Neighbors(vertex)) {
        if (nums[neighbor] == 0) {
            if (time_up[neighbor] > time_in[vertex]) {
                ++Number_of_component;
                Numeration(graph,
                           neighbor,
                           Number_of_component,
                           nums,
                           Number_of_component,
                           time_up,
                           time_in,
                           components);
            } else {
                Numeration(graph, neighbor, num, nums, Number_of_component, time_up, time_in, components);
            }
        }
    }
}
void dfs_visit(const Graph& graph,
               Graph::Vertex vertex,
               Graph::Vertex backvertex,
               int32_t& time,
               std::vector<bool>& used,
               std::vector<int32_t>& time_up,
               std::vector<int32_t>& time_in) {
    time_up[vertex] = ++time;
    time_in[vertex] = time;
    used[vertex] = true;
    for (const auto& neighbor: graph.Get_Neighbors(vertex)) {
        if (neighbor == backvertex) {
            continue;
        }
        if (!used[neighbor]) {
            dfs_visit(graph,
                      neighbor,
                      vertex,
                      time,
                      used,
                      time_up,
                      time_in);
            time_up[vertex] = std::min(time_up[vertex], time_up[neighbor]);
        } else {
            time_up[vertex] = std::min(time_up[vertex], time_in[neighbor]);
        }
    }
}
std::vector<std::vector<Graph::Vertex>> TwoEdgeComponents(const Graph& graph) {
    std::vector<std::vector<Graph::Vertex>> components(graph.Get_Number_Of_Verteses() + 1);
    int32_t time = 0;
    int32_t number = 0;
    std::vector<bool> used(graph.Get_Number_Of_Verteses() + 1, false);
    std::vector<int32_t> time_up(graph.Get_Number_Of_Verteses() + 1, 0);
    std::vector<int32_t> time_in(graph.Get_Number_Of_Verteses() + 1, 0);
    std::vector<Graph::Vertex> nums(graph.Get_Number_Of_Verteses() + 1, 0);
    for (auto count = 1; count <= graph.Get_Number_Of_Verteses(); ++count) {
        if (!used[count]) {
            dfs_visit(graph, count, 0, time, used, time_up, time_in);
        }
    }
    for (auto i = 1; i <= graph.Get_Number_Of_Verteses(); ++i) {
        if (nums[i] == 0) {
            ++number;
            Numeration(graph, i, number, nums, number, time_up, time_in, components);
        }
    }
    return components;
}
int32_t Answer(const Graph& graph) {
    std::vector<std::vector<Graph::Vertex>> two_edge_components = TwoEdgeComponents(graph);
    int32_t number_of_leaves = 0;
    int32_t number_of_bridges = 0;
    for (auto& component: two_edge_components) {
        if (component.empty()) {
            continue;
        }
        number_of_bridges = 0;
        for (Graph::Vertex vertex = 0; vertex < component.size(); ++vertex) {
            for (const auto& neighbor: graph.Get_Neighbors(component[vertex])) {
                if (!IsNotBridge(component, neighbor)) {
                    ++number_of_bridges;
                }
            }
        }
        if (number_of_bridges == 1) {
            ++number_of_leaves;
        }
    }
    return !(number_of_leaves % 2) ? number_of_leaves / 2 : number_of_leaves / 2 + 1;
}
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int32_t num_of_verteses, num_of_edges;
    std::cin >> num_of_verteses >> num_of_edges;
    AdjList graph(num_of_verteses, num_of_edges);
    for (int32_t i = 1; i <= num_of_edges; ++i) {
        Graph::Vertex first_vertex, second_vertex;
        std::cin >> first_vertex >> second_vertex;
        graph.Add_Edge(first_vertex, second_vertex);
    }
    std::cout << Answer(graph);
    return 0;
}
