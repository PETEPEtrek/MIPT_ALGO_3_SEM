#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
#include <queue>

enum { INF = 2147483647 };
struct Point {
    int32_t x = 0;
    int32_t y = 0;
    int32_t Num_of_free_high = INF;
    int32_t Num_of_free_down = INF;
    int32_t Num_of_free_left = INF;
    int32_t Num_of_free_right = INF;
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
    mutable std::unordered_map<Point, int64_t> distant;
    Vertex Get_Num_Of_Verteses() const {
        return num_of_verteses;
    }
    int32_t Get_size_of_desk_in_x() const {
        return Desk_size_x;
    }
    int32_t Get_size_of_desk_in_y() const {
        return Desk_size_y;
    }
    virtual void Build_Graph(Point& a, std::vector<std::vector<char>>& city, std::vector<Point>& walls) = 0;
    virtual std::set<Point> Get_Neighbors(Point v) const = 0;
protected:
    bool IsDirected = true;
    Vertex num_of_edges = 0;
    Vertex num_of_verteses = 0;
    int32_t Desk_size_x = 0;
    int32_t Desk_size_y = 0;
    std::unordered_map<Point, bool> used_cells;
};
class AdjList : public Graph {
    std::unordered_map<Point, std::set<Point>> list_of_adjacency;

    std::set<Point> Get_Neighbors(Point v) const override {
        if (!list_of_adjacency.count(v)) {
            std::set<Point> zero{};
            return zero;
        }
        return list_of_adjacency.at(v);
    }
public:
    AdjList(int32_t desk_size_x, int32_t desk_size_y) {
        num_of_verteses = 1;
        num_of_edges = 0;
        Desk_size_x = desk_size_x;
        Desk_size_y = desk_size_y;
        for (int32_t x = 1; x <= Desk_size_x; ++x) {
            for (int32_t y = 1; y <= Desk_size_y; ++y) {
                Point position(x, y);
                distant[position] = INF;
            }
        }
    }
    void InitializePoint(Point& a, std::vector<Point>& walls) {
        a.Num_of_free_right = Desk_size_y - a.y;
        a.Num_of_free_left = a.y - 1;
        a.Num_of_free_high = a.x - 1;
        a.Num_of_free_down = Desk_size_x - a.x;
        for (const auto& wall: walls) {
            if (wall.x == a.x) {
                if (wall.y - a.y > 0) {
                    a.Num_of_free_right = std::min(a.Num_of_free_right, wall.y - a.y - 1);
                } else if (wall.y - a.y < 0) {
                    a.Num_of_free_left = std::min(a.Num_of_free_left, a.y - wall.y - 1);
                }
            } else if (wall.y == a.y) {
                if (wall.x - a.x > 0) {
                    a.Num_of_free_down = std::min(a.Num_of_free_down, wall.x - a.x - 1);
                } else if (wall.x - a.x < 0) {
                    a.Num_of_free_high = std::min(a.Num_of_free_high, a.x - wall.x - 1);
                }
            }
        }
    }
    void Build_Graph(Point& a, std::vector<std::vector<char>>& city, std::vector<Point>& walls) override {
        used_cells[a] = true;
        InitializePoint(a, walls);
        std::vector<Point> moves =
            {{(a.Num_of_free_down + 1) / 2, 0}, {0, (a.Num_of_free_right + 1) / 2}, {-(a.Num_of_free_high + 1) / 2, 0},
             {0, -(a.Num_of_free_left + 1) / 2}};
        for (auto& move: moves) {
            if ((a + move).x >= 1 && (a + move).y >= 1 && (a + move).x <= Desk_size_x && (a + move).y <= Desk_size_y
                && city[(a + move).x][(a + move).y] != '#') {
                Point b = a + move;
                InitializePoint(b, walls);
                if (!(a.x == b.x && a.y == b.y)) {
                    ++num_of_edges;
                    list_of_adjacency[a].emplace(b);
                    if (!IsDirected) {
                        list_of_adjacency[b].emplace(a);
                    }
                    if (!used_cells[b]) {
                        ++num_of_verteses;
                        Build_Graph(b, city, walls);
                    }
                }
            }
        }
    }
    ~AdjList() = default;

};
void Answer(std::unordered_map<Point, int64_t>& distance, const Point& to) {
    if (distance[to] == INF) {
        std::cout << "-1";
    } else {
        std::cout << distance[to];
    }
}
void Shortest_Path_Between_Verteses(const Graph& graph, const Point& from, const Point& to) {
    std::unordered_map<Point, int64_t> distance(graph.Get_Num_Of_Verteses() + 1);
    std::unordered_map<Point, Point> parent(graph.Get_Num_Of_Verteses() + 1);
    for (int32_t x = 1; x <= graph.Get_size_of_desk_in_x(); ++x) {
        for (int32_t y = 1; y <= graph.Get_size_of_desk_in_y(); ++y) {
            Point position(x, y);
            distance[position] = INF;
        }
    }
    std::queue<Point> queue;
    std::unordered_map<Point, bool> used(graph.Get_Num_Of_Verteses() + 1);
    used[from] = true;
    distance[from] = 0;
    parent[from] = {0, 0};
    queue.push(from);
    while (!queue.empty()) {
        Point v = queue.front();
        queue.pop();
        for (auto u: graph.Get_Neighbors(v)) {
            if (!used[u]) {
                used[u] = true;
                distance[u] = distance[v] + 1;
                graph.distant[u] = std::min(distance[u], graph.distant[u]);
                queue.push(u);
            }
        }
    }
    Answer(distance, to);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int32_t Size_of_desk_x, Size_of_desk_y;
    std::cin >> Size_of_desk_x >> Size_of_desk_y;
    AdjList graph(Size_of_desk_x, Size_of_desk_y);
    Point start;
    Point end;
    std::vector<Point> walls;
    std::vector<std::vector<char>> city{};
    std::vector<Point> subways;
    city.resize(Size_of_desk_x + 1);
    for (int32_t x = 1; x <= Size_of_desk_x; ++x) {
        city[x].resize(Size_of_desk_y + 1, 0);
    }
    for (int32_t x = 1; x <= Size_of_desk_x; ++x) {
        for (int32_t y = 1; y <= Size_of_desk_y; ++y) {
            char place;
            std::cin >> place;
            if (place == 'S') {
                start.x = x;
                start.y = y;
            } else if (place == 'T') {
                end.x = x;
                end.y = y;
            } else if (place == '#') {
                Point wall(x, y);
                walls.push_back(wall);
            }
            city[x][y] = place;
            if (place) {
                Point sub(x, y);
                subways.push_back(sub);
            }
        }
    }
    graph.Build_Graph(start, city, walls);
    Shortest_Path_Between_Verteses(graph, start, end);
    return 0;
}
