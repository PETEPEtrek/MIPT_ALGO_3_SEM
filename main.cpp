#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
#include <queue>

class Graph {
public:
    typedef int64_t Vertex;
    Vertex Get_Num_Of_Verteses() const {
        return num_of_verteses;
    }
    virtual void Add_Edge(Vertex a) = 0;
    virtual std::set<Vertex> Get_Neighbors(Vertex v) const = 0;
protected:
    bool IsDirected = false;
    Vertex num_of_edges = 0;
    Vertex num_of_verteses = 0;
    std::unordered_map<Vertex, bool> used_cells;
};
class AdjList : public Graph {
    std::unordered_map<Vertex, std::set<Vertex>> list_of_adjacency;

    std::set<Vertex> Get_Neighbors(Vertex v) const override {
        return list_of_adjacency.at(v);
    }
public:
    AdjList() {
        num_of_verteses = 1;
        num_of_edges = 0;
    }
    void Add_Edge(Vertex number) override {
        used_cells[number] = true;
        Vertex
            first = (number % 10) * 1000 + (number / 1000) * 100 + ((number / 100) % 10) * 10 + ((number % 100) / 10);
        Vertex
            second = ((number / 100) % 10) * 1000 + ((number % 100) / 10) * 100 + (number % 10) * 10 + (number / 1000);
        std::vector<Vertex> moves = {number + 1000, number - 1, first, second};
        for (auto& move: moves) {
            if ((move < 10000) && ((move) % 10) && (move >= 1111)) {
                ++num_of_edges;
                list_of_adjacency[number].emplace(move);
                if (!IsDirected && (move - number != 1000) && (number - move != 1)) {
                    list_of_adjacency[move].emplace(number);
                }
                if (!used_cells[move]) {
                    ++num_of_verteses;
                    Add_Edge(move);
                }
            }
        }
    }
    ~AdjList() = default;

};
std::vector<Graph::Vertex> Answer_And_Making_Path(std::unordered_map<Graph::Vertex, Graph::Vertex>& parent,
                                                  std::unordered_map<Graph::Vertex, int64_t>& distance,
                                                  Graph::Vertex& to) {
    if (distance[to] == 0) {
        return {};
    } else {
        std::cout << distance[to] + 1 << std::endl;
        std::vector<Graph::Vertex> answer(1, to);
        Graph::Vertex vertex = parent[to];
        Graph::Vertex zero = 0;
        while (vertex != zero) {
            answer.push_back(vertex);
            vertex = parent[vertex];
        }
        return answer;
    }
}
std::vector<Graph::Vertex> Shortest_Path_Between_Verteses(const Graph& graph, Graph::Vertex from, Graph::Vertex to) {
    std::unordered_map<Graph::Vertex, int64_t> distance(graph.Get_Num_Of_Verteses() + 1);
    std::unordered_map<Graph::Vertex, Graph::Vertex> parent(graph.Get_Num_Of_Verteses() + 1);
    std::queue<Graph::Vertex> queue;
    std::unordered_map<Graph::Vertex, bool> used(graph.Get_Num_Of_Verteses() + 1);
    queue.push(from);
    used[from] = true;
    distance[from] = 0;
    parent[from] = 0;
    while (!queue.empty()) {
        Graph::Vertex v = queue.front();
        queue.pop();
        for (auto u: graph.Get_Neighbors(v)) {
            if (!used[u]) {
                used[u] = true;
                if (distance[u] == 0) {
                    distance[u] = distance[v] + 1;
                }
                parent[u] = v;
                queue.push(u);
            }
        }
    }
    return Answer_And_Making_Path(parent, distance, to);
}

int main() {
    AdjList graph;
    int32_t first_number;
    std::cin >> first_number;
    int32_t second_number;
    std::cin >> second_number;
    if (first_number == second_number) {
        std::cout << "1" << std::endl;
        std::cout << first_number;
        return 0;
    }
    graph.Add_Edge(first_number);
    std::vector<Graph::Vertex> answer = Shortest_Path_Between_Verteses(graph, first_number, second_number);
    if (answer.empty()) {
        std::cout << "-1";
    } else {
        for (size_t i = answer.size() - 1; i > 0; --i) {
            std::cout << answer[i] << std::endl;
        }
        std::cout << answer[0];
    }
    return 0;
}

