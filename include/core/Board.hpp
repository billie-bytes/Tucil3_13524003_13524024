#pragma once
#include <fstream>
#include <vector>
#include <array>
#include <unordered_map>

enum class Direction : int {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum Tile {
    LAVA = 998,
    WALL = 999,
};

class Board {
private:
    int** matrix;
    Board(int panjang, int lebar);
    
    bool ifOrdered();
    
public:
    std::unordered_map<int, std::unordered_map<int, int>> orderedTiles; // orderedTiles[x][y] = order on position x,y tile 
    int panjang;
    int lebar;
    int pinX;
    int pinY;
    int winX;
    int winY;
    int ord;
    void placeWall(int x, int y);
    void placeLava(int x, int y);
    void placeValue(int x, int y, int val);
    void placeMainChar(int x, int y);
    void placeWin(int x, int y);
    static Board* create(int panjang, int lebar);
    static Board* create(std::ifstream& config);
    int** getMatrix() const ;

    /**
     * @brief moveButNotRly itu cuma buat cek apakah move itu valid, and cek cost nya.
     * fungsi ini ga beneran mindahin posisi pinnya.
     * 
     * @brief move is exactly the same as moveButNotRly tapi dia beneran mindahin posisi pinnya.
     * 
     * @return kedua fungsi ini bakal return cost dari jalan ke arah itu.
     * @return kedua fungsi bakal return -1 kalo:
     *  - Hit tile ordered tapi ga sesuai order
     *  - Hit lava
     */
    int moveButNotRly(Direction d);
    int move(Direction d);
    ~Board();
    Board& operator=(const Board& other);
    
};