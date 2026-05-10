/**
 * 
 * File ini kita implementasi the actual algorithm. Asumsikan
 * board udah diinisialisasikan saat menggunakan fungsi-fungsi di sini.
 * 
 * Untuk tiap fungsi di bawah, akan nerima 2 argumen yaitu board dan argumen.
 * Semua fungsi akan mereturn vector berisi enum Direction (didefinisikan di
 * Board.hpp).
 * 
 * Misal, given a board yang udah diinisialisasi, manggil BFS(board, h2) akan
 * mereturn vector yang isinya misal [LEFT, UP, RIGHT] yang artinya ini solusi
 * yang ditemukan.
 * 
 * 
 */
#pragma once
#include "Board.hpp"
#include <cmath>

class SearchNode {
public:
    int x;
    int y;
    int ord;

    SearchNode(int px = 0, int py = 0, int po = 0): x(px), y(py), ord(po) {}

    bool operator==(const SearchNode& rhs) const {
        if(ord == -1 || rhs.ord == -1) return x == rhs.x && y == rhs.y;
        return x == rhs.x && y == rhs.y && ord == rhs.ord;
    }
};

struct SearchNodeHash {
    size_t operator()(const SearchNode& n) const { // custom hash
        return std::hash<int>{}(n.x) ^ (std::hash<int>{}(n.y) << 1) ^ (std::hash<int>{}(n.ord) << 2);
    }
};

struct Neighbor {
    SearchNode node;
    double totalCost;
    double priority;
    Direction dir;

    bool operator>(const Neighbor& rhs) const {
        return priority> rhs.priority;
    }
};

std::pair<std::pair<int, std::vector<Direction>>, std::pair<int, std::vector<std::pair<int, std::vector<Direction>>>>> UCS(const Board& board, bool isOrdered);
std::pair<std::pair<int, std::vector<Direction>>, std::pair<int, std::vector<std::pair<int, std::vector<Direction>>>>> GBFS(const Board& board, int heuristic, bool isOrdered);
std::pair<std::pair<int, std::vector<Direction>>, std::pair<int, std::vector<std::pair<int, std::vector<Direction>>>>> ASTAR(const Board& board, int heuristic, bool isOrdered);
std::pair<std::pair<int, std::vector<Direction>>, std::pair<int, std::vector<std::pair<int, std::vector<Direction>>>>> BFS(const Board& board, bool isOrdered);
std::pair<std::pair<int, std::vector<Direction>>, std::pair<int, std::vector<std::pair<int, std::vector<Direction>>>>> BeamSearch(const Board& board, int heuristic, bool isOrdered, size_t k);
double heuristics(SearchNode node, SearchNode goal, int choice);
double manhattanDistance(SearchNode node, SearchNode goal);
double euclideanDistance(SearchNode node, SearchNode goal);