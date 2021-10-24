#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
#include <queue>
struct Point {
    int64_t x = 0;
    int64_t y = 0;
    Point() {};
    Point(int64_t x, int64_t y) : x(x), y(y) {}
    Point operator+(const Point& point) const {
        Point ans = *this;
        ans.x += point.x;
        ans.y += point.y;
        return ans;
    }
    bool operator<(const Point& point) const {
        return (x < point.x) || (point.x >= x && (y < point.y));
    }
    bool operator==(const Point& point) const {
        return ((x == point.x) && (y == point.y));
    }
    bool operator!=(const Point& point) const {
        return !(*this == point);
    }
};
namespace std {
template <>
struct hash<Point> {
    std::size_t operator()(const Point& point) const {
        using std::size_t;
        using std::hash;
        return ((hash<int64_t>()(point.x) ^ (hash<int64_t>()(point.y) << 1)) >> 1);
    }
};
}
class Graph {
public:
    typedef int64_t Vertex;
    std::unordered_map<Point, bool> used_cells;
    virtual void Build_Graph(Point first_vertex) = 0;
    virtual std::set<Point> Get_Neighbors(Point vertex) const = 0;
    Vertex Get_Num_Of_Verteses() const {
        return num_of_verteses;
    }
protected:
    bool IsDirected = false;
    int32_t num_of_edges = 0;
    Vertex num_of_verteses = 0;
    int32_t Desk_size = 0;
};
class AdjList : public Graph {
    std::unordered_map<Point, std::set<Point>> list_of_adjacency;

    std::set<Point> Get_Neighbors(Point vertex) const override {
        return list_of_adjacency.at(vertex);
    }
public:
    AdjList(int32_t desk_size) {
        num_of_verteses = 1;
        num_of_edges = 0;
        Desk_size = desk_size;
    }
    void Build_Graph(Point first_vertex) override {
        used_cells[first_vertex] = true;
        std::vector<Point> moves = {{-2, -1}, {-1, -2}, {-1, 2}, {-2, 1},
                                    {1, -2}, {2, -1}, {1, 2}, {2, 1}};
        for (auto& move: moves) {
            if ((first_vertex + move).x >= 1 && (first_vertex + move).y >= 1 && (first_vertex + move).x <= Desk_size && (first_vertex + move).y <= Desk_size) {
                ++num_of_edges;
                list_of_adjacency[first_vertex].emplace(first_vertex + move);
                if (!IsDirected) {
                    list_of_adjacency[first_vertex + move].emplace(first_vertex);
                }
                if (!used_cells[first_vertex + move]) {
                    ++num_of_verteses;
                    Build_Graph(first_vertex + move);
                }
            }
        }
    }
    ~AdjList() = default;

};
std::vector<Point> Answer_And_Making_Path(std::unordered_map<Point, Point>& parent,
                                          std::unordered_map<Point, int64_t>& distance,
                                          Point& to) {
    if (distance[to] == 0) {
        return {};
    } else {
        std::cout << distance[to] << std::endl;
        std::vector<Point> answer(1, to);
        Point vertex = parent[to];
        Point zero{0, 0};
        while (vertex != zero) {
            answer.push_back(vertex);
            vertex = parent[vertex];
        }
        return answer;
    }
}
std::vector<Point> Shortest_Path_Between_Verteses(const Graph& graph, Point& from, Point& to) {
    std::unordered_map<Point, int64_t> distance(graph.Get_Num_Of_Verteses() + 1);
    std::unordered_map<Point, Point> parent(graph.Get_Num_Of_Verteses() + 1);
    std::queue<Point> queue;
    std::unordered_map<Point, bool> used(graph.Get_Num_Of_Verteses() + 1);
    queue.push(from);
    used[from] = true;
    distance[from] = 0;
    parent[from] = {0, 0};
    while (!queue.empty()) {
        Point v = queue.front();
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
    int32_t Size_of_desk;
    std::cin >> Size_of_desk;
    int64_t first_point_x, first_point_y;
    std::cin >> first_point_x >> first_point_y;
    Point first(first_point_x, first_point_y);
    int64_t second_point_x, second_point_y;
    std::cin >> second_point_x >> second_point_y;
    Point second(second_point_x, second_point_y);
    if (first == second) {
        std::cout << "0" << std::endl;
        std::cout << first.x << " " << first.y;
        return 0;
    }
    AdjList graph(Size_of_desk);
    graph.Build_Graph(first);
    std::vector<Point> answer = Shortest_Path_Between_Verteses(graph, first, second);
    if (answer.empty()) {
        std::cout << "-1";
    } else {
        for (size_t i = answer.size() - 1; i > 0; --i) {
            std::cout << answer[i].x << " " << answer[i].y << std::endl;
        }
        std::cout << answer[0].x << " " << answer[0].y;
    }
    return 0;
}

