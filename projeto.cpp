#include <vector>  //for std::vector
#include <list> 
#include <iterator>
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



class graph
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
    graph(int vertices)
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
    ~graph();

    void addEdge(int src, int dest)
    {
        adjLists[src].createNode(dest);
        adjLists[dest].createNode(src);
    }

    int getLenght(int i)
    {
        return adjLists[i].getLength();
    }

    int getAdj(int i, int j)
    {
        return adjLists[i].getValue(j);
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


