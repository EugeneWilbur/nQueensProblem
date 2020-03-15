#include <iostream>
#include <list>
#include <queue>
#include <ctime>

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
        children = new Board*[n];
        queens = new int*[n];
        for(int i = 0; i < n; i++){
            queens[i] = new int[2] {-1, -1};
            children[i] = nullptr;
        }
        nQueens = 0;
        nChildren = 0;
        visited = false;
    }

    ~Board(){
        for(int i = 0; i < 2; i++){
            delete queens;
        }
        delete [] queens;
        for(int i = 0; i < n; i++){
            delete children;
        }
        delete [] children;
    }

    void addChild(){
        if(children[nChildren] == nullptr && nChildren < n) {
            auto *temp = new Board(n);

            //copies the queens the parent has into the child
            for (int i = 0; i < nQueens; i++) {
                temp->queens[i][0] = this->queens[i][0];
                temp->queens[i][1] = this->queens[i][1];
                temp->nQueens = this->nQueens;
            }
            children[nChildren] = temp;
            nChildren++;
        }
    }

    void addQueen(int x, int y){
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
        if(this->isGoal()){
            for(int i = 0; i < n; i++){
                this->addChild();
                this->addQueenToChild(i, depth, i);
                //calls this function again for each child
                this->children[i]->treeSetUp(depth+1);
            }
        } else {
            return this;
        }

        //returns the root of the whole data struct.
        return this;
    }

    bool isGoal(){

        for(int i = 0; i < this->nQueens; i++){
            for(int j = i; j < this->nQueens; j++){
                //if qi[x coordinate] == qj[x coordinate] return false;
                //if qi[y coordinate] == qj[y coordinate] return false;
                //if qi[x]-qj[x] == qi[y] - qj[y] they are on the same diagonal, return false;
                if((queens[i][0] == queens[j][0] || queens[i][1] == queens[j][1] || abs(queens[i][0] - queens[j][0]) == abs(queens[i][1] - queens[j][1])) && i != j) {
                    return false;
                }
            }
        }
        return true;
    }

    double BFS(Board *root){
        double solutions = 0;
        int isGoalAns;
        std::queue<Board*> frontier;
        Board *current, *temp;

        frontier.push(root);
        while(!frontier.empty()){
            current = frontier.front();
            frontier.pop();
            current->visited = true;
            for(int i = 0; i < current->nChildren; i++){
                if(!current->children[i]->visited){
                    if (current->children[i]->nQueens != n){
                        frontier.push(current->children[i]);
                    } else if(current->children[i]->isGoal() && current->children[i]->nQueens == n){
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

private:
    int n;
    int nQueens, nChildren; // number of queens and children in the current board state.
    int **queens; //position of each queen.
    Board **children;
    bool visited;
};



int main() {
    clock_t start_t, end_t;
    double cpu_time_used;

    start_t = clock();
    for(int i = 0; i < 10; i++){

        auto *root = new Board(i);
        root = root->treeSetUp();

        std::cout << "Number of solutions for n = " << root->getSize() << " is: " << root->BFS(root) << std::endl;
    }
    end_t = clock();
    cpu_time_used = ((double) (end_t - start_t)) / CLOCKS_PER_SEC;

    std::cout << "Time taken: " << cpu_time_used << std::endl;
    return 0;
}







