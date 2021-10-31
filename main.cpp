#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
struct pair_hash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ h2;
    }
};
class Graph {
public:
    typedef int64_t Vertex;
    Vertex Get_Num_Of_Verteses() const {
        return num_of_verteses;
    }
    virtual void Add_Edge(Graph::Vertex a, Graph::Vertex& b) = 0;
    virtual std::multiset<Graph::Vertex> Get_Neighbors(Graph::Vertex v) const = 0;
    virtual bool IsOnly(Graph::Vertex first_vertex, Graph::Vertex second_vertex) const = 0;
protected:
    bool IsDirected = false;
    int32_t num_of_edges = 0;
    Vertex num_of_verteses = 1;
};
class AdjList : public Graph {
    std::vector<std::multiset<Graph::Vertex>> list_of_adjacency;
    std::multiset<Graph::Vertex> Get_Neighbors(Graph::Vertex v) const override {
        return list_of_adjacency[v];
    }
public:
    AdjList(int32_t verteses, int32_t edges) {
        num_of_verteses = verteses;
        num_of_edges = edges;
        list_of_adjacency.resize(num_of_verteses + 1);
    }
    void Add_Edge(Graph::Vertex first_vertex, Graph::Vertex& second_vertex) override {
        list_of_adjacency[first_vertex].emplace(second_vertex);
        if (!IsDirected) {
            list_of_adjacency[second_vertex].emplace(first_vertex);
        }
    }
    bool IsOnly(Graph::Vertex first_vertex, Graph::Vertex second_vertex) const override {
        return list_of_adjacency[first_vertex].count(second_vertex) == 1;
    }
    ~AdjList() = default;
};
void dfs_visit(const Graph& graph,
               Graph::Vertex vertex,
               Graph::Vertex backvertex,
               int32_t& time,
               std::vector<bool>& used,
               std::unordered_map<std::pair<Graph::Vertex, Graph::Vertex>, int32_t, pair_hash>& nice_edges,
               std::unordered_map<int32_t, bool>& ans,
               std::vector<int32_t>& time_up,
               std::vector<int32_t>& time_in) {
    time_up[vertex] = ++time;
    time_in[vertex] = time;
    used[vertex] = true;
    for (const auto& neighbor: graph.Get_Neighbors(vertex)) {
        if (neighbor == backvertex) {
            continue;
        }
        if (used[neighbor]) {
            time_up[vertex] = std::min(time_up[vertex], time_in[neighbor]);
        } else {
            dfs_visit(graph, neighbor, vertex, time, used, nice_edges, ans, time_up, time_in);
            time_up[vertex] = std::min(time_up[vertex], time_up[neighbor]);
            if (time_up[neighbor] > time_in[vertex] && graph.IsOnly(vertex, neighbor)) {
                ans[nice_edges[{vertex, neighbor}]] = true;
            }
        }
    }
}
void dfs(const Graph& graph,
         Graph::Vertex vertex,
         Graph::Vertex backvertex,
         int32_t& time,
         std::vector<bool>& used,
         std::unordered_map<std::pair<Graph::Vertex, Graph::Vertex>, int32_t, pair_hash>& nice_edges,
         std::unordered_map<int32_t, bool>& ans,
         std::vector<int32_t>& time_up,
         std::vector<int32_t>& time_in) {
    for (int32_t i = 1; i <= graph.Get_Num_Of_Verteses(); ++i) {
        if (!used[i]) {
            dfs_visit(graph, i, -1, time, used, nice_edges, ans, time_up, time_in);
        }
    }
}
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int32_t num_of_verteses, num_of_edges;
    std::cin >> num_of_verteses >> num_of_edges;
    std::unordered_map<std::pair<Graph::Vertex, Graph::Vertex>, int32_t, pair_hash> nice_edges;
    std::unordered_map<int32_t, bool> ans;
    AdjList graph(num_of_verteses, num_of_edges);
    for (int32_t i = 1; i <= num_of_edges; ++i) {
        Graph::Vertex first_vertex, second_vertex;
        std::cin >> first_vertex >> second_vertex;
        graph.Add_Edge(first_vertex, second_vertex);
        nice_edges[{first_vertex, second_vertex}] = i;
        nice_edges[{second_vertex, first_vertex}] = i;
        ans[i] = false;
    }
    std::vector<bool> used(graph.Get_Num_Of_Verteses() + 1, false);
    std::vector<int32_t> time_up(graph.Get_Num_Of_Verteses() + 1, 0);
    std::vector<int32_t> time_in(graph.Get_Num_Of_Verteses() + 1, 0);
    int32_t time = 0;
    dfs(graph, 1, -1, time, used, nice_edges, ans, time_up, time_in);
    int32_t num = 0;
    std::vector<int32_t> answer;
    for (int32_t count = 1; count <= num_of_edges; ++count) {
        if (ans[count]) {
            ++num;
            answer.push_back(count);
        }
    }
    std::cout << num << std::endl;
    for (const auto& item: answer) {
        std::cout << item << std::endl;
    }
    return 0;
}

