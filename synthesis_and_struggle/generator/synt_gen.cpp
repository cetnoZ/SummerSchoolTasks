#include <iostream>
#include <time.h>
#include <random>
#include <inttypes.h>
#include <string>
#include <fstream>
#include <memory.h>
#include <vector>
#include <algorithm>
#include <list>
#include <set>







using namespace std; 
  
// Define the number of runs for the test data 
// generated 
#define RUN 1 
  
// Define the maximum number of nodes of the tree 
#define MAXNODE 100 
  
class Tree 
{ 
    int V;    // No. of vertices 
  
    // Pointer to an array containing adjacency listss 
    list<int> *adj; 
  
    // used by isCyclic() 
    bool isCyclicUtil(int v, bool visited[], bool *rs); 
public: 
    Tree(int V);   // Constructor 
    void addEdge(int v, int w);   // adds an edge 
    void removeEdge(int v, int w);   // removes an edge 
  
    // returns true if there is a cycle in this graph 
    bool isCyclic(); 
}; 
  
// Constructor 
Tree::Tree(int V) 
{ 
    this->V = V; 
    adj = new list<int>[V]; 
} 
  
void Tree::addEdge(int v, int w) 
{ 
    adj[v].push_back(w); // Add w to v’s list. 
} 
  
void Tree::removeEdge(int v, int w) 
{ 
    list<int>::iterator it; 
    for (it=adj[v].begin(); it!=adj[v].end(); it++) 
    { 
        if (*it == w) 
        { 
            adj[v].erase(it); 
            break; 
        } 
    } 
    return; 
} 
  
// This function is a variation of DFSUytil() in 
// https://www.geeksforgeeks.org/depth-first-search-or-dfs-for-a-graph/ 
bool Tree::isCyclicUtil(int v, bool visited[], bool *recStack) 
{ 
    if (visited[v] == false) 
    { 
        // Mark the current node as visited and part of 
        // recursion stack 
        visited[v] = true; 
        recStack[v] = true; 
  
        // Recur for all the vertices adjacent to this vertex 
        list<int>::iterator i; 
        for (i = adj[v].begin(); i != adj[v].end(); ++i) 
        { 
            if (!visited[*i] && isCyclicUtil(*i, visited, recStack)) 
                return true; 
            else if (recStack[*i]) 
                return true; 
        } 
  
    } 
    recStack[v] = false;  // remove the vertex from recursion stack 
    return false; 
} 
  
// Returns true if the graph contains a cycle, else false. 
// This function is a variation of DFS() in 
// https://www.geeksforgeeks.org/depth-first-search-or-dfs-for-a-graph/ 
bool Tree::isCyclic() 
{ 
    // Mark all the vertices as not visited and not part of recursion 
    // stack 
    bool *visited = new bool[V]; 
    bool *recStack = new bool[V]; 
    for(int i = 0; i < V; i++) 
    { 
        visited[i] = false; 
        recStack[i] = false; 
    } 
  
    // Call the recursive helper function to detect cycle in different 
    // DFS trees 
    for (int i = 0; i < V; i++) 
        if (isCyclicUtil(i, visited, recStack)) 
            return true; 
  
    return false; 
} 
  
void print_tree(int node, std::ofstream& out_file) 
{ 
    set<pair<int, int>> container; 
    set<pair<int, int>>::iterator it; 
  
    // Uncomment the below line to store 
    // the test data in a file 
    // freopen ("Test_Cases_Unweighted_Tree.in", "w", stdout); 
  
    //For random values every time 
    srand(time(NULL)); 
  
    int NUM;    // Number of Vertices/Nodes 
  
    for (int i=1; i<=RUN; i++) 
    { 
        NUM = node; 
  
        // First print the number of vertices/nodes 
        //printf("%d\n", NUM); 
        Tree t(NUM); 
        // Then print the edges of the form (a b) 
        // where 'a' is parent of 'b' 
        for (int j=1; j<=NUM-1; j++) 
        { 
            int a = rand() % NUM; 
            int b = rand() % NUM; 
            pair<int, int> p = make_pair(a, b); 
  
            t.addEdge(a, b); 
  
            // Search for a random "new" edge everytime 
            while (container.find(p) != container.end() 
                    || t.isCyclic() == true) 
            { 
                t.removeEdge(a, b); 
  
                a = rand() % NUM; 
                b = rand() % NUM; 
                p = make_pair(a, b); 
                t.addEdge(a, b); 
            } 
            container.insert(p); 
        } 
  
        for (it=container.begin(); it!=container.end(); ++it) 
            out_file << it->first + 1 << " " << it->second + 1 << "\n"; 
  
        container.clear(); 
        
    } 
  
    // Uncomment the below line to store 
    // the test data in a file 
    // fclose(stdout); 

}





const uint32_t maxn = 99;
const uint32_t maxp = 1'000'00;
const uint32_t maxa = 1'000'00;

const uint32_t number_of_tests = 50;

uint32_t get_random_a(){
    return (rand() * rand()) % maxa + 1;
}
uint32_t get_random_n(){
    return (rand() * rand()) % maxn + 1;
}

std::vector<std::vector<int>> tree(maxn + 19);
bool check[maxn] = {false};

bool cycle_exists = false;
void is_cycle(int v, int p = -1)
{
    check[v] = true;
    for(int i = 1; i < tree[v].size(); i++)
        if(check[tree[v][i]] && p != tree[v][i])
            cycle_exists = true;
        else
            if(p != tree[v][i])
                is_cycle(tree[v][i], v);
        
}



void make_tests()
{
    for(int32_t i = 1; i <= number_of_tests; i++){
        system("clear");
        std::cout << i * 2 << "%\n";

        std::ofstream out_file;
        out_file.open(("../tests/in/" + std::to_string(i)).c_str());

        uint32_t N = (rand() * rand()) % maxn + 2;
        uint32_t K = (rand() * rand()) % maxa + 1;
        std::vector<uint32_t> array_of_tree;

        out_file << N << " " << K << "\n";

        std::vector<int32_t> tmp;
        for(uint32_t i = 0; i < N - 1; i++)
            tmp.push_back(get_random_a());
        std::sort(tmp.begin(), tmp.end());
        for(const auto& item: tmp)
            out_file << item << " ";
        out_file << "\n";
        print_tree(N, out_file);

        /*for(int i = 0; i < N - 1; i++){
            while (true)
            {    
                uint32_t u = (rand()  % (N)) + 1;
                uint32_t v = (rand() * rand() % (N)) + 1;
                if(u == v)
                    continue;
                if(tree[u].size() != 0 && std::find(std::begin(tree[u]), std::end(tree[u]), v) != std::end(tree[u])){
                    continue;
                }
                
                tree[u].push_back(v);
                tree[v].push_back(u);
            
                cycle_exists = false;
                
                is_cycle(u);
                if(cycle_exists){
                    tree[u].pop_back();
                    tree[v].pop_back();
                    continue;
                }

                out_file << u << " " << v << std::endl;
                break;
            }
        }*/
        out_file << "\n";
        out_file.close();
        tree.clear();
        memset(check, 0, maxn);
    }
}

int main(){
    srand(time(NULL));
    make_tests();
}