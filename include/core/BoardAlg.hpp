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
        return x == rhs.x && y == rhs.y;
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
    Direction dir;

    bool operator>(const Neighbor& rhs) const {
        return totalCost > rhs.totalCost;
    }
};

/**
 * Misalkan UCS dikasih UCS(b,2) dan UCS gapunya heuristic 2, return aja
 * {} (melambangkan vector kosong) terus outputin
 * std::cout << We don't have that heuristic for this algorithm, bucko << std::endl; 
 */
std::pair<int, std::vector<Direction>> UCS(Board b, int heuristic);
std::vector<Direction> GBFS(Board b, int heuristic);
std::vector<Direction> ASTAR(Board b, int heuristic);

double heuristics(SearchNode node, SearchNode goal, int choice);
double manhattanDistance(SearchNode node, SearchNode goal);
double euclideanDistance(SearchNode node, SearchNode goal);