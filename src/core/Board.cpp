#include "../../include/core/Board.hpp"
#include <iostream>
#include <string>
#include <cctype>
#include <fstream>
Board::Board(int panjang, int lebar):panjang(panjang),lebar(lebar),ord(0),winX(-1),winY(-1){
    matrix = new int*[panjang];
    for(int i = 0; i<panjang; i++){
        matrix[i]=new int[lebar]();
    }
}

bool Board::ifOrdered(){
    auto x_it = orderedTiles.find(pinX);
    
    if (x_it != orderedTiles.end()) {
        auto y_it = x_it->second.find(pinY);
        
        if (y_it != x_it->second.end()) {
            int targetOrder = y_it->second; 
            
            if (targetOrder == -1) {
                return true;
            }
            
            if (ord == targetOrder) {
                ++ord;
                return true;
            } else {
                return false;
            }
        }
    }
    return true;
}

void Board::placeLava(int x, int y){
    if(x<0||y<0){
        std::cout << "Where are you putting it, kiddo" << std::endl;
        return;
    }
    if(x==0||y==0){
        std::cout << "Can't put lava on the outside wall buster" << std::endl;
        return;
    }
    matrix[x][y] = LAVA; //Unique ident for lava cuz WALL is for walls
}

void Board::placeWall(int x, int y){
    if(x<0||y<0){
        std::cout << "Where are you putting it, pal" << std::endl;
        return;
    }
    if(matrix[x][y]==LAVA){
        matrix[x][y] = WALL;
        std::cout << "Replaced the lava at [" << x << "," << y <<"] with a wall" << std::endl;
        return; 
    }
    matrix[x][y] = WALL;
}

void Board::placeValue(int x, int y, int value){
    if(x<0||y<0){
        return;
    }
    if(value == WALL || value == LAVA){
        return;
    }
    matrix[x][y] = value;
}

void Board::placeMainChar(int x, int y){
    if(x<0||y<0){
        std::cout << "Where are you putting it, blud" << std::endl;
        return;
    }
    if(matrix[x][y] == WALL || matrix[x][y] == LAVA){
        std::cout << "Those squares already have lava or a wall, buster" << std::endl;
        return;
    }
    pinX = x;
    pinY = y;
}

void Board::placeWin(int x, int y){
    if(x<0||y<0){
        std::cout << "Where are you putting it, blud" << std::endl;
        return;
    }
    if(matrix[x][y] == WALL || matrix[x][y] == LAVA){
        std::cout << "Those squares already have lava or a wall, buster" << std::endl;
        return;
    }
    winX = x;
    winY = y;
}

Board::~Board(){
    for(int i = 0; i<panjang; ++i){
        delete[] matrix[i];
    }
    delete[] matrix;
}

Board* Board::create(int panjang, int lebar){
    if(panjang>=1&&lebar>=1)return (new Board(panjang, lebar));
    else return nullptr;
}


Board* Board::create(std::ifstream& config){
    int n, m;
    if (!(config >> n >> m)) {
        return nullptr;
    }
    
    Board* board = Board::create(n, m);
    if (!board) {
        return nullptr;
    }
    
    std::vector<std::string> layout(n);
    for(int i = 0; i < n; ++i){
        config >> layout[i];
    }

    for(int i = 0; i < n; ++i){
        for(int j = 0; j < m; ++j){
            int cost;
            config >> cost;

            char tile = layout[i][j];

            if(tile == 'X'){
                board->placeWall(i, j);
            } else if(tile == 'L'){
                board->placeLava(i, j);
            } else {
                if(tile=='O')board->placeWin(i,j);
                board->placeValue(i, j, cost);
                
                if(tile == 'Z'){
                    board->placeMainChar(i, j);
                } 
                
                if(std::isdigit(tile)){
                    int order = tile - '0';
                    board->orderedTiles[i][j] = order;
                } else {
                    board->orderedTiles[i][j] = -1;
                }
            }
        }
    }
    return board;
}

int** Board::getMatrix() const {
    return matrix;
}

int Board::moveButNotRly(Direction d){
    int savedX = pinX;
    int savedY = pinY;
    int cost = 0;

    while(true){
        int nextX = pinX;
        int nextY = pinY;
        
        switch(d){
            case Direction::UP: nextY--; break;
            case Direction::DOWN: nextY++; break;
            case Direction::LEFT: nextX--; break;
            case Direction::RIGHT: nextX++; break;
        }

        if(matrix[nextX][nextY] == WALL){
            break; 
        }

        pinX = nextX;
        pinY = nextY;

        if(matrix[pinX][pinY] == LAVA){
             pinX = savedX;
             pinY = savedY;
             return -1;
        } 
        if(!ifOrdered()){
             pinX = savedX;
             pinY = savedY;
             return -1;
        }  
        
        cost += matrix[pinX][pinY];
    }
    
    pinX = savedX;
    pinY = savedY;
    return cost;
}

int Board::move(Direction d){
    int cost = 0;
    
    while(true){
        int nextX = pinX;
        int nextY = pinY;
        
        switch(d){
            case Direction::UP: nextY--; break;
            case Direction::DOWN: nextY++; break;
            case Direction::LEFT: nextX--; break;
            case Direction::RIGHT: nextX++; break;
        }

        if(matrix[nextX][nextY] == WALL){
            break; 
        }

        pinX = nextX;
        pinY = nextY;

        if(matrix[pinX][pinY] == LAVA) return -1;
        
        cost += matrix[pinX][pinY];
    }
    return cost;
}

Board& Board::operator=(const Board& other) {
    if (this == &other) {
        return *this;
    }

    if (matrix != nullptr) {
        for(int i = 0; i < panjang; ++i){
            delete[] matrix[i];
        }
        delete[] matrix;
    }

    panjang = other.panjang;
    lebar = other.lebar;
    pinX = other.pinX;
    pinY = other.pinY;
    winX = other.winX;
    winY = other.winY;
    ord = other.ord;
    orderedTiles = other.orderedTiles;

    matrix = new int*[panjang];
    for(int i = 0; i < panjang; ++i){
        matrix[i] = new int[lebar];
        for(int j = 0; j < lebar; ++j){
            matrix[i][j] = other.matrix[i][j];
        }
    }

    return *this;
}