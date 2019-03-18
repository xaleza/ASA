#include <vector>  //for std::vector
#include <list> 
#include <iterator>
#include <algorithm>
using namespace std; 

struct Node
{ 
    int data; 
    struct Node *next; 
}; 

class linkedList
{
    private:
    Node *head, *tail;
    int length;
    public:
    linkedList()
    {
        length = 0;
      head=NULL;
      tail=NULL;
    }

    void createNode(int value)
    {
        length++;
        Node *temp=new Node;
        temp->data=value;
        temp->next=NULL;
        if(head==NULL)
        {
            head=temp;
            tail=temp;
            temp=NULL;
        }
        else
        {	
            tail->next=temp;
            tail=temp;
        }
    }

    int getValue(int pos)
    {
        Node *temp = new Node;
        int i = 0;
        temp = head;
        for(i=0; i<pos; i++)
        {
            temp = temp->next;
        }
        return temp->data;
    }

    int getLength()
    {
        return length;
    }
  };



class Graph
{
private:
    /* data */
    int numVertices;
    std::vector<int> artPts;
    std::vector<linkedList> adjLists;
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
        adjLists[src].createNode(dest);
        adjLists[dest].createNode(src);
    }

    int getLength(int i)
    {
        return adjLists[i].getLength();
    }

    int getAdj(int i, int j)
    {
        return adjLists[i].getValue(j);
    }

    void addArtPt(int i)
    {
        artPts.push_back(i);
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
        return (std::find(artPts.begin(), artPts.end(), i) != artPts.end());
    }

    int getArtPt(int i)
    {
        return artPts[i];
    }
};

int currLow = 0;
int maxLow = 0;

void dfs(int i, int p, Graph &graph, int time, std::vector<int> &subgraph)
{
    if(graph.getVisited(i)<0)
    {  
            graph.setVisited(i, time);
            graph.setLow(i, time);     

            subgraph.push_back(i+1);

            int nIndChildren = 0;

            for (int j = 0; j < graph.getLength(i); j++) 
            {//enqt ha um proximo
                int curr = graph.getAdj(i,j);

                if(graph.getVisited(curr)<0)
                {//next ainda n foi visitado
                    nIndChildren++;

                    graph.setParent(curr, i); 
                    
                    dfs(curr, i, graph, ++time, subgraph);

                    //temos que estar no atual e ver o filho

                    if(graph.getLow(curr)<graph.getLow(i))
                    {
                        graph.setLow(i, graph.getLow(curr));
                        currLow++;
                    }

                    if(graph.getVisited(i)<=graph.getLow(curr) && p>=0 && !graph.isArtPt(i)){
                        graph.addArtPt(i);
                        graph.setLow(curr, graph.getLow(i));
                        currLow++;
                    }

                    if(nIndChildren >= 2 && p<0 && !graph.isArtPt(i)){
                        graph.addArtPt(i);
                    }
                }
                
                else if (curr!=graph.getParent(i)){//next ja foi visitado
                    if(graph.getLow(curr)<graph.getLow(i)){
                        graph.setLow(i, graph.getLow(curr));
                        currLow++;
                    }
                }
            }

            if(currLow > maxLow)
                maxLow = currLow;
            currLow = 0;
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

    for(int i = 0; i < numOfRouters; i++)
    {
        if(graph.getVisited(i)<0)
        {
            dfs(i, -1, graph, 0, subgraph);
            r.push_back(*max_element(subgraph.begin(),subgraph.end()));
            subgraph.clear();
            ++count;
        }
    }

    printf("%d\n", count);
    printf("%d", r[0]);
    for(int i = 1; i < count; i++)
        printf(" %d", r[i]);
    printf("\n%d\n%d\n", graph.getArtPtsSize(), maxLow);

    return 0;
}


