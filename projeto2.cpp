#include <vector>  //for std::vector
#include <unistd.h>
#include <queue>
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
	
	Edge(int cap, int cf)
	{
		this->cap = cap;
		this->cf = cf;
	}
};

struct Neighbour
{
	int vertex;
	int edgeIndex;
	bool forward;
	Neighbour(int vertex, int edgeIndex, bool forward)
	{
		this->vertex = vertex;
		this->edgeIndex = edgeIndex;
		this->forward = forward;
	}
};

class Graph
{
private:
	/* data */
	int numVertices, numStations, numSuppliers;
	std::vector<Vertex> vertices;
	std::vector<vector<int> > stations;
	std::vector<vector<Neighbour> > neighbours;
	std::vector<Edge> edges;
	std::vector<int> L; 

public:
	Graph(int v, int e, int f)
	{
		numVertices = v;
		numStations = e;
		numSuppliers = f;
		for(int i=0; i < numVertices; i++)
			vertices.push_back(Vertex(0, 0));
		neighbours.resize(numVertices);
		L.resize(numVertices-2);
		stations.resize(e);
	}
	//~Graph();

	void addEdge(int u, int v, int cap)
	{
		edges.push_back(Edge(cap,cap));

		neighbours[u].push_back(Neighbour(v,edges.size()-1,false));
		neighbours[v].push_back(Neighbour(u,edges.size()-1,true));
	}

	void addStation(int i, int inc, int j)
	{
		stations[j].push_back(i);
		stations[j].push_back(inc);
	}

	int getX(int pos)
	{
		return stations[pos][1];
	}
	int getY(int pos)
	{
		return stations[pos][0];
	}

	int getEdgeIndex(int u, int index)
	{
		return neighbours[u][index].edgeIndex;
	}

	int getNeighbourVertex(int u, int index)
	{
		return neighbours[u][index].vertex;
	}

	int getLength(int i)
	{
		return neighbours[i].size();
	}

	int getNeigh(int i, int j)
	{
		return neighbours[i][j].vertex;
	}

	int getHeight(int i)
	{
		return vertices[i].h;
	}

	int getPreflow(int i )
	{
		return vertices[i].pre_flow;
	}

	int getCap(int i)
	{
		return edges[i].cap;
	}

	int getCf(int i){
		return edges[i].cf;
	}

	bool isEven(int i)
	{
		return i%2==0;
	}

	bool isStationEntry(int pos)
	{
		if(isEven(numSuppliers))
		{
			if(pos > numSuppliers && pos != numVertices-1 && !isEven(pos))
				return true;
		}
		else if(pos > numSuppliers && pos != numVertices-1 && isEven(pos))
		{
			return true;
		}
		return false;
	}

	void preflow()
	{
		int s = numVertices-1;
		vertices[s].h = numVertices;
		for(int i = 0; i < getLength(s); i++)
		{
			vertices[getNeighbourVertex(s,i)].pre_flow = edges[getEdgeIndex(s, i)].cap;
			vertices[s].pre_flow -= edges[getEdgeIndex(s, i)].cap;
			edges[getEdgeIndex(s,i)].cf =0;
		}
	}

	void relabel(int u)
	{
		int min = 9999;
		for(int i = 0; i < getLength(u); i++)
		{
			if(vertices[neighbours[u][i].vertex].h < min && (getCf(getEdgeIndex(u,i)) > 0 || !neighbours[u][i].forward))
				min = vertices[neighbours[u][i].vertex].h;
		}
		vertices[u].h = min +1;
	}

	void push(int u, int index)
	{
		int flow;
		int edgeIndex= getEdgeIndex(u, index);
		
		if(neighbours[u][index].forward)
		{
			flow = getCf(edgeIndex) ^ ((getPreflow(u) ^ getCf(edgeIndex)) & -(getPreflow(u) < getCf(edgeIndex)));
			edges[edgeIndex].cf -= flow;
		}
		else
		{
			//minimum
			flow = getCap(edgeIndex) ^ ((getPreflow(u) ^ getCap(edgeIndex)) & -(getPreflow(u) < getCap(edgeIndex)));
			edges[edgeIndex].cf += flow;

		}
		
		vertices[u].pre_flow -= flow;
		vertices[neighbours[u][index].vertex].pre_flow += flow;
	}

	void discharge(int u)
	{
		int index=0, v;
		while(getPreflow(u)>0)
		{
			//chegou ao fim da lista de vizinhos
			if(index == getLength(u)){
				relabel(u);
				index = 0;
			}
			else
			{
				v = neighbours[u][index].vertex;
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

		return vertices[0].pre_flow;
	}

	void minimumCut()
	{
		std::vector<bool> visited;
		std::vector<int> lastVertices;
		std::vector<int> lastStations;
		visited.resize(numVertices);
		bool hasVisitedCh = false;

		queue <int> queue;
		queue.push(numVertices-1);
		visited[numVertices-1] = true;

		while(!queue.empty())
		{	
			hasVisitedCh = false;
			int curr = queue.front();
			queue.pop();
			int len = getLength(curr);

			for(int i =0; i < len; i++)
			{
				if(!visited[i] && getCf(getEdgeIndex(curr, i))>0)
				{
					queue.push(i);
					visited[i] = true;
					hasVisitedCh= true;
				}
			}
			if(!hasVisitedCh)
			{
				if(!isStationEntry(curr))
					lastStations.push_back(curr);
				else
					lastVertices.push_back(curr);
			}
			std::sort(lastStations.begin(), lastStations.end());
			
			if(lastStations.size()>0)
				printf("%d", lastStations[0]);
	
			for(int i = 1; i < lastStations.size(); i++)
				printf(" %d", lastStations[i]);

			printf("\n");
			
		}

	}

	void cut(){

		std::vector<bool> over;
		over.resize(numVertices);
		std::vector<int> cutAbast;

		for(int i=0; i<numVertices; i++){
			over[i]=false;
		}

		for(int i=1; i<numVertices-1; i++){
			if(vertices[i].h > numVertices)
				over[i]=true;
		}

		int inV;
		int outV;

		for(int i=0; i<numStations; i++){
			inV = stations[i][0];
			outV = stations[i][1];

			if(over[outV] && !over[inV]){//se o unico q esta por cima e' o vertice de
				cutAbast.push_back(inV);//abastecimento do lado direito aka o corte passa no abastecimento
			}
		}

		if(cutAbast.size()>0){
			printf("%d", cutAbast[0]);
		}

		for(int i=1; i<cutAbast.size(); i++){
			printf(" %d", cutAbast[i]);
		}

		printf("\n");

		for(int i = 1; i<numVertices-1; i++){
			//ver se o i esta de um lado e o vizinho esta do outro
			//mas so se de i para o vizinho for forward
			// dar print de i (origem) e vizinho (destino) \n
		}

	}	

	void print()
	{
		for(int i = 0; i < numVertices; i++)
		{
	// 	printf("u = %d",i);
	 	for(int j = 0; j < getLength(i);j++)
	 		//printf("dest = %d, cap = %d; ", neighbours[i][j], getCap(i,j));
	// 	printf("\n");
	// 	}
	// 			//printf("u = %d, v = %d, h = %d, cf = %d \n",i, neighbours[i][j],)
	 		printf("i=%d, j= %d, preflux = %d, height = %d\n",i,getNeigh(i,j), vertices[getNeigh(i,j)].pre_flow, vertices[getNeigh(i,j)].h);
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
	

	Graph graph(vertices, e, f);

	//liga o vertice source aos fornecedores
	for(int i = 1; i < f+1; i++)
	{
		scanf("%d", &cap);

		graph.addEdge(source, i, cap);
		
	}
	int j = 0;
	//transforma os pts de abastecimento em dois vertices
	for(int i = f+1; i < f+1+2*e; i+=2)
	{	

		scanf("%d", &cap);
		graph.addEdge(i, i+1, cap);
		graph.addStation(i, i+1, j);		
		j++;
	}

	//trata das ligacoes
	for(int i = 0; i < t; i++)
	{
		scanf("%d %d %d", &x, &y, &cap);
		
		//se for uma ligacao a partir de um abastecimento, incrementar para ficar no vertice certo
		x--;
		y--;
		if(y == 0)
			//hiper no fim da lista
			y = f+2*e+1;
		else if(y > f)
			y = graph.getY(y-f-1);
		

		if(x> f)
			x = graph.getX(x-f-1);
		
		graph.addEdge(x, y, cap);
	}
	//graph.print();
	printf("%d\n", graph.relabelToFront());
	graph.cut();

	//graph.print();
	//graph.minimumCut();
	return 0;
}