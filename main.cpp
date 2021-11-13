#include <iostream>
#include <vector>
#include <set>

class Graph {
public:
    typedef int64_t Vertex;
    Vertex Get_Num_Of_Verteses() const {
        return num_of_verteses;
    }
    virtual void Add_Edge(Graph::Vertex a, Graph::Vertex& b) = 0;
    virtual std::vector<Graph::Vertex>& Get_Neighbors(Graph::Vertex v) = 0;
protected:
    struct Connect {
        Vertex vertex;
        int64_t number;
        int64_t quantity_edge = 0;
    };

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
        bool is_in_list = false;
        for (auto &elem : list_of_adjacency[first_vertex]) {
            if (elem.vertex == second_vertex) {
                is_in_list = true;
                elem.quantity_edge++;
                break;
            }
        }
        if (is_in_list && !IsDirected) {
            for (auto &elem : list_of_adjacency[second_vertex]) {
                if (elem.vertex == first_vertex) {
                    elem.quantity_edge++;
                    return;
                }
            }
        }
        Connect tmp;
        tmp.vertex = second_vertex;
        tmp.number = number_edge;
        tmp.quantity_edge = 1;
        list[a].push_back(tmp);

        if (!is_oriented) {
            tmp.vertex = a;
            tmp.number = number_edge;
            tmp.quantity_edge = 1;
            list[b].push_back(tmp);
        }

        ++number_edge;
        list_of_adjacency[first_vertex].push_back(second_vertex);
        if (!IsDirected) {
            list_of_adjacency[second_vertex].push_back(first_vertex);
        }
    }
    ~AdjList() = default;
};
void dfs_visit(Graph& graph,
               Graph::Vertex vertex,
               Graph::Vertex backvertex,
               int32_t& time,
               std::vector<bool>& used,
               std::set<Graph::Vertex>& artpoints,
               std::vector<int32_t>& time_up,
               std::vector<int32_t>& time_in) {
    time_up[vertex] = ++time;
    time_in[vertex] = time;
    used[vertex] = true;
    int32_t number = 0;
    for (const auto& neighbor: graph.Get_Neighbors(vertex)) {
        if (neighbor == backvertex) {
            continue;
        }
        if (used[neighbor]) {
            time_up[vertex] = std::min(time_up[vertex], time_in[neighbor]);
        } else {
            dfs_visit(graph, neighbor, vertex, time, used, artpoints, time_up, time_in);
            time_up[vertex] = std::min(time_up[vertex], time_up[neighbor]);
            if (backvertex != -1 && time_up[neighbor] >= time_in[vertex]) {
                artpoints.emplace(vertex);
            }
            ++number;
        }
    }
    if (backvertex == -1 && number >= 2) {
        artpoints.emplace(vertex);
    }
}
void dfs(Graph& graph,
         int32_t& time,
         std::vector<bool>& used,
         std::set<Graph::Vertex>& artpoints,
         std::vector<int32_t>& time_up,
         std::vector<int32_t>& time_in) {
    for (int32_t i = 1; i <= graph.Get_Num_Of_Verteses(); ++i) {
        if (!used[i]) {
            dfs_visit(graph, i, -1, time, used, artpoints, time_up, time_in);
        }
    }
}
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int32_t num_of_verteses, num_of_maglev;
    std::cin >> num_of_verteses >> num_of_maglev;
    AdjList graph(num_of_verteses + (num_of_maglev + 1), 0);
    int64_t current_num_of_maglev = num_of_verteses;
    for (int32_t i = 1; i <= num_of_maglev; ++i) {
        ++current_num_of_maglev;
        Graph::Vertex first_scyscraper, second_scyscraper, third_scyscraper;
        std::cin >> first_scyscraper >> second_scyscraper >> third_scyscraper;
        graph.Add_Edge(first_scyscraper, current_num_of_maglev);
        graph.Add_Edge(second_scyscraper, current_num_of_maglev);
        graph.Add_Edge(third_scyscraper, current_num_of_maglev);
    }
    std::vector<bool> used(graph.Get_Num_Of_Verteses() + 1, false);
    std::set<Graph::Vertex> artpoints;
    std::vector<int32_t> time_up(graph.Get_Num_Of_Verteses() + 1, 0);
    std::vector<int32_t> time_in(graph.Get_Num_Of_Verteses() + 1, 0);
    int32_t time = 0;
    dfs(graph, time, used, artpoints, time_up, time_in);
    std::vector<Graph::Vertex> answer;
    for (const auto& artpoint: artpoints) {
        if (artpoint > num_of_verteses) {
            answer.push_back(artpoint - num_of_verteses);
        }
    }
    std::cout << answer.size() << std::endl;
    for (const auto& artpoint: answer) {
        std::cout << artpoint << std::endl;
    }
    return 0;
}
