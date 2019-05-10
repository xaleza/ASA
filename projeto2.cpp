#include <vector>  //for std::vector
#include <unistd.h>
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
	
	void addPreflow(int pre_flow){
		this->pre_flow += pre_flow;
	}
};

struct Edge
{
	int cap, cf; //capacidade e capacidade restante
	bool forward;
	
	Edge(int cap, int cf, bool forward)
	{
		this->cap = cap;
		this->cf = cf;
		this->forward = forward;
	}
};

class Graph
{
private:
	/* data */
	int numVertices;
	std::vector<Vertex> vertices;
	std::vector<vector<int> > neighbours;
	std::vector<vector<Edge> > edges;
	std::vector<int> L; 

public:
	Graph(int v)
	{
		numVertices = v;
		for(int i=0; i < numVertices; i++)
			vertices.push_back(Vertex(0, 0));
		neighbours.resize(numVertices);
		edges.resize(numVertices);
		L.resize(numVertices-2);
	}
	//~Graph();

	void addEdge(int u, int v, int cap)
	{
		neighbours[u].push_back(v);
		neighbours[v].push_back(u);
		
		edges[u].push_back(Edge(cap, cap, true));
		edges[v].push_back(Edge(cap, 0, false));
	}

	int getLength(int i)
	{
		return neighbours[i].size();
	}

	Vertex getNeigh(int i, int j)
	{
		return vertices[neighbours[i][j]];
	}

	int getHeight(int i)
	{
		return vertices[i].h;
	}

	int getPreflow(int i )
	{
		return vertices[i].pre_flow;
	}

	int getCap(int i, int j)
	{
		return edges[i][j].cap;
	}

	int getCf(int i, int j){
		return edges[i][j].cf;
	}

	void preflow()
	{
		int s = 0;
		vertices[s].h = numVertices;
		for(int i = 0; i < getLength(s); i++)
		{
			vertices[neighbours[s][i]].pre_flow = edges[s][i].cap;
			vertices[s].pre_flow -= edges[s][i].cap;
		}
	}

	void relabel(int u)
	{
		int min = 9999;
		for(int i = 0; i < getLength(u); i++)
		{
			if(vertices[neighbours[u][i]].h < min && (edges[u][i].cf > 0 || edges[u][i].forward == false))
				min = vertices[neighbours[u][i]].h;
		}
		vertices[u].h = min +1;
	}

	void push(int u, int index)
	{
		int flow;
		if(edges[u][index].forward)
		{
			flow = getCf(u, index) ^ ((getPreflow(u) ^ getCf(u, index)) & -(getPreflow(u) < getCf(u, index)));
			edges[u][index].cf -= flow;
		}
		else
		{
			//minimum
			flow = getCap(u, index) ^ ((getPreflow(u) ^ getCap(u, index)) & -(getPreflow(u) < getCap(u, index)));
			edges[u][index].cf += flow;
		}
		
		vertices[u].pre_flow -= flow;
		vertices[neighbours[u][index]].pre_flow += flow;
	}

	void discharge(int u)
	{
		int index=0, v;
		int i=0;
		while(getPreflow(u)>0)
		{
			//chegou ao fim da lista de vizinhos
			if(index == getLength(u)){
				relabel(u);
				index = 0;
			}
			else
			{
				v = neighbours[u][index];
				if(vertices[u].h == (vertices[v].h +1))
				{
					push(u, index);
				}
				index++;
			}
			
		}
	}

	int relabelToFront(){
		int index = 0;
		int u, oldh;
		preflow();
		
		for(int i = 0; i < numVertices - 2; i++)
			L[i] = i+1;
		
		u = L[0];
		while(index < numVertices-2)
		{	
			oldh = getHeight(u);
			discharge(u);
			if(vertices[u].h > oldh)
			{
				for(int i=index; i > 0; i--)
				{	
					L[i]= L[i-1];
				}
				L[0] = u;
				index = 0;
			}
			index++;
			u = L[index];
		}

		return vertices[numVertices-1].pre_flow;
	}

	void print()
	{
		for(int i = 0; i < numVertices; i++)
		{
			printf("i=%d, preflux = %d, height = %d\n",i, vertices[i].pre_flow, vertices[i].h);
		}
	}
};


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

	printf("Max flow = %d\n", graph.relabelToFront());
	graph.print();
	return 0;
}