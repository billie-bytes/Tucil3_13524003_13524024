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


/**
 * Misalkan UCS dikasih UCS(b,2) dan UCS gapunya heuristic 2, return aja
 * {} (melambangkan vector kosong) terus outputin
 * std::cout << We don't have that heuristic for this algorithm, bucko << std::endl; 
 */
std::vector<Direction> UCS(Board b, int heuristic);
std::vector<Direction> GBFS(Board b, int heuristic);
std::vector<Direction> ASTAR(Board b, int heuristic);
