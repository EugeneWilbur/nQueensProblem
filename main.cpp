#include <iostream>
#include <list>
#include <queue>
#include <ctime>
#include <vector>
#include <cmath>



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
        gen = 0;

        for(int i = 0; i < n; i++){
            queens.push_back(0);
        }
    }

    ~Board() = default;


    void addChild(int x, int y){
        if(this->children.size() < n ) {
            auto *temp = new Board(n);

            //copies the queens the parent has into the child
            for (int i = 0; i < n; i++) {
                temp->queens[i] = this->queens[i];
            }
            temp->queens[x] = y;
            temp->gen = this->gen + 1;
            temp->printBoard();
            this->children.push_back(temp);
        }
    }

    //recursively sets up each child to have n number of children. This is repeated n times.
    Board* nextGen(bool &first){
        std::cout << "new" << std::endl;
        auto *temp = this;
        for(int i = 0; i < n; i++){
            if(first){
                if(i+1 < n){
                    temp->addChild(this->gen, i + 1);
                }
            } else {
                temp->addChild(this->gen, i);
            }
        }
        return temp;
    }

    Board* graphSetUp(bool &first){
        auto *temp = this;
        if(temp->gen < n){
            temp = temp->nextGen(first);
            first = false;
            for(int i = 0; i < temp->children.size(); i++){
                temp->children[i] = temp->children[i]->graphSetUp(first);
            }

        }
        return temp;
    }

    // Returns number of queen clashes, 0 means no queens clashing for current nQueens.
    int eval(){
        if(n == 0){
            return 1;
        }
        int clashes = 0;
        for(int i = 0; i < n; i++){
            for(int j = i; j < n; j++){
                //q[i] = x, i = y value.
                //if q1[x]-q2[x] == q1[y] - q2[y] they are on the same diagonal, add a clash;
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
        auto current = this;

        frontier.push(current);
        if(current->eval() == 0){
            solutions++;
        }
        while(!frontier.empty()){
            current = frontier.front();
            frontier.pop();
            current->visited = true;
            //current->printBoard();
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


    void simulatedAnnealing(){
        auto * current = this;
        Board * next = nullptr;
        srand(time(nullptr));
        double t = 1, tMin = 0.00001, a = 0.999;
        double probRate, rnum;
        int childrensAngryQueens;



        int evalVal, bestOfGenIndex = 0;
        while(t > tMin){
            for(int i = 0; i < n; i++){
                int angryQueens = current->eval();
                if(angryQueens == 0){
                    current->printBoard();
                    return;
                }
                if(current->children.empty()) {
                    current = this;
                }

                next = current->children[rand() % current->children.size()];

                childrensAngryQueens = next->eval();

                probRate = exp(-(angryQueens - childrensAngryQueens)/t);
                rnum = ((double) rand() / (RAND_MAX));

               // std::cout << probRate << " " << num << std::endl;

                if(childrensAngryQueens < angryQueens || probRate > rnum){
                    current = next;
                }
            }
            t *= a;
        }

    }

private:
    int n, gen;
    std::vector<int> queens; //position of each queen. (i:x position [i]:y position)
    std::vector<Board*> children; // A vector of pointers to n number of children for each node
    bool visited;
};


int main() {
    bool first = true;
    clock_t start_t, end_t;
    double cpu_time_used;

    /*
    auto * root = new Board(4);
    root->printBoard();

    for(int k = 0; k < 16; k++) {
        for (int i = 0; i < 4; i++) {
            for (int j = 1; j < 4; j++) {
                root->addChild(i, j);
            }
        }
    }
*/

    for(int i = 4; i < 5; i++){
        auto *root = new Board(i);
        root->graphSetUp(first);
        start_t = clock();

        std::cout << "The solution for n = " << root->getSize() << " is: " << std::endl;
        std::cout << root->BFS() << std::endl;
       // root->hillClimbSearch();
       // root->simulatedAnnealing();


        end_t = clock();
        cpu_time_used = ((double) (end_t - start_t)) / CLOCKS_PER_SEC;
        std::cout << "Time taken: " << cpu_time_used << std::endl;

    }


    return 0;
}







