#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

enum { INF = 2147483647 };
struct Point {
    int32_t x = 0;
    int32_t y = 0;
    int32_t level_of_water = INF;
    Point() {};
    Point(int64_t x, int64_t y) : x(x), y(y) {}
    Point(int64_t x, int64_t y, int64_t level_of_water) : x(x), y(y), level_of_water(level_of_water) {}
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
    Vertex Get_Num_Of_Verteses() const {
        return num_of_verteses;
    }
    int32_t Get_size_of_desk_in_x() const {
        return Desk_size_x;
    }
    int32_t Get_size_of_desk_in_y() const {
        return Desk_size_y;
    }
    virtual void Build_Graph(Point& vertex, std::vector<std::vector<int32_t>>& territory) = 0;
    virtual void Build_Trans_Graph(Point& vertex, std::vector<std::vector<int32_t>>& territory) = 0;
    virtual std::unordered_set<Point> Get_Neighbors(Point vertex) const = 0;
    virtual bool is_connection(Point first_vertex, Point second_vertex) = 0;
protected:
    bool IsDirected = true;
    int32_t num_of_edges = 0;
    Vertex num_of_verteses = 1;
    int32_t Desk_size_x = 0;
    int32_t Desk_size_y = 0;
    std::unordered_map<Point, bool> used_cells;
};
class AdjList : public Graph {
    std::unordered_map<Point, std::unordered_set<Point>> list_of_adjacency;

    std::unordered_set<Point> Get_Neighbors(Point v) const override {
        if (!list_of_adjacency.count(v)) {
            return {};
        }
        return list_of_adjacency.at(v);
    }
public:
    AdjList(int32_t desk_size_x, int32_t desk_size_y) {
        num_of_verteses = 1;
        num_of_edges = 0;
        Desk_size_x = desk_size_x;
        Desk_size_y = desk_size_y;
    }
    void Build_Graph(Point& vertex, std::vector<std::vector<int32_t>>& territory) override {
        used_cells[vertex] = true;
        vertex.level_of_water = territory[vertex.x][vertex.y];
        std::vector<Point> moves = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
        for (auto& move: moves) {
            if ((vertex + move).x >= 1 && (vertex + move).y >= 1 && (vertex + move).x <= Desk_size_x && (vertex + move).y <= Desk_size_y) {
                Point second_vertex = vertex + move;
                second_vertex.level_of_water = territory[second_vertex.x][second_vertex.y];
                if (!IsDirected || (second_vertex.level_of_water <= vertex.level_of_water)) {
                    ++num_of_edges;
                    list_of_adjacency[vertex].emplace(second_vertex);
                }
                if (!IsDirected || second_vertex.level_of_water >= vertex.level_of_water) {
                    ++num_of_edges;
                    list_of_adjacency[second_vertex].emplace(vertex);
                }
                if (!used_cells[second_vertex]) {
                    ++num_of_verteses;
                    Build_Graph(second_vertex, territory);
                }
            }
        }
    }
    void Build_Trans_Graph(Point& vertex, std::vector<std::vector<int32_t>>& territory) override {
        used_cells[vertex] = true;
        vertex.level_of_water = territory[vertex.x][vertex.y];
        std::vector<Point> moves = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
        for (auto& move: moves) {
            if ((vertex + move).x >= 1 && (vertex + move).y >= 1 && (vertex + move).x <= Desk_size_x && (vertex + move).y <= Desk_size_y) {
                Point second_vertex = vertex + move;
                second_vertex.level_of_water = territory[second_vertex.x][second_vertex.y];
                if (!IsDirected || (second_vertex.level_of_water >= vertex.level_of_water)) {
                    ++num_of_edges;
                    list_of_adjacency[vertex].emplace(second_vertex);
                }
                if (!IsDirected || second_vertex.level_of_water <= vertex.level_of_water) {
                    ++num_of_edges;
                    list_of_adjacency[second_vertex].emplace(vertex);
                }
                if (!used_cells[second_vertex]) {
                    ++num_of_verteses;
                    Build_Trans_Graph(second_vertex, territory);
                }
            }
        }
    }
    ~AdjList() = default;
    bool is_connection(Point first_vertex, Point second_vertex) override {
        return list_of_adjacency[first_vertex].count(second_vertex);
    }
};

void dfs_visit_for_graph(Graph& graph, std::unordered_map<Point, bool>& used, Point& v, std::vector<Point>& special_order) {
    used[v] = true;
    for (auto u: graph.Get_Neighbors(v)) {
        if (!used[u]) {
            dfs_visit_for_graph(graph, used, u, special_order);
        }
    }
    special_order.push_back(v);
}
void dfs_visit_for_trans_graph(Graph& trans_graph,
                               std::unordered_map<Point, bool>& used,
                               Point& v,
                               std::vector<Point>& csc) {
    used[v] = true;
    csc.push_back(v);
    for (auto u: trans_graph.Get_Neighbors(v)) {
        if (!used[u]) {
            dfs_visit_for_trans_graph(trans_graph, used, u, csc);
        }
    }
}
std::vector<std::vector<Point>> Cosarayu(Graph& graph, Graph& trans_graph, std::vector<std::vector<int32_t>>& territory) {
    std::vector<std::vector<Point>> answer;
    std::unordered_map<Point, bool> used;
    std::vector<Point> special_order;
    std::vector<Point> components;
    for (int64_t i = 1; i <= graph.Get_size_of_desk_in_x(); ++i) {
        for (int64_t j = 1; j <= graph.Get_size_of_desk_in_y(); ++j) {
            Point point{i, j, territory[i][j]};
            if (!used[point]) {
                dfs_visit_for_graph(graph, used, point, special_order);
            }
        }
    }
    used.clear();
    int64_t count = 1;
    for (int64_t i = 1; i <= graph.Get_size_of_desk_in_x(); ++i) {
        for (int64_t j = 1; j <= graph.Get_size_of_desk_in_y(); ++j) {
            Point point = special_order[graph.Get_Num_Of_Verteses() - count++];
            if (!used[point]) {
                dfs_visit_for_trans_graph(trans_graph, used, point, components);
                answer.push_back(components);
                components.clear();
            }
        }
    }
    return answer;
}
void Answer(Graph& graph, std::vector<std::vector<Point>>& answer) {
    int32_t n = 0;
    bool opt = false;
    for (int64_t i = 0; i < answer.size(); ++i) {
        for (int64_t j = i + 1; j < answer.size(); ++j) {
            for (const auto& item_first: answer[i]) {
                for (const auto& item_second: answer[j]) {
                    opt = graph.is_connection(item_first, item_second);
                    if (opt) {
                        break;
                    }
                }
                if (opt) {
                    break;
                }
            }
            if (opt) {
                break;
            }
        }
        if (!opt) {
            ++n;
        } else {
            opt = false;
        }
    }
    std::cout << n;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int32_t Size_of_desk_x, Size_of_desk_y;
    std::cin >> Size_of_desk_x >> Size_of_desk_y;
    AdjList graph(Size_of_desk_x, Size_of_desk_y);
    AdjList trans_graph(Size_of_desk_x, Size_of_desk_y);
    std::vector<std::vector<int32_t>> territory{};
    territory.resize(Size_of_desk_x + 1);
    for (int32_t x = 1; x <= Size_of_desk_x; ++x) {
        territory[x].resize(Size_of_desk_y + 1, 0);
    }
    for (int32_t x = 1; x <= Size_of_desk_x; ++x) {
        for (int32_t y = 1; y <= Size_of_desk_y; ++y) {
            int32_t place;
            std::cin >> place;
            territory[x][y] = place;
        }
    }
    Point start{1, 1};
    graph.Build_Graph(start, territory);
    trans_graph.Build_Trans_Graph(start, territory);
    std::vector<std::vector<Point>> answer = Cosarayu(graph, trans_graph, territory);
    Answer(graph, answer);
    return 0;
}
