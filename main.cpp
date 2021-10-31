#include <iostream>
#include <vector>
#include <queue>
enum { INF = 2147483647 };
class Graph {
public:
    typedef int64_t Vertex;
    Vertex Get_Num_Of_Verteses() const {
        return num_of_verteses;
    }
    virtual void Add_Edge(const Vertex& first_vertex, const Vertex& second_vertex) = 0;
    virtual std::vector<Vertex>& Get_Neighbors(Vertex v) = 0;
protected:
    bool IsDirected = true;
    int32_t num_of_edges = 0;
    Vertex num_of_verteses = 0;
};
class AdjList : public Graph {
    std::vector<std::vector<Graph::Vertex>> list_of_adjacency;
    std::vector<Vertex>& Get_Neighbors(Vertex v) override {
        return list_of_adjacency[v];
    }
public:
    explicit AdjList(int32_t desk_size) {
        num_of_verteses = desk_size;
        num_of_edges = 0;
        list_of_adjacency.resize(num_of_verteses + 1);
    }
    void Add_Edge(const Vertex& first_vertex, const Vertex& second_vertex) override {
        if (first_vertex >= num_of_verteses || second_vertex >= num_of_verteses || first_vertex < 0 || second_vertex < 0) {
            return;
        }
        ++num_of_edges;
        list_of_adjacency[first_vertex].push_back(second_vertex);
        if (!IsDirected) {
            ++num_of_edges;
            list_of_adjacency[second_vertex].push_back(first_vertex);
        }
    }
    ~AdjList() = default;

};

std::vector<int64_t> Shortest_Path_Between_Verteses(Graph& graph, const std::vector<Graph::Vertex>& subways) {
    std::vector<int64_t> distance(graph.Get_Num_Of_Verteses(), INF);
    std::queue<Graph::Vertex> queue;
    for (const auto& subway: subways) {
        queue.push(subway);
        distance[subway] = 0;
    }
    while (!queue.empty()) {
        Graph::Vertex v = queue.front();
        queue.pop();
        for (auto u: graph.Get_Neighbors(v)) {
            if (distance[u] == INF || distance[u] > distance[v] + 1) {
                distance[u] = distance[v] + 1;
                queue.push(u);
            }
        }
    }
    return distance;
}
void BuildCodeGraph(Graph& graph, const Graph::Vertex& number_of_columns) {
    for (Graph::Vertex v = 0; v < graph.Get_Num_Of_Verteses(); ++v) {
        Graph::Vertex num = v / number_of_columns * number_of_columns;
        graph.Add_Edge(v, v - number_of_columns);
        graph.Add_Edge(v, v + number_of_columns);
        if (v - 1 >= num && v - 1 < num + number_of_columns) {
            graph.Add_Edge(v, v - 1);
        }
        if (v + 1 >= num && v + 1 < num + number_of_columns) {
            graph.Add_Edge(v, v + 1);
        }
    }
}
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int32_t Size_of_desk_x, Size_of_desk_y;
    std::cin >> Size_of_desk_x >> Size_of_desk_y;
    AdjList graph(Size_of_desk_x * Size_of_desk_y);
    std::vector<std::vector<Graph::Vertex>> city{};
    std::vector<Graph::Vertex> subways;
    city.resize(Size_of_desk_x);
    for (int32_t x = 0; x < Size_of_desk_x; ++x) {
        city[x].resize(Size_of_desk_y, 0);
    }
    for (int32_t x = 0; x < Size_of_desk_x; ++x) {
        for (int32_t y = 0; y < Size_of_desk_y; ++y) {
            int32_t is_subway;
            std::cin >> is_subway;
            city[x][y] = is_subway;
            if (is_subway) {
                subways.push_back(x * Size_of_desk_y + y);
            }
        }
    }
    BuildCodeGraph(graph, Size_of_desk_y);
    std::vector<int64_t> answer = Shortest_Path_Between_Verteses(graph, subways);
    for (int64_t i = 0; i < answer.size(); ++i) {
        std::cout << answer[i] << ' ';
        if ((i + 1) % Size_of_desk_y == 0) {
            std::cout << std::endl;
        }
    }
    return 0;
}
