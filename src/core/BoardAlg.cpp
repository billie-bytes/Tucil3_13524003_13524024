#include "../../include/core/BoardAlg.hpp"
#include <algorithm>
#include <queue>


/**
 * Feel free untuk bikin fungsi helper sebanyak yang kamu mau.
 * But pastiin dibuat sebagai static function buat good practice.
 * Example: static void helperFunc(){...}
 */

static std::vector<Neighbor> expand(const Board& board, const SearchNode& current, double currentCost) {
    std::vector<Neighbor> neighbors;

    int** matrix = board.getMatrix();

    std::vector<Direction> directions = {
        Direction::RIGHT,
        Direction::UP,
        Direction::LEFT,
        Direction::DOWN
    };

    for (Direction d : directions) {

        int nextX = current.x;
        int nextY = current.y;
        int nextOrd = current.ord;

        double slideCost = 0;

        while (true) {

            int checkX = nextX;
            int checkY = nextY;

            switch (d) {
                case Direction::RIGHT: checkX++; break;
                case Direction::UP: checkY--; break;
                case Direction::LEFT: checkX--; break;
                case Direction::DOWN: checkY++; break;
            }

            if (matrix[checkX][checkY] == WALL) {
                break;
            }

            nextX = checkX;
            nextY = checkY;

            if (matrix[nextX][nextY] == LAVA) {
                slideCost = -1;
                break;
            }

            auto x_it = board.orderedTiles.find(nextX);
            if (x_it != board.orderedTiles.end()) {
                auto y_it = x_it->second.find(nextY);
                if (y_it != x_it->second.end()) {
                    int order = y_it->second;
                    
                    if (order == nextOrd) {
                        nextOrd++;
                    }
                    else if (order > nextOrd) {
                        slideCost = -1.0;
                    }
                }
            }

            if (slideCost == -1.0) {
                break;
            }

            slideCost += matrix[nextX][nextY];
        }

        if (slideCost == -1.0) { // died
            continue;
        }

        if (nextX == current.x && nextY == current.y) { // not moved
            continue;
        }

        neighbors.push_back({SearchNode(nextX, nextY, nextOrd), currentCost + slideCost, 0.0, d}); 
    }

    return neighbors;
}

std::vector<Direction> reconstructPath(std::unordered_map<SearchNode, std::pair<SearchNode, Direction>, SearchNodeHash>& parent, const SearchNode& start, const SearchNode& goal) {
    std::vector<Direction> path;

    SearchNode current = goal;

    while (!(current == start)) {

        auto [prev, dir] = parent[current];

        path.push_back(dir);

        current = prev;
    }

    std::reverse(path.begin(), path.end());

    return path;
}


std::pair<int, std::vector<Direction>> UCS(const Board& board, int heuristic){
    SearchNode start(board.pinX, board.pinY, board.ord);
    SearchNode goal(board.winX, board.winY, 0);
    std::priority_queue<Neighbor, std::vector<Neighbor>, std::greater<Neighbor>> pq;
    std::unordered_map<SearchNode, double, SearchNodeHash> dist;
    std::unordered_map<SearchNode, std::pair<SearchNode, Direction>, SearchNodeHash> parent;


    pq.push({start, 0.0, 0.0, Direction::RIGHT});

    dist[start] = 0.0;

    while (!pq.empty()) {

        Neighbor current = pq.top();
        pq.pop();

        SearchNode currentNode = current.node;
        int currentCost = current.totalCost;

        if (currentNode == goal) {
            return {currentCost, reconstructPath(parent, start, currentNode)};
        }

        auto expanded = expand(board, currentNode, currentCost);

        for (const Neighbor& next : expanded) {
            if (!dist.count(next.node) || next.totalCost < dist[next.node]) {
                dist[next.node] = next.totalCost;
                parent[next.node] = {currentNode, next.dir};
                pq.push({next.node, next.totalCost, next.totalCost, next.dir});
            }
        }
    }

    return {-1, {}};
}

std::pair<int, std::vector<Direction>> GBFS(const Board& board, int heuristic){
    SearchNode start(board.pinX, board.pinY, board.ord);
    SearchNode goal(board.winX, board.winY, 0);
    std::priority_queue<Neighbor, std::vector<Neighbor>, std::greater<Neighbor>> pq;
    std::unordered_map<SearchNode, double, SearchNodeHash> dist;
    std::unordered_map<SearchNode, std::pair<SearchNode, Direction>, SearchNodeHash> parent;

    // GBFS: Priority is h(n)
    pq.push({start, 0.0, heuristics(start, goal, heuristic), Direction::RIGHT});

    dist[start] = 0.0;

    while (!pq.empty()) {

        Neighbor current = pq.top();
        pq.pop();

        SearchNode currentNode = current.node;
        int currentCost = current.totalCost;

        if (currentNode == goal) {
            return {currentCost, reconstructPath(parent, start, currentNode)};
        }

        auto expanded = expand(board, currentNode, currentCost);

        for (const Neighbor& next : expanded) {
            double estimatedCostOnward = heuristics(next.node, goal, heuristic);
            if (!dist.count(next.node) || estimatedCostOnward < dist[next.node]) {
                dist[next.node] = estimatedCostOnward;
                parent[next.node] = {currentNode, next.dir};
                pq.push({next.node, next.totalCost, estimatedCostOnward, next.dir});
            }
        }
    }

    return {-1, {}};
}

std::pair<int, std::vector<Direction>> ASTAR(const Board& board, int heuristic){
    SearchNode start(board.pinX, board.pinY, board.ord);
    SearchNode goal(board.winX, board.winY, 0);
    std::priority_queue<Neighbor, std::vector<Neighbor>, std::greater<Neighbor>> pq;
    std::unordered_map<SearchNode, double, SearchNodeHash> dist;
    std::unordered_map<SearchNode, std::pair<SearchNode, Direction>, SearchNodeHash> parent;

    // A*: Priority is f(n) = g(n) + h(n)
    pq.push({start, 0.0, heuristics(start, goal, heuristic), Direction::RIGHT});

    dist[start] = 0.0;

    while (!pq.empty()) {

        Neighbor current = pq.top();
        pq.pop();

        SearchNode currentNode = current.node;
        int currentCost = current.totalCost;

        if (currentNode == goal) {
            return {currentCost, reconstructPath(parent, start, currentNode)};
        }

        auto expanded = expand(board, currentNode, currentCost);

        for (const Neighbor& next : expanded) {
            double estimatedCostOnward = heuristics(next.node, goal, heuristic);
            if (!dist.count(next.node) || next.totalCost + estimatedCostOnward < dist[next.node]) {
                dist[next.node] = next.totalCost + estimatedCostOnward;
                parent[next.node] = {currentNode, next.dir};
                pq.push({next.node, next.totalCost, next.totalCost + estimatedCostOnward, next.dir});
            }
        }
    }

    return {-1, {}};
}

double heuristics(SearchNode node, SearchNode goal, int choice) {
    switch (choice) {
    case 1:
        return manhattanDistance(node, goal);

    case 2:
        return euclideanDistance(node, goal);
    
    default:
        return 0;
    }
}

double manhattanDistance(SearchNode node, SearchNode goal) {
    return double(std::abs(node.x - goal.x) + std::abs(node.y - goal.y));
}

double euclideanDistance(SearchNode node, SearchNode goal) {
    return std::sqrt(std::pow(node.x - goal.x, 2) + std::pow(node.y - goal.y, 2));
}