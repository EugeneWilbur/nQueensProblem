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
        std::cout << "\n    ";
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
        visited = false;
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
                temp->queens[i][0] = this->queens[i][0];
                temp->queens[i][1] = this->queens[i][1];
                temp->nQueens = this->nQueens;
            }
        }
        return temp;
    }

    void addChild(){
        if(children[nChildren] == nullptr && nChildren <= n) {
            children[nChildren] = createChild();
            nChildren++;
        }
    }

    void addQueen(int x, int y){
        squares[x][y].placeQueen();
        queens[nQueens][0] = x;
        queens[nQueens][1] = y;
        nQueens++;
    }

    void addQueenToChild(int child, int queenX, int queenY){
        if(child <= nChildren){
            children[child]->addQueen(queenX, queenY);
        } else {
            std::cout << "No child at index: " << child << std::endl;
        }
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

    int isGoal(){
        if(this->getnQueens() != n){
            return 1;
        } else {
            for(int i = 0; i < n; i++){
                for(int j = i; j < n; j++){
                    //if qi[x coordinate] == qj[x coordinate] return false;
                    //if qi[y coordinate] == qj[y coordinate] return false;
                    //if qi[x]-qj[x] == qi[y] - qj[y] they are on the same diagonal, return false;
                    if((queens[i][0] == queens[j][0] || queens[i][1] == queens[j][1] || abs(queens[i][0] - queens[j][0]) == abs(queens[i][1] - queens[j][1])) && i != j) {
                        return 0;
                    }
                }
            }
        }
        return 2;
    }

    int BFS(Board *root){
        int solutions = 0, isGoalAns;
        std::queue<Board*> frontier;
        Board *current, *temp;

        if(root->isGoal() == 2){
            root->printBoard();
            solutions++;
        }
        frontier.push(root);
        while(!frontier.empty()){
            current = frontier.front();
            frontier.pop();
            current->visited = true;
            for(int i = 0; i < nChildren; i++){
                if(!current->children[i]->visited){
                    isGoalAns = current->children[i]->isGoal();
                    if(isGoalAns == 2){
                        current->children[i]->printBoard();
                        solutions++;
                    } else if (isGoalAns == 1){
                        frontier.push(current->children[i]);
                    }
                }
            }
        }
        return solutions;
    }

private:
    int n = 6;
    int nQueens, nChildren; // number of queens and children in the current board state.
    int **queens; //position of each queen.
    Square **squares; //grid of squares to make up the board
    Board *children[6];
    bool visited;
};



int main() {

    //TODO see if you even need the squares class
    auto *root = new Board;

    root = root->treeSetUp();


    std::cout << "Number of solutions for n = " << root->getSize() << " is: " << root->BFS(root) << std::endl;
    return 0;
}







