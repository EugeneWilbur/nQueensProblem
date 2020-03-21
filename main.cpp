#include <iostream>
#include <list>
#include <queue>
#include <ctime>
#include <vector>

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
                } else if(queens[i][0] == i && queens[i][1] == j-1) {
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
    explicit Board(int N){
        n = N;
        queens = new int*[n];
        for(int i = 0; i < n; i++){
            queens[i] = new int[2] {-1, -1};
        }
        nQueens = 0;
        visited = false;
    }

    ~Board(){
        for(int i = 0; i < 2; i++){
            delete queens;
        }
        delete [] queens;
    }

    void addChild(){
        if(this->children.size() < n) {
            auto *temp = new Board(n);

            //copies the queens the parent has into the child
            for (int i = 0; i < nQueens; i++) {
                temp->queens[i][0] = this->queens[i][0];
                temp->queens[i][1] = this->queens[i][1];
                temp->nQueens = this->nQueens;
            }
            children.push_back(temp);
        }
    }

    void addQueen(int x, int y){
        queens[nQueens][0] = x;
        queens[nQueens][1] = y;
        nQueens++;
    }

    void addQueenToChild(int child, int queenX, int queenY){
        if(child <= this->children.size()){
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
            //since isGoal checks for nQueens currently on the board, it can also tell us if the current node
            //is on the right track. If it is on the right track (has no clashing queens) it can have children.
            if(this->children[i]->eval() == 0){
                //calls this function again for each child
                this->children[i]->treeSetUp(depth+1);
            }
        }
        //returns the root of the whole data struct.
        return this;
    }

    // Returns number of queen clashes, 0 means no queens clashing for current nQueens.
    int eval(){
        if(this->nQueens != n){
            return n + (n - this->nQueens);
        }
        int clashes = 0;
        for(int i = 0; i < this->nQueens; i++){
            for(int j = i; j < this->nQueens; j++){
                //if qi[x coordinate] == qj[x coordinate] return false;
                //if qi[y coordinate] == qj[y coordinate] return false;
                //if qi[x]-qj[x] == qi[y] - qj[y] they are on the same diagonal, return false;
                if((queens[i][0] == queens[j][0] || queens[i][1] == queens[j][1] || abs(queens[i][0] - queens[j][0]) == abs(queens[i][1] - queens[j][1])) && i != j) {
                    clashes++;
                }
            }
        }
        return clashes;
    }


    double BFS(){
        double solutions = 0;
        int isGoalAns;
        std::queue<Board*> frontier;
        Board *current, *temp;

        frontier.push(this);
        while(!frontier.empty()){
            current = frontier.front();
            frontier.pop();
            current->visited = true;
            for(int i = 0; i < current->children.size(); i++){
                if(!current->children[i]->visited){
                    if (current->children[i]->nQueens != n){
                        frontier.push(current->children[i]);
                    } else if(current->children[i]->eval() == 0 && current->children[i]->nQueens == n){
                        if(n < 7){
                            //current->children[i]->printBoard();
                        }
                        solutions++;
                    }
                }
            }
        }
        return solutions;
    }

    void hillClimbSearch(){
        auto * current = this;
        int evalVal, bestOfGenIndex = 0;
        while(true){
            bestOfGenIndex = 0;
            for(int i = 0 ; i < current->children.size(); i++){
                evalVal = children[i]->eval();

                if(evalVal >= current->eval()){
                    current->printBoard();
                    return;
                }
                if(evalVal < children[bestOfGenIndex]->eval()){
                    bestOfGenIndex = i;
                }
            }
            std::cout << bestOfGenIndex << current->children.size();
            current = current->children[bestOfGenIndex];
        }
    }

private:
    int n;
    int nQueens; // number of queens and children in the current board state.
    int **queens; //position of each queen.
    std::vector<Board*> children;
    bool visited;
};



int main() {
    clock_t start_t, end_t;
    double cpu_time_used;

    for(int i = 1; i < 13; i++){

        auto *root = new Board(i);
        root = root->treeSetUp();
        start_t = clock();
        std::cout << "The solution for n = " << root->getSize() << " is: " << std::endl;
        root->hillClimbSearch();
        end_t = clock();
        cpu_time_used = ((double) (end_t - start_t)) / CLOCKS_PER_SEC;

        std::cout << "Time taken: " << cpu_time_used << std::endl;
    }


    return 0;
}







