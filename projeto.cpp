#include <vector>  //for std::vector
#include <list> 
#include <iterator>
#include <algorithm>
#include <set>
using namespace std; 


int currTime = 0;


class Graph
{
private:
    /* data */
    int numVertices;
    std::set<int> artPts;
    std::vector<vector<int> > adjLists;
    std::vector<int> visited;
    std::vector<int> parent;
    std::vector<int> low;
   
public:
    Graph(int vertices)
    {
        numVertices = vertices;
        adjLists.resize(vertices);
        visited.resize(vertices);
        parent.resize(vertices);
        low.resize(vertices);
        
        for(int i = 0; i < vertices; i++)
        {
            visited[i] = -1;
            parent[i] = -1;
            low[i] = -1;
        }

    }
    //~Graph();

    void addEdge(int src, int dest)
    {
        adjLists[src].push_back(dest);
        adjLists[dest].push_back(src);
    }

    int getLength(int i)
    {
        return adjLists[i].size();
    }

    int getAdj(int i, int j)
    {
        return adjLists[i][j];
    }

    void addArtPt(int i)
    {
        artPts.insert(i);
    }

    int getVisited(int i)
    {
        return visited[i];
    }

    int getParent(int i)
    {
        return parent[i];
    }

    int getLow(int i)
    {
        return low[i];
    }

    void setLow(int v, int l)
    {   
        low[v] = l;
    }

    void setVisited(int v, int t)
    {
        visited[v] = t;
    }

    void setParent(int v, int p)
    {
        parent[v] = p;
    }

    int getArtPtsSize()
    {
        return artPts.size();
    }

    bool isArtPt(int i)
    {
        return artPts.find(i) != artPts.end();
    }

    void clearVisited()
    {
        for(int i=0; i<numVertices; i++){
            visited[i] = -1;
        }
    }
  
};

void dfs(int i, int p, Graph &graph, std::vector<int> &subgraph)
{
            graph.setVisited(i, currTime);
            graph.setLow(i, currTime);     

            subgraph.push_back(i+1);

            int nIndChildren = 0;

            for (int j = 0; j < graph.getLength(i); j++) 
            {//enqt ha um proximo
                int curr = graph.getAdj(i,j);

                if(graph.getVisited(curr)<0)
                {//next ainda n foi visitado
                    nIndChildren++;

                    graph.setParent(curr, i); 
                    
                    ++currTime;
                    dfs(curr, i, graph, subgraph);

                    //temos que estar no atual e ver o filho

                    if(graph.getLow(curr)<graph.getLow(i))
                    {
                        graph.setLow(i, graph.getLow(curr));
                    }

                    if(graph.getVisited(i)<=graph.getLow(curr) && p>=0 && !graph.isArtPt(i)){
                        graph.addArtPt(i);
                    }

                    if(nIndChildren >= 2 && p<0 && !graph.isArtPt(i)){
                        graph.addArtPt(i);
                    }
                }
                
                else if (curr!=graph.getParent(i)){//next ja foi visitado
                    if(graph.getVisited(curr)<graph.getLow(i)){
                        graph.setLow(i, graph.getVisited(curr));
                    }
                }
            }

}

void dfs2(int i, int p, Graph &graph, std::vector<int> &subgraph){
    graph.setVisited(i, 1);   

    subgraph.push_back(i+1);

    for (int j = 0; j < graph.getLength(i); j++) 
    {//enqt ha um proximo
        int curr = graph.getAdj(i,j);

        if(graph.getVisited(curr)<0 && !graph.isArtPt(curr))
        {//next ainda n foi visitado
            dfs2(curr, i, graph, subgraph); 
        }
    }
}

int main()
{
    int numOfRouters = 0;
    int numOfCon = 0;

    int x;
    int y;

    scanf("%d\n%d", &numOfRouters, &numOfCon);
    Graph graph(numOfRouters);

    for(int i = 0; i < numOfCon; i++)
    {
        scanf("%d %d", &x, &y);
        x= x-1;
        y= y-1;
        graph.addEdge(x, y);
    }

    std::vector<int> subgraph;
    std::vector<int> r;
    int count = 0;
    int maxSubgraph = 0, currSubgraph = 0;

    for(int i = 0; i < numOfRouters; i++)
    {
        if(graph.getVisited(i)<0)
        {
            dfs(i, -1, graph, subgraph);
            r.push_back(*max_element(subgraph.begin(),subgraph.end()));
            subgraph.clear();
            subgraph.shrink_to_fit();
            ++count;
        }
    }
    
    graph.clearVisited();

    for(int i = 0; i < numOfRouters; i++)
    {
        if(graph.getVisited(i)<0 && !graph.isArtPt(i))
        {
            dfs2(i, -1, graph, subgraph);
            currSubgraph = subgraph.size();
            if(currSubgraph>maxSubgraph)
                maxSubgraph = currSubgraph;
            subgraph.clear();
            subgraph.shrink_to_fit();
        }
    }

    std::sort(r.begin(), r.end());
    printf("%d\n", count);
    printf("%d", r[0]);
    for(int i = 1; i < count; i++)
        printf(" %d", r[i]);
    printf("\n%d\n%d\n", graph.getArtPtsSize(), maxSubgraph);

    return 0;
}