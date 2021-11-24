#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

enum { ENDOFPARENT = -1, INF = INT64_MAX };

struct Point {
    int64_t x = 0;
    int64_t y = 0;
};

struct Move {
    std::vector<Point> moves = {{-2, 1}, {-2, -1}, {1, 2}, {-1, 2}};
};

class Graph {
public:
    using Vertex = int64_t;
    virtual std::vector<Vertex> Get_Neighbors(Vertex vertex) const = 0;
    Vertex Get_Num_Of_Verteses() const {
        return num_of_verteses;
    }
    virtual void AddEdge(const Vertex& first_vertex, const Vertex& second_vertex) = 0;
protected:
    bool IsDirected = false;
    int32_t num_of_edges = 0;
    Vertex num_of_verteses = 0;
};
class AdjList : public Graph {
    std::vector<std::vector<Vertex>> list_of_adjacency;

    std::vector<Vertex> Get_Neighbors(Vertex vertex) const override {
        return list_of_adjacency[vertex];
    }
public:
    AdjList(int64_t desk_size) {
        num_of_verteses = desk_size;
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
class Grid {
    int64_t size_of_desk;

public:
    explicit Grid(int64_t size) {
        size_of_desk = size;
    }
    int64_t GetDeskSize() {
        return size_of_desk;
    }
    int64_t DeskToArr(int64_t pos_x, int64_t pos_y) const {
        return pos_x * size_of_desk + pos_y;
    }
    bool Is_In_Desk(int64_t pos_x, int64_t pos_y) const {
        if (pos_y >= 0 && pos_y < size_of_desk && pos_x >= 0 && pos_x < size_of_desk) {
            return true;
        }
        return false;
    }
};

void BuildGraph(Graph& graph, Grid& grid) {
    Move move;
    for (Graph::Vertex pos_x = 0; pos_x < grid.GetDeskSize(); ++pos_x) {
        for (Graph::Vertex pos_y = 0; pos_y < grid.GetDeskSize(); ++pos_y) {
            for (const auto& option_move: move.moves) {
                if (grid.Is_In_Desk(pos_x + option_move.x, pos_y + option_move.y)) {
                    Graph::Vertex first_vertex = grid.DeskToArr(pos_x, pos_y);
                    Graph::Vertex second_vertex = grid.DeskToArr(pos_x + option_move.x, pos_y + option_move.y);
                    graph.AddEdge(first_vertex, second_vertex);
                }
            }
        }
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

void PrintShortestPath(const std::vector<Graph::Vertex>& answer, Graph::Vertex size_of_desk) {
    if (answer.empty()) {
        std::cout << -1 << std::endl;
        return;
    }
    std::cout << answer.size() - 1 << std::endl;
    for (auto& vertex: answer) {
        Graph::Vertex pos_x = vertex / size_of_desk;
        Graph::Vertex pos_y = vertex % size_of_desk;
        std::cout << pos_x + 1 << " " << pos_y + 1 << std::endl;
    }
}
int main() {
    int64_t desk_size;
    std::cin >> desk_size;
    AdjList graph(desk_size * desk_size);
    Grid grid(desk_size);
    BuildGraph(graph, grid);
    Point from, to;
    std::cin >> from.x >> from.y;
    std::cin >> to.x >> to.y;
    Graph::Vertex from_pos = grid.DeskToArr(from.x - 1, from.y - 1);
    Graph::Vertex to_pos = grid.DeskToArr(to.x - 1, to.y - 1);
    std::vector<Graph::Vertex> answer = ShorthestPathForHorse(graph, from_pos, to_pos);
    PrintShortestPath(answer, desk_size);
    return 0;
}

