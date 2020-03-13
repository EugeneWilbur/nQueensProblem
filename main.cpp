#include <iostream>
#include <list>
#include <queue>



class Square{
public:

    Square(): x{0}, y{0}, queen{false} {};
    explicit Square(int xc, int yc, bool queenc): x{xc}, y{yc}, queen{queenc} {};

    void placeQueen(){
        this->queen = true;
    }

    void takeQueen(){
        this->queen = false;
    }

    bool hasQueen(){
        return queen;
    }

    void setPosition(int xc, int yc){
        this->x = xc;
        this->y = yc;
    }

    int getX(){
        return x;
    }

    int getY(){
        return y;
    }

private:
    int x, y;
    bool queen;
};

class Board{
public:
    int getSize(){
        return n;
    }

    //prints current board state (for debugging and visualisation mostly).
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
                } else if(squares[i][j - 1].hasQueen()) {
                    std::cout << "  " << "Q" << "  ";
                } else {
                    std::cout << "  .  ";
                }
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    //init n*n board of 2d Squares by
    Board(){
        squares = new Square*[n];
        queens = new int*[n];
        children = nullptr;
        for(int i = 0; i < n; i++){
            squares[i] = new Square[n];
            queens[i] = new int[2] {-1, -1};
        }

        //let all squares know there x and y position
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                squares[i][j].setPosition(i,j);
            }
        }
        nQueens = 0;
        nChildren = 0;
    }

    ~Board(){
        for(int i = 0; i < n; i++){
            delete [] squares[i];
        }
        delete [] squares;
    }

    //this class is just for 2 dimensional [][] operator overload (2nd [])
    class operatorClass{
    public:
        explicit operatorClass(Square *squares): squares(squares){}

        Square& operator[](int index){
            //index is the value in the send set of [];
            // e.g. board[nums][index];
            return squares[index];
        }
    private:
        Square * squares;
    };

    //outside operator overload (first [])
    operatorClass operator [](int num){
        //num is value in the first set of [];
        if (num > n || num < 0){
            std::cout << "Index out of bounds." << std::endl;
            return operatorClass(squares[0]);
        } else {
            return operatorClass(squares[num]);
        }
    }

    int getnQueens(){
        return nQueens;
    }

    Board* createChild(){
        auto *temp = new Board();

        //copies the queens the parent has into the child

        for(int i = 0; i < nQueens; i++){
            if(this->queens[i][0] >= 0 && this->queens[i][1] >= 0) {
                temp->squares[this->queens[i][0]][this->queens[i][1]].placeQueen();
            }
        }
        return temp;
    }

    void addChild(){
        if(children == nullptr) {
            children = createChild();
            nChildren++;
        }
    }

    void queenTest(){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < 2; j++){
                std::cout << queens[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    void addQueen(int x, int y){
        this->squares[x][y].placeQueen();
        queens[nQueens][0] = x;
        queens[nQueens][1] = y;
        nQueens++;
    }

    // TODO dont leave this here
    // TODO add multiple children
    Board *children;

private:
    int n = 4;
    int nQueens, nChildren;
    int **queens;
    Square **squares;
};


int main() {
    Board root;
    root.createChild();

    root.addQueen(0,0);


    root.printBoard();

    root.addChild();

    root.children->addQueen(1, 2);

    root.children->printBoard();
    root.printBoard();

    return 0;
}




