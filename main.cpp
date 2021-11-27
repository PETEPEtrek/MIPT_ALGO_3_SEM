#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

enum { ENDOFPARENT = -1, INF = 10000 };

struct DiffNums {
    int32_t Shifttoright(int32_t number) {
        return (number % 10) * 1000 + number / 10;
    }
    int32_t Plusfirstdigit(int32_t number) {
        if (number / 1000 != 9) {
            return number + 1000;
        }
        return number;
    }
    int32_t Shifttoleft(int32_t number) {
        return (number % 1000) * 10 + number / 1000;
    }
    int32_t Minusfirstdigit(int32_t number) {
        if (number % 10 != 1) {
            return number - 1;
        }
        return number;
    }
    bool FirstLesserThanNine(int32_t number) {
        return number / 1000 != 9;
    }

    bool LastNotOne(int32_t number) {
        return number % 10 != 1;
    }
};

class Graph {
public:
    using Vertex = int32_t;
    virtual std::vector<Vertex> Get_Neighbors(Vertex vertex) const = 0;
    Vertex Get_Num_Of_Verteses() const {
        return num_of_verteses;
    }
    virtual void AddEdge(const Vertex& first_vertex, const Vertex& second_vertex) = 0;
protected:
    bool IsDirected = true;
    int32_t num_of_edges = 0;
    Vertex num_of_verteses = 0;
};
class AdjList : public Graph {
    std::vector<std::vector<Vertex>> list_of_adjacency;

    std::vector<Vertex> Get_Neighbors(Vertex vertex) const override {
        return list_of_adjacency[vertex];
    }
public:
    AdjList(int32_t num_of_vertices) {
        num_of_verteses = num_of_vertices;
        num_of_edges = 0;
        list_of_adjacency.resize(num_of_verteses);
        for (Graph::Vertex i = 0; i < num_of_verteses; ++i) {
            list_of_adjacency[i] = std::vector<Graph::Vertex>();
        }
    }
    void AddEdge(const Vertex& first_vertex, const Vertex& second_vertex) {
        ++num_of_edges;
        list_of_adjacency[first_vertex].emplace_back(second_vertex);
        if (!IsDirected) {
            ++num_of_edges;
            list_of_adjacency[second_vertex].emplace_back(first_vertex);
        }
    }
    ~AdjList() = default;

};

void BuildGraph(Graph& graph) {
    DiffNums action{};
    for (int32_t number = 1000; number < INF; ++number) {
        std::string number_str = std::to_string(number);
        if (std::string::npos != number_str.find_first_of('0')) {
            continue;
        }
        Graph::Vertex second_number;
        if (action.LastNotOne(number)) {
            second_number = action.Minusfirstdigit(number);
            graph.AddEdge(number, second_number);
        }
        if (action.FirstLesserThanNine(number)) {
            second_number = action.Plusfirstdigit(number);
            graph.AddEdge(number, second_number);
        }
        second_number = action.Shifttoright(number);
        graph.AddEdge(number, second_number);
        second_number = action.Shifttoleft(number);
        graph.AddEdge(number, second_number);
    }
}
std::vector<Graph::Vertex> GetAnswerPath(const Graph::Vertex destination, std::vector<Graph::Vertex>& distance,
                                         std::vector<Graph::Vertex>& parent) {
    std::vector<Graph::Vertex> answer;
    if (distance[destination] == INF) {
        return answer;
    }
    Graph::Vertex vertex = destination;
    while (vertex != ENDOFPARENT) {
        answer.push_back(vertex);
        vertex = parent[vertex];
    }
    std::reverse(answer.begin(), answer.end());
    return answer;
}
std::vector<Graph::Vertex> ShorthestPathForHorse(const Graph& graph,
                                                 const Graph::Vertex& from,
                                                 const Graph::Vertex& to) {
    std::vector<Graph::Vertex> distance(graph.Get_Num_Of_Verteses(), INF);
    std::vector<Graph::Vertex> parent(graph.Get_Num_Of_Verteses(), ENDOFPARENT);
    std::queue<Graph::Vertex> queue;
    distance[from] = 0;
    queue.push(from);
    while (!queue.empty()) {
        Graph::Vertex vertex = queue.front();
        queue.pop();
        for (const auto& neighbor: graph.Get_Neighbors(vertex)) {
            if (distance[neighbor] == INF) {
                distance[neighbor] = distance[vertex] + 1;
                parent[neighbor] = vertex;
                queue.push(neighbor);
            }
        }
    }
    return GetAnswerPath(to, distance, parent);
}

void PrintShortestPath(const std::vector<Graph::Vertex>& answer) {
    if (answer.empty()) {
        std::cout << -1 << std::endl;
        return;
    }
    std::cout << answer.size() << std::endl;
    for (auto& vertex: answer) {
        std::cout << vertex << std::endl;
    }
}
int main() {
    AdjList graph(INF);
    BuildGraph(graph);
    Graph::Vertex from, to;
    std::cin >> from;
    std::cin >> to;
    std::vector<Graph::Vertex> answer = ShorthestPathForHorse(graph, from, to);
    PrintShortestPath(answer);
    return 0;
}

