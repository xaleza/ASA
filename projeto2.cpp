#include <vector>  //for std::vector
#include <list> 
#include <iterator>
#include <algorithm>
#include <set>
using namespace std; 

struct Vertex 
{ 
	int h, pre_flow; 

	Vertex(int h, int pre_flow) 
	{ 
		this->h = h; 
		this->pre_flow = pre_flow; 
	} 
};

struct Edge 
{ 	
    int u, v; 
	int cap;
    int flow; 

	Edge(int u, int v, int cap, int flow) 
	{ 
		this->u = u; 
		this->v = v; 
		this->cap = cap; 
        this->flow = flow; 		
	} 
}; 

class Graph
{
private:
    /* data */
    int numVertices;
    std::vector<Vertex> vertices;
    vector<Edge> edges;

    // Function to push excess flow from u 
	bool push(int u); 

	// Function to relabel a vertex u 
	void relabel(int u); 

	// This function is called to initialize 
	// preflow 
	void preflow(); 

	// Function to reverse edge 
	void updateReverseEdgeFlow(int i, int flow); 
   
public:
    Graph(int vertices);
    
    void addEdge(int u, int v, int cap);

    int getMaxFlow();
    //~Graph();
};
    
Graph::Graph(int numVertices)
{
    this->numVertices = numVertices;

    for(int i = 0; i < numVertices; i++)
        vertices.push_back(Vertex(0,0));
}

void Graph::addEdge(int u, int v, int cap)
{
    edges.push_back(Edge(u, v, cap, 0));
}

void Graph::preflow()
{
    int source = 0;

    vertices[source].h = vertices.size();


    for (int i = 0; i < edges.size(); i++)
    {
        if(edges[i].u == source)
        {
            edges[i].flow = edges[i].cap;

            vertices[edges[i].v].pre_flow += edges[i].flow;

			edges.push_back(Edge(edges[i].v, source, 0, -edges[i].flow)); 
        }
    }
}

//retorna o index do vertice com overflow
int oFVertex(vector<Vertex>& vertices) 
{ 
	for (int i = 1; i < vertices.size() - 1; i++) 
	if (vertices[i].pre_flow > 0) 
			return i; 
    //se nao encontrar nenhum
	return -1; 
} 

// Update reverse flow for flow added on ith Edge 
void Graph::updateReverseEdgeFlow(int i, int flow) 
{ 
	int u = edges[i].v, v = edges[i].u; 

	for (int j = 0; j < edges.size(); j++) 
	{ 
		if (edges[j].v == v && edges[j].u == u) 
		{ 
			edges[j].flow -= flow; 
			return; 
		} 
	} 

	// adding reverse Edge in residual graph 
	Edge e = Edge(u, v, flow, 0); 
	edges.push_back(e); 
} 

// To push flow from overflowing vertex u 
bool Graph::push(int u) 
{ 
	// Traverse through all edges to find an adjacent (of u) 
	// to which flow can be pushed 
	for (int i = 0; i < edges.size(); i++) 
	{ 
		// Checks u of current edge is same as given 
		// overflowing vertex 
		if (edges[i].u == u) 
		{ 
			// if flow is equal to capacity then no push 
			// is possible 
			if (edges[i].flow == edges[i].cap) 
				continue; 

			// Push is only possible if height of adjacent 
			// is smaller than height of overflowing vertex 
			if (vertices[u].h > vertices[edges[i].v].h) 
			{ 
				// Flow to be pushed is equal to minimum of 
				// remaining flow on edge and excess flow. 
				int flow = min(edges[i].cap - edges[i].flow, 
							vertices[u].pre_flow); 

				// Reduce excess flow for overflowing vertex 
				vertices[u].pre_flow -= flow; 

				// Increase excess flow for adjacent 
				vertices[edges[i].v].pre_flow += flow; 

				// Add residual flow (With capacity 0 and negative 
				// flow) 
				edges[i].flow += flow; 

				updateReverseEdgeFlow(i, flow); 

				return true; 
			} 
		} 
	} 
	return false; 
} 

// function to relabel vertex u 
void Graph::relabel(int u) 
{ 
	// Initialize minimum height of an adjacent 
	int mh = INT_MAX; 

	// Find the adjacent with minimum height 
	for (int i = 0; i < edges.size(); i++) 
	{ 
		if (edges[i].u == u) 
		{ 
			// if flow is equal to capacity then no 
			// relabeling 
			if (edges[i].flow == edges[i].cap) 
				continue; 

			// Update minimum height 
			if (vertices[edges[i].v].h < mh) 
			{ 
				mh = vertices[edges[i].v].h; 

				// updating height of u 
				vertices[u].h = mh + 1; 
			} 
		} 
	} 
} 

// main function for printing maximum flow of graph 
int Graph::getMaxFlow() 
{ 
	preflow();

	// loop untill none of the Vertex is in overflow 
	while (oFVertex(vertices) != -1) 
	{  
		int u = oFVertex(vertices); 
		if (!push(u)) 
			relabel(u); 
	} 

	// ver.back() returns last Vertex, whose 
	// e_flow will be final maximum flow 
	return vertices.back().pre_flow; 
} 

int main()
{
    int x;
    int y;

    int f = 0; //numero de fornecedores
    int e = 0; //numero de estacoes de abastecimento
    int t = 0; //numero de ligacoes
    int vertices = 0;
    int cap;
    
    
    scanf("%d %d %d", &f, &e, &t);

    int source = 0; //vertice source
    //source-fornecedores- 2*estacoes - hiper
    vertices = 1 + f + 2*e + 1;
    

    Graph graph(vertices);

    //liga o vertice source aos fornecedores
    for(int i = 1; i < f+1; i++)
    {
        scanf("%d", &cap);
    
        graph.addEdge(source, i, cap);
        
    }
   
    //transforma os pts de abastecimento em dois vertices
    for(int i = f+1; i < f+1+2*e; i+=2)
    {
        scanf("%d", &cap);

        graph.addEdge(i, i+1, cap);
    }

    //trata das ligacoes
    for(int i = 0; i < t; i++)
    {
        scanf("%d %d %d", &x, &y, &cap);
        
        //se for uma ligacao a partir de um abastecimento, incrementar para ficar no vertice certo
        x--;
        if(y == 1)
            //hiper no fim da lista
            y = f+2*e+1;
        else
        {
            y--;
        }
        
        if(x > f)
        {
            x++;
        }
        graph.addEdge(x, y, cap);
    }

    printf("%d\n", graph.getMaxFlow());

    return 0;
}