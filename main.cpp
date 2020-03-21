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
                } else if(queens[j - 1] == i) {
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
        visited = false;


        for(int i = 0; i < n; i++){
            queens.push_back(0);
        }
    }


    void addChild(){
        if(this->children.size() < n) {
            auto *temp = new Board(n);

            //copies the queens the parent has into the child
            for (int i = 0; i < n; i++) {
                temp->queens[i] = this->queens[i];
            }
            this->children.push_back(temp);
        }
    }

    void addQueen(int x, int y){
        queens[x] = y;
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
            this->children[i]->treeSetUp(depth+1);
        }
        //returns the root of the whole data struct.
        return this;
    }

    // Returns number of queen clashes, 0 means no queens clashing for current nQueens.
    int eval(){
        int clashes = 0;
        for(int i = 0; i < n; i++){
            for(int j = i; j < n; j++){
                //if qi[x coordinate] == qj[x coordinate] return false;
                //if qi[y coordinate] == qj[y coordinate] return false;
                //if qi[x]-qj[x] == qi[y] - qj[y] they are on the same diagonal, return false;
                if((queens[i] == queens[j] || abs(queens[i] - queens[j]) == abs(i - j)) && i != j) {
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
            current->printBoard();
            for(int i = 0; i < current->children.size(); i++){
                if(!current->children[i]->visited){
                    frontier.push(current->children[i]);
                    if(current->children[i]->eval() == 0){
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
            if(current->eval() == 0 || current->children.empty()) {
                current->printBoard();
                return;
            }
            bestOfGenIndex = 0;
            for(int i = 1; i < current->children.size(); i++){
                //evalVal saves me calling eval() multiple times.
                evalVal = current->children[i]->eval();

                //keep track of which of the children are best, so we can explore that node next.
                if(evalVal < current->children[bestOfGenIndex]->eval()){

                    bestOfGenIndex = i;
                }
            }
            //if the board state is not improving, print the best state we found, and return.
            if(current->children[bestOfGenIndex]->eval() > current->eval()){
                current->printBoard();
                return;
            }
            //move to the next node.
            current = current->children[bestOfGenIndex];
        }
    }

private:
    int n;
    std::vector<int> queens; //position of each queen. (i:x position [i]:y position)
    std::vector<Board*> children; // A vector of pointers to n number of children for each node
    bool visited;
};



int main() {
    clock_t start_t, end_t;
    double cpu_time_used;


    for(int i = 4; i < 10; i++){

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







