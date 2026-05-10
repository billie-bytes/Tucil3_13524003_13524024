#include "../../include/core/BoardAlg.hpp"
#include <algorithm>
#include <queue>
#include <unordered_set>
#include <utility>
/**
 * Feel free untuk bikin fungsi helper sebanyak yang kamu mau.
 * But pastiin dibuat sebagai static function buat good practice.
 * Example: static void helperFunc(){...}
 */

static int getOrder(const Board& b, int x, int y){
    auto x_it = b.orderedTiles.find(x);
    if (x_it != b.orderedTiles.end()) {
        auto y_it = x_it->second.find(y);
        if (y_it != x_it->second.end()) {
            if(y_it->second!=-1){
                return y_it->second;
            }
        }
    }
    return -1;
}

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

            if(checkX < 0 || checkX >= board.panjang || checkY < 0 || checkY >= board.lebar) {
                slideCost = -1;
                break;
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

            int order = board.orderedTiles.at(nextX).at(nextY);

            if (order == nextOrd) {
                nextOrd++;
            }
            else if (order > nextOrd) {
                slideCost = -1.0;
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


std::pair<std::pair<int, std::vector<Direction>>, std::pair<int, std::vector<Direction>>> UCS(const Board& board){
    SearchNode start(board.pinX, board.pinY, board.ord);
    SearchNode goal(board.winX, board.winY, board.highestOrd+1);
    std::priority_queue<Neighbor, std::vector<Neighbor>, std::greater<Neighbor>> pq;
    std::unordered_map<SearchNode, double, SearchNodeHash> dist;
    std::unordered_map<SearchNode, std::pair<SearchNode, Direction>, SearchNodeHash> parent;


    pq.push({start, 0.0, 0.0, Direction::RIGHT});

    dist[start] = 0.0;

    int countIterate = 0;

    while (!pq.empty()) {

        Neighbor current = pq.top();
        pq.pop();
        countIterate++;

        SearchNode currentNode = current.node;
        int currentCost = current.totalCost;

        if (currentNode == goal) {
            return {{currentCost, reconstructPath(parent, start, currentNode)}, {countIterate, {}}};
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

    return {{-1, {}}, {-1, {}}};
}

std::pair<std::pair<int, std::vector<Direction>>, std::pair<int, std::vector<Direction>>> GBFS(const Board& board, int heuristic){
    SearchNode start(board.pinX, board.pinY, board.ord);
    SearchNode goal(board.winX, board.winY, board.highestOrd+1);
    std::priority_queue<Neighbor, std::vector<Neighbor>, std::greater<Neighbor>> pq;
    std::unordered_map<SearchNode, double, SearchNodeHash> dist;
    std::unordered_map<SearchNode, std::pair<SearchNode, Direction>, SearchNodeHash> parent;

    // GBFS: Priority is h(n)
    pq.push({start, 0.0, heuristics(start, goal, heuristic), Direction::RIGHT});

    dist[start] = 0.0;

    int countIterate = 0;

    while (!pq.empty()) {

        Neighbor current = pq.top();
        pq.pop();
        countIterate++;

        SearchNode currentNode = current.node;
        int currentCost = current.totalCost;

        if (currentNode == goal) {
            return {{currentCost, reconstructPath(parent, start, currentNode)}, {countIterate, {}}};
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

    return {{-1, {}}, {-1, {}}};
}

std::pair<std::pair<int, std::vector<Direction>>, std::pair<int, std::vector<Direction>>> ASTAR(const Board& board, int heuristic){
    SearchNode start(board.pinX, board.pinY, board.ord);
    SearchNode goal(board.winX, board.winY, board.highestOrd+1);
    std::priority_queue<Neighbor, std::vector<Neighbor>, std::greater<Neighbor>> pq;
    std::unordered_map<SearchNode, double, SearchNodeHash> dist;
    std::unordered_map<SearchNode, std::pair<SearchNode, Direction>, SearchNodeHash> parent;

    // A*: Priority is f(n) = g(n) + h(n)
    pq.push({start, 0.0, heuristics(start, goal, heuristic), Direction::RIGHT});

    dist[start] = 0.0;

    int countIterate = 0;

    while (!pq.empty()) {

        Neighbor current = pq.top();
        pq.pop();
        countIterate++;

        SearchNode currentNode = current.node;
        int currentCost = current.totalCost;

        if (currentNode == goal) {
            return {{currentCost, reconstructPath(parent, start, currentNode)}, {countIterate, {}}};
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

    return {{-1, {}}, {-1, {}}};
}

std::pair<std::pair<int, std::vector<Direction>>, std::pair<int, std::vector<Direction>>> BFS(const Board& board){
    /*
    This is just.. BFS I guess. But an interesting result is that it
    optimizes the number of moves, not the total cost
    */
    SearchNode start(board.pinX, board.pinY, board.ord);
    SearchNode goal(board.winX, board.winY, board.highestOrd+1);
    std::queue<Neighbor> q;
    std::unordered_set<SearchNode, SearchNodeHash> visited;
    std::unordered_map<SearchNode, std::pair<SearchNode, Direction>, SearchNodeHash> parent;
    q.push({start, 0.0, 0.0, Direction::RIGHT});
    visited.insert(start);

    int countIterate = 0;

    while (!q.empty()) {
        
        Neighbor current = q.front();
        q.pop();
        countIterate++;

        SearchNode currentNode = current.node;
        int currentCost = current.totalCost;
        if (currentNode == goal) {
            return {{currentCost, reconstructPath(parent, start, currentNode)}, {countIterate, {}}};
        }

        auto expanded = expand(board, currentNode, currentCost);
        for (const Neighbor& next : expanded) {
            
            if (visited.find(next.node) == visited.end()) {
                visited.insert(next.node);
                parent[next.node] = {currentNode, next.dir};
                q.push(next);
            }
        }
    }

    return {{-1, {}}, {-1, {}}};
}

std::pair<std::pair<int, std::vector<Direction>>, std::pair<int, std::vector<Direction>>> BeamSearch(const Board& board, int heuristic, size_t k = 50){
    SearchNode start(board.pinX, board.pinY, board.ord);
    SearchNode goal(board.winX, board.winY, board.highestOrd+1);
    
    /*
    Cuz this algorithm is basically A* but limits the nodes considered into a specified
    length, it uses a lot of discarding memories from vectors. Having one vector and needing
    to shift all the object to the right from one pop is just too slow. So instead we just
    push all the valid objects (in this case, the top k nodes) into a new vector "nextLevel"
    and just discard the previous vector (effectively throwing it to the std::vector dtor)
    */
    std::vector<Neighbor> currentLevel;
    std::unordered_map<SearchNode, double, SearchNodeHash> dist;
    std::unordered_map<SearchNode, std::pair<SearchNode, Direction>, SearchNodeHash> parent;

    double startHeuristic = heuristics(start, goal, heuristic);
    currentLevel.push_back({start, 0.0, startHeuristic, Direction::RIGHT});
    dist[start] = 0.0;

    int countIterate = 0;

    while (!currentLevel.empty()) {
        countIterate++;
        std::vector<Neighbor> nextLevel;
        for (const Neighbor& current : currentLevel) {
            SearchNode currentNode = current.node;
            int currentCost = current.totalCost;
            if (currentNode == goal) {
                return {{currentCost, reconstructPath(parent, start, currentNode)}, {countIterate, {}}};
            }

            auto expanded = expand(board, currentNode, currentCost);

            for (const Neighbor& next : expanded) {
                double estimatedCostOnward = heuristics(next.node, goal, heuristic);
                double f_n = next.totalCost + estimatedCostOnward;

                if (!dist.count(next.node) || f_n < dist[next.node]) {
                    
                    dist[next.node] = f_n;
                    parent[next.node] = {currentNode, next.dir};

                    nextLevel.push_back({next.node, next.totalCost, f_n, next.dir});
                }
            }
        }

        /*
        This function is like quicksort where it picks a pivot and sorts
        everything less than it to the left and more than it to the right
        (the partinioning process). But it doesnt sort all the way, it just
        finds the nth element. But in the process of finding the nth element
        it also made it that every element to the left of the nth element
        become the top n elements. So we use this here.
        */
        if (nextLevel.size() > k) {
            std::nth_element(
                nextLevel.begin(),
                nextLevel.begin() + k,
                nextLevel.end(),
                [](const Neighbor& a, const Neighbor& b) {
                    return a.priority < b.priority; 
                }
            );
            nextLevel.resize(k);
        }
        currentLevel = std::move(nextLevel);
    }
    return {{-1, {}}, {-1, {}}};
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