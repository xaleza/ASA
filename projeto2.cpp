#include <vector>  //for std::vector
#include <list> 
#include <iterator>
#include <algorithm>
#include <set>
using namespace std; 

class Graph
{
private:
    /* data */
    int numVertices;
    std::vector<vector<int> > adjLists;
    std::vector<vector<int> > capacity;
   
public:
    Graph(int vertices)
    {
        numVertices = vertices;
        adjLists.resize(vertices);
        capacity.resize(vertices);
    }
    //~Graph();

    void addEdge(int src, int dest, int cap)
    {
        adjLists[src].push_back(dest);
        capacity[src].push_back(cap);
    }

    int getLength(int i)
    {
        return adjLists[i].size();
    }

    int getAdj(int i, int j)
    {
        return adjLists[i][j];
    }

    int getCap(int i, int j)
    {
        return capacity[i][j];
    }
    
    void print()
    {
        for(int i=0; i < numVertices; i++)
        {
            printf("%d: ", i);
            for(int j=0; j < getLength(i); j++)
                printf("%d/%d ", getAdj(i,j), getCap(i,j));
            printf("\n");
        }
    }
};


int main()
{
    //GRAFO:
    //0: vertice source
    //1: hipermercado
    //2...f+1: fornecedores
    //f+2... f+2+2*e: pontos de abastecimento (2x uma vez que guardamos dois vertices para cada vertice)
    
    int x;
    int y;

    int f = 0; //numero de fornecedores
    int e = 0; //numero de estacoes de abastecimento
    int t = 0; //numero de ligacoes
    int vertices = 0;
    int cap;
    
    
    scanf("%d %d %d", &f, &e, &t);

    //divisoes do grafo
    int source = 0; //vertice source
    int hiper = 1;
    int fornecedores = 2; //onde comeca os fornecedores
    int abastecimentos = f+2; //onde comeca os abast..
    int fimAbastecimentos = abastecimentos + 2*e;

    vertices = 2 + f + 2*e;
    

    Graph graph(vertices);

    //liga o vertice source aos fornecedores
    for(int i = 2; i < abastecimentos; i++)
    {
        scanf("%d", &cap);
    
        graph.addEdge(source, i, cap);
        
    }
   
    //transforma os pts de abastecimento em dois vertices
    for(int i = abastecimentos; i < fimAbastecimentos; i+=2)
    {
        scanf("%d", &cap);

        graph.addEdge(i, i+1, cap);
    }

    //trata das ligacoes
    for(int i = 0; i < t; i++)
    {
        scanf("%d %d %d", &x, &y, &cap);
        
        //se for uma ligacao a partir de um abastecimento, incrementar para ficar no vertice certo
        if(x >= abastecimentos)
        {
            x++;
        }
        graph.addEdge(x, y, cap);
    }

    return 0;
}