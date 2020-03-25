#include <iostream>
#include <list>
#include <queue>
#include <ctime>
#include <vector>
#include <cmath>



class Board{
public:
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

    Board(Board const & that) = default;
    Board(Board && that) noexcept = default;
    Board & operator = (Board const & that) noexcept = default;
    Board & operator = (Board && that) noexcept = default;

    ~Board() = default;


    Board* addChild(int x, int y){
        auto *temp = new Board(n);

        //copies the queens the parent has into the child
        for (int i = 0; i < n; i++) {
            temp->queens[i] = this->queens[i];
        }
        temp->queens[x] = y;
        temp->gen = this->gen +1;
        return temp;

    }

    //recursively sets up each child to have n number of children. This is repeated n times.
    std::vector<Board*> nextGen(bool first){
        std::vector<Board*> temp;
        for(int i = 0; i < n; i++){
            if(first){
                if(i+1 < n){
                    temp.push_back(this->addChild(this->gen, i + 1));
                }
            } else {
                temp.push_back(this->addChild(this->gen, i));
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

    //returns total number of goal states found.
    double BFS(){
        double solutions = 0;
        std::queue<Board*> frontier;
        std::vector<Board*> children;
        bool first = true;
        auto current = this;

        frontier.push(current); //push root onto queue to start BFS.

        if(current->eval() == 0){ // check if initial condition is a goal state.
            solutions++;
        }

        while(!frontier.empty()){ //while we have nodes left to search
            current = frontier.front(); //pop the oldest node off the queue.
            frontier.pop();

            current->visited = true; //make sure we dont check this node again.

            if(current->gen < n){
                children = current->nextGen(first); //create children for the current node as we are going to need to check them.
                first = false; //just a variable to check if we are passing in the I.C or not.
            }
            for(auto & i : children){ //go through all children of the current node.
                if(!i->visited){ //if we haven't visited those children add them too queue.
                    frontier.push(i);
                    if(i->eval() == 0){  // if child is goal state increase number of solutions.
                        if(n < 7){
                            i->printBoard(); //if n < 7 print the current solution.
                        }
                        solutions++;
                    }
                }
            }
            children.clear();
        }
        return solutions;
    }

    //prints goal state if found, prints failure if not found.
    void hillClimbSearch(){
        auto * current = this;
        int evalVal, bestOfGenIndex = 0;
        std::vector<Board*> children;
        bool first = true;

        if(n == 0){
            std::cout << "Hill climb failed to find a solution." << std::endl; //ran out of nodes in hill climb.
            return;
        }
        while(true){
            if(current->gen < n) {
                children.clear();
                children = current->nextGen(first);
            }

            if(current->eval() == 0 || children.empty()) { //if goal state found or no more nodes we are going to stop
                if(current->eval() == 0){
                    current->printBoard(); //print solution
                } else {
                    std::cout << "Hill climb failed to find a solution." << std::endl; //ran out of nodes in hill climb.
                }
                return;
            }
            bestOfGenIndex = 0; //reset, so we can find the best of the next lot of children to be checked.
            for(int i = 1; i < children.size(); i++){ // go through all children (expect first, because first
                //                                                  is a duplicate of the parent.
                evalVal = children[i]->eval(); //evalVal saves me calling eval() multiple times.

                //keep track of which of the children are best, so we can explore that node next.
                if(evalVal < children[bestOfGenIndex]->eval()){
                    bestOfGenIndex = i;
                }
            }
            //if the board state is not improving, print the best state we found, and return.
            if(children[bestOfGenIndex]->eval() > current->eval()){
                std::cout << "Hill climb failed to find a solution." << std::endl;
                return;
            }
            //move to the next node.
            current = children[bestOfGenIndex];
            children.clear();

        }
    }


    void simulatedAnnealing(){
        auto * current = this;
        Board * next = nullptr;
        bool first = true;
        std::vector<Board*> children;

        srand(time(nullptr));
        double t = 10, tMin = 0.001, a = 0.99;
        double probRate, rnum;
        int childrensAngryQueens, angryQueens; //number of clashing pairs of queens for child and current node.

        if(n == 0){
            std::cout << "Simulated Annealing failed to find solution." << std::endl;
            return;
        }
        while(t > tMin){
            int j = 0;
            while(j < 100){
                //if we go to deep into the tree, this reset us back to the top. We keep going through the tree
                // until either goal state is found, or our pre set condition is met (t has reduced to far).
                if(current->gen == n || children.empty()) {
                    current = this;
                    for(auto i : children){
                        delete i;
                    }
                    children.clear();
                    children = current->nextGen(first);
                }


                angryQueens = current->eval(); // angryQueens is set to the number of clashing pairs of queens for current
                if(angryQueens == 0){ //if no clashing pairs, goal state is found, print goal state.
                    current->printBoard();
                    return;
                }
                next = children[rand() % children.size()]; // pick a random child.
                childrensAngryQueens = next->eval(); // find childs number of clashing queens.

                if(childrensAngryQueens == 0){
                    next->printBoard();
                    return;
                }


                // probRate is a value that is always between 1-0 (at least for the conditions we need it, e.g. when
                // childrensAngryQueens is greater than angryQueens). The higher the number, the higher the chance we
                // 'turn around' and take a larger clashing queens value.

                // This value is smaller the bigger the difference in the number of clashing queens. This value also
                // becomes smaller and smaller as t reduces.

                // This results in us sometimes going to the next node even if it is  bigger. But the chance of this
                // happening is reduced the bigger the gap in number of queens and the longer we explore the graph.
                probRate = (double)exp(-((childrensAngryQueens - angryQueens)/t));
                rnum = ((double) rand() / (RAND_MAX));

                if(childrensAngryQueens < angryQueens || probRate > rnum){
                    current = next; // go to next node if it has less than
                    if(current->gen < n){
                        children.clear();
                        for(auto i : children){
                            delete i;
                        }
                        children = current->nextGen(first); //create children for the current node as we are going to need to check them.
                        first = false; //just a variable to check if we are passing in the I.C or not.
                    }
                }
                j++;
            }
            t *= a; // decrease t.
        }
        for(auto i :children){
            delete i;
        }
        delete current;
        std::cout << "Simulated Annealing failed to find solution." << std::endl;
    }

private:
    int n, gen; // n is size of the board. gen is depth in the tree.
    std::vector<int> queens; //position of each queen. (i:x position [i]:y position)
    //std::vector<Board*> children; // A vector of pointers to n number of children for each node
    bool visited; //just to keep track of if we have visited it in bfs.
};


int main() {
    clock_t start_t, end_t;
    double cpu_time_used;

    for(int i = 0; i < 8; i++){
        auto *root = new Board(i);



        std::cout << "N = " << i << std::endl;

        start_t = clock();

        std::cout << "The solution for n = " << i << " is: " << std::endl << std::endl;
        std::cout << "BFS: " << root->BFS() << std::endl;

        end_t = clock();
        cpu_time_used = ((double) (end_t - start_t)) / CLOCKS_PER_SEC;
        std::cout << "BFS: " << cpu_time_used << std::endl << std::endl;


        std::cout << "Initial State: " << std::endl;
        root->printBoard();

        std::cout << "Solutions:" << std::endl;

        start_t = clock();
        root->hillClimbSearch();
        end_t = clock();

        cpu_time_used = ((double) (end_t - start_t)) / CLOCKS_PER_SEC;
        std::cout << "Hill Climb Time: " << cpu_time_used << std::endl << std::endl;

        start_t = clock();

        root->simulatedAnnealing();

        end_t = clock();

        cpu_time_used = ((double) (end_t - start_t)) / CLOCKS_PER_SEC;
        std::cout << "SA Time: " << cpu_time_used << std::endl << std::endl << std::endl;

        std::cout << "--------------------------------------------------------------------" << std::endl;

        delete root;
    }



    return 0;
}

