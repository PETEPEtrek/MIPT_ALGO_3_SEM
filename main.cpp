#include <iostream>
#include <vector>
#include <unordered_map>
#include <list>
enum { INF = 2147483647 };

class Graph {
public:
    typedef size_t Vertex;
    virtual Graph::Vertex& Get_Number_Of_Verteses() = 0;
    virtual void Add_Edge(Vertex& first_vertex, Vertex& second_vertex, int32_t& value) = 0;
    virtual std::list<std::pair<Vertex, int32_t>>& Get_Neighbors(Vertex v) = 0;
protected:
    bool IsDirected = true;
    int32_t num_of_edges = 0;
    Vertex num_of_verteses = 0;
};
class AdjList : public Graph {
    std::vector<std::list<std::pair<Vertex, int32_t>>> list_of_adjacency;
    std::list<std::pair<Vertex, int32_t>>& Get_Neighbors(Vertex vertex) override {
        return list_of_adjacency[vertex];
    }
public:
    Graph::Vertex& Get_Number_Of_Verteses() override {
        return num_of_verteses;
    }
    AdjList(Vertex verteses, int32_t number_of_edges) {
        num_of_verteses = verteses;
        num_of_edges = number_of_edges;
        list_of_adjacency.resize(num_of_verteses + 1);
        for (int32_t i = 0; i <= num_of_verteses; ++i) {
            list_of_adjacency[i].resize(num_of_verteses + 1);
        }
    }
    void Add_Edge(Vertex& first_vertex, Vertex& second_vertex, int32_t& value) override {
        list_of_adjacency[first_vertex].emplace_back(std::make_pair(second_vertex, value));
        if (!IsDirected) {
            list_of_adjacency[second_vertex].emplace_back(std::make_pair(first_vertex, value));
        }
    }
    ~AdjList() = default;

};

int32_t Shortest_weighted_path(Graph& graph, Graph::Vertex& start, Graph::Vertex& end, int32_t& max_value) {
    std::vector<std::pair<int32_t, std::list<Graph::Vertex>::iterator>> distance(graph.Get_Number_Of_Verteses() + 1);
    for (int i = 0; i <= graph.Get_Number_Of_Verteses(); i++) {
        distance[i].first = INF;
    }
    std::vector<bool> used(graph.Get_Number_Of_Verteses() + 1, false);
    distance[start].first = 0;
    std::list<Graph::Vertex> list[max_value + 1];
    list[0].push_back(start);
    int32_t pos = 0;
    while (1) {
        while (list[pos % (max_value + 1)].empty() && pos < max_value * graph.Get_Number_Of_Verteses()) {
            ++pos;
        }
        if (pos == max_value * graph.Get_Number_Of_Verteses()) {
            break;
        }
        Graph::Vertex vertex = list[pos % (max_value + 1)].front();
        list[pos % (max_value + 1)].pop_front();
        auto list_ex = graph.Get_Neighbors(vertex);
        for (auto& neighbor: list_ex) {
            Graph::Vertex neighbor_vertex = neighbor.first;
            int32_t weight = neighbor.second;

            int32_t du = distance[vertex].first;
            int32_t dv = distance[neighbor_vertex].first;
            if (dv > du + weight) {
                if (dv != INF) {
                    list[dv % (max_value + 1)].erase(distance[neighbor_vertex].second);
                }
                distance[neighbor_vertex].first = du + weight;
                dv = distance[neighbor_vertex].first;
                list[dv % (max_value + 1)].push_front(neighbor_vertex);
                distance[neighbor_vertex].second = list[dv % (max_value + 1)].begin();
            }
        }
    }
    return distance[end].first;
}
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int32_t num_of_verteses, num_of_edges;
    std::cin >> num_of_verteses >> num_of_edges;
    AdjList graph(num_of_verteses, num_of_edges);
    Graph::Vertex from, to;
    std::cin >> from >> to;
    int32_t max_value = 0;
    for (int32_t i = 0; i < num_of_edges; ++i) {
        Graph::Vertex first_vertex, second_vertex;
        int32_t value;
        std::cin >> first_vertex >> second_vertex >> value;
        if (value > max_value) {
            max_value = value;
        }
        graph.Add_Edge(first_vertex, second_vertex, value);
    }
    int32_t path = Shortest_weighted_path(graph, from, to, max_value);
    if (path == INF) {
        std::cout << "-1";
    } else {
        std::cout << path;
    }
    return 0;
}
