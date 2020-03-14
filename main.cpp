#include <iostream>
#include <list>
#include <queue>


//class for each square in the board. Keeps track of its position and whether it has a queen or not.
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
        for(int i = 0; i < n; i++){
            squares[i] = new Square[n];
            queens[i] = new int[2] {-1, -1};
            children[i] = nullptr;
        }

        //let all squares know there x and y position
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                squares[i][j].setPosition(i,j);
            }
        }
        nQueens = 0;
        nChildren = 0;
        parent = nullptr;
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

    void addChild(){
        if(children[nChildren] == nullptr && nChildren <= n) {
            children[nChildren] = createChild();
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
        squares[x][y].placeQueen();
        queens[nQueens][0] = x;
        queens[nQueens][1] = y;
        nQueens++;
    }

    Board *backGen(){
        return parent;
    }

    void addQueenToChild(int child, int queenX, int queenY){
        if(child <= nChildren){
            children[child]->addQueen(queenX, queenY);
        } else {
            std::cout << "No child at index: " << child << std::endl;
        }
    }

    Board *nextGen(int i){
        if(i <= nChildren){
            return children[i];
        }
        else {
            std::cout << "No child at that index" << std::endl;
            return this;
        }
    }

    void printChild(){
        for(int i = 0; i < nChildren; i++){
            children[i]->printBoard();
        }
    }

    int **getQueensPosition(){
        return queens;
    }

    //recursively sets up each child to have n number of children. This is repeated n times.
    Board* treeSetUp(int depth = 0){
        if(depth == n) {
            //returns the 'youngest' generation of children
            return this;

        }
        for(int i = 0; i < n; i++){
            this->addChild();
            this->addQueenToChild(i, depth, i);
            //calls this function again for each child
            this->children[i] = this->children[i]->treeSetUp(depth+1);
        }
        //returns the root of the whole data struct.
        return this;
    }

    bool isGoal(){
        if(this->getnQueens() != n){
            return false;
        } else {
            for(int i = 0; i < n; i++){
                for(int j = i; j < n; j++){
                    //if qi[x coordinate] == qj[x coordinate] return false;
                    //if qi[y coordinate] == qj[y coordinate] return false;
                    //if qi[x]-qj[x] == qi[y] - qj[y] they are on the same diagonal, return false;
                    if((queens[i][0] == queens[j][0] || queens[i][1] == queens[j][1] || abs(queens[i][0] - queens[j][0]) == abs(queens[i][1] - queens[j][1])) && i != j) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

private:
    int n = 4;
    int nQueens, nChildren; // number of queens and children in the current board state.
    int **queens; //position of each queen.
    Square **squares; //grid of squares to make up the board
    Board *parent; //parent of each // TODO "parent" might be unneccassary
    Board *children[4];
    //made this private as i want "add child()" to call this function. I dont want
    //this function being called on its own.
    // TODO see if you can make this public
    Board* createChild(){
        auto *temp = new Board();

        //copies the queens the parent has into the child

        for(int i = 0; i < nQueens; i++){
            if(this->queens[i][0] >= 0 && this->queens[i][1] >= 0) {
                temp->squares[this->queens[i][0]][this->queens[i][1]].placeQueen();
                temp->queens[i][0] = this->queens[i][0];
                temp->queens[i][1] = this->queens[i][1];
                temp->nQueens = this->nQueens;
                temp->parent = this;
            }
        }
        return temp;
    }
};


int BFS(Board *root); //returns number of soltuions.


int main() {

    //TODO see if you even need the squares class
    auto *root = new Board;
    auto *token = new Board;

    root = root->treeSetUp();

    token = root;
    /*
    for(int i = 0; i < root->getSize(); i++){
        token->printChild();
        token = token->nextGen(3);
    }*/

    token = token->nextGen(1);
    token = token->nextGen(3);
    token = token->nextGen(0);
    std::cout << "1 = true, 0 = false." << std::endl;
    std::cout << token->isGoal() << std::endl;
    token->printBoard();

    token = token->nextGen(2);
    std::cout << token->isGoal() << std::endl;
    token->printBoard();


    return 0;
}



int BFS(Board *root){
    //TODO
    return 0;
}






