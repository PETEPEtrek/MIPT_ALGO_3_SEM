#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
enum { MAX_VALUE = 31, INF = 2147483647 };

class Graph {
public:
    typedef size_t Vertex;
    virtual Graph::Vertex& Get_Number_Of_Verteses() = 0;
    virtual void Add_Edge(Vertex& first_vertex, Vertex& second_vertex, int32_t& value) = 0;
    virtual std::vector<std::pair<Vertex, int32_t>>& Get_Neighbors(Vertex v) = 0;
protected:
    bool IsDirected = true;
    int32_t num_of_edges = 0;
    Vertex num_of_verteses = 0;
};
class AdjList : public Graph {
    std::vector<std::vector<std::pair<Vertex, int32_t>>> list_of_adjacency;
    std::vector<std::pair<Vertex, int32_t>>& Get_Neighbors(Vertex vertex) override {
        return list_of_adjacency[vertex];
    }
public:
    Graph::Vertex& Get_Number_Of_Verteses() override {
        return num_of_verteses;
    }
    AdjList(Vertex verteses, int32_t number_of_edges) {
        num_of_verteses = verteses;
        num_of_edges = number_of_edges;
        list_of_adjacency.resize(num_of_verteses);
    }
    void Add_Edge(Vertex& first_vertex, Vertex& second_vertex, int32_t& value) override {
        list_of_adjacency[first_vertex - 1].emplace_back(std::make_pair(second_vertex - 1, value));
        if (!IsDirected) {
            list_of_adjacency[second_vertex - 1].emplace_back(std::make_pair(first_vertex - 1, value));
        }
    }
    ~AdjList() = default;

};

int32_t Shortest_weighted_path(Graph& graph, Graph::Vertex& start, Graph::Vertex& end) {
    std::vector<std::queue<Graph::Vertex>> queues(MAX_VALUE);
    std::vector<int32_t> distance(graph.Get_Number_Of_Verteses(), INF);
    distance[start] = 0;
    queues[0].push(start);
    for (int32_t pos = 0; pos <= MAX_VALUE * graph.Get_Number_Of_Verteses(); ++pos) {
        while (!queues[pos % MAX_VALUE].empty()) {
            Graph::Vertex vertex = queues[pos % MAX_VALUE].front();
            queues[pos % MAX_VALUE].pop();
            for (const auto& neighbor: graph.Get_Neighbors(vertex)) {
                if ((distance[vertex] + neighbor.second < distance[neighbor.first])
                    || (distance[neighbor.first] == INF)) {
                    distance[neighbor.first] = distance[vertex] + neighbor.second;
                    queues[(neighbor.second + pos) % MAX_VALUE].push(neighbor.first);
                }
            }
        }
    }
    return distance[end];
}
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int32_t num_of_verteses, num_of_edges;
    std::cin >> num_of_verteses >> num_of_edges;
    AdjList graph(num_of_verteses, num_of_edges);
    Graph::Vertex from, to;
    std::cin >> from >> to;
    for (int32_t i = 0; i < num_of_edges; ++i) {
        Graph::Vertex first_vertex, second_vertex;
        int32_t value;
        std::cin >> first_vertex >> second_vertex >> value;
        graph.Add_Edge(first_vertex, second_vertex, value);
    }
    --from;
    --to;
    int32_t path = Shortest_weighted_path(graph, from, to);
    if (path == INF) {
        std::cout << "-1";
    } else {
        std::cout << path;
    }
    return 0;
}
