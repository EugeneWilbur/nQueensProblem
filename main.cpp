#include <iostream>

class Square{
public:
    void setX(int xc){
        this->x = xc;
    }

    void setY(int yc){
        this->y = yc;
    }

    void placeQueen(){
        this->queen = true;
    }

    void takeQueen(){
        this->queen = false;
    }

    bool hasQueen(){
        return queen;
    }

private:
    int x;
    int y;
    bool queen;
};

class Board{
public:
    int getSize(){
        return n;
    }

    void printBoard(){
        std:: cout << "    ";
        for(int i = 0; i < n; i++){
            std::cout << i << "    ";
        }
        std::cout << std::endl;
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n + 1; j++){
                if(j == 0){
                    std::cout << i << " ";
                } else if(board[i][j - 1].hasQueen() == true) {
                    std::cout << "  " << "Q" << "  ";
                } else {
                    std::cout << "  .  ";
                }
            }
            std::cout << std::endl;
        }
    }

    Board(){
        board = new Square*[n];

        for(int i = 0; i < n; i++){
            board[i] = new Square[n];
        }

        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                board[i][j].setX(i);
                board[i][j].setY(j);
            }
        }
    }

private:
    int n = 4;
    Square **board;

};



int main() {
    Board board;

    board.printBoard();
    return 0;
}
