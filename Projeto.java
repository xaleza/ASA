import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.StreamTokenizer;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.util.*;


class Graph{
    private int numVertices;
    private ArrayList<Integer> artPts = new ArrayList<Integer>(); //articulation pts
    private List<LinkedList<Integer>> adjLists;
    private int [] visited;
    private int [] parent;
    private int [] low;

    Graph(int vertices)
    {
        numVertices = vertices;
        adjLists = new ArrayList<LinkedList<Integer>>();
        visited = new int[vertices];
        parent = new int[vertices];
        low = new int[vertices];

        Arrays.fill(visited, -1);
    
        for (int i = 0; i < vertices; i++)
            adjLists.add(new LinkedList<Integer>());
    }

    void addEdge(int src, int dest)
    {
        adjLists.get(src).add(dest);
        adjLists.get(dest).add(src);
    }

    int getLength(int i){
        return adjLists.get(i).size();
    }

    int getAdj(int i, int j){
        return adjLists.get(i).get(j);
    }

    void addArtPt(int i){
        artPts.add(i);
    }

    int getVisited(int i){
        return visited[i];
    }

    int getParent(int i){
        return parent[i];
    }

    int getLow(int i){
        return low[i];
    }

    void setLow(int v, int l){
        low[v] = l;
    }

    void setVisited(int v, int t){
        visited[v] = t;
    }

    void setParent(int v, int p){
        parent[v] = p;
    }

    int getArtPtsSize(){
        return artPts.size();
    }

    boolean isArtPt(int i){
        return artPts.contains(i);
    }

    int getArtPt(int i){
        return artPts.get(i);
    }
}

public class Projeto{
    private static int nIndChildren = 0;

    public static void dfs(int i, int p, Graph graph, int time, ArrayList<Integer> subgraph) {
        if(graph.getVisited(i)<0){  
            graph.setVisited(i, time);
            graph.setLow(i, time);
            graph.setParent(i, p);      

            subgraph.add(i+1);

            if(p>=0 && graph.getParent(p)==-1)
                nIndChildren++;

            for (int j = 0; j < graph.getLength(i); j++) {
                int curr = graph.getAdj(i,j);

                if(curr!=p){
                    if (graph.getVisited(curr)<0) {  //next ainda n foi visitado 
                        dfs(curr, i, graph, ++time, subgraph);
                    }
                    else{//next ja foi visitado
                        int adj = curr;
                        int prnt = i;

                        traceback(prnt, adj, graph);
                    }
                }
            }
            traceback(p, i, graph);
            
        }

    }

    public static void traceback(int prnt, int child, Graph graph){

        while(prnt >=0){
            if(graph.getLow(child)<graph.getLow(prnt)){
                graph.setLow(prnt, graph.getLow(child));
            }

            if(graph.getVisited(prnt)<=graph.getLow(child) && 
            !graph.isArtPt(prnt) && graph.getParent(prnt)>=0){
                graph.addArtPt(prnt);
                return;
            }

            child = prnt;
            prnt = graph.getParent(prnt);
        }

        if(nIndChildren >= 2 && !graph.isArtPt(child)){
            graph.addArtPt(child);
        }
        
    }

    public static int getMaxSubgraphSize(int i, Graph graph){
        int max = -1;
        for (int j = 0; j < graph.getLength(i); j++){
            int curr = graph.getAdj(i,j);
            if(curr!=graph.getParent(i) && !graph.isArtPt(curr) && graph.getLength(curr)>max)
                max = graph.getLength(curr);
        }
        return max;
    }


    public static void main(String[] args){
        
        int numOfRouters = 0;
        int numOfCon = 0; //numero de conexoes
        
        int x; //var aux da matriz
        int y;

        StreamTokenizer st = new StreamTokenizer(new BufferedReader(new InputStreamReader(System.in)));
        st.parseNumbers();

        try{
            st.nextToken();
            numOfRouters = (int) st.nval;
            Graph graph = new Graph(numOfRouters);
            st.nextToken();
            numOfCon = (int) st.nval;
            
            for(int i = 0; i < numOfCon; i++){
                st.nextToken();
                x = (int) st.nval-1;
                st.nextToken();
                y = (int) st.nval-1;
                graph.addEdge(x, y);
            }

            ArrayList<Integer> subgraph = new ArrayList<Integer>(); //para colocar os elementos de cada subrede
            ArrayList<Integer> r = new ArrayList<Integer>(); //R (maiores indentificadores de cada subrede)
            int count = 0;

            for(int i = 0; i < numOfRouters; i++) {
                if(graph.getVisited(i)<0) {
                    dfs(i, -1, graph, 0, subgraph);
                    r.add(Collections.max(subgraph));
                    subgraph = new ArrayList<Integer>();
                    ++count;
                    nIndChildren = 0;
                }
            }
            //OUTPUT
            BufferedWriter log = new BufferedWriter(new OutputStreamWriter(System.out));
            Collections.sort(r);
            log.write(count + "\n");


            log.write(""+ r.get(0));
            for(int i = 1; i < count; i++)
                log.write(" " + r.get(i));
            
            log.write("\n");
            log.write(graph.getArtPtsSize()+ "\n");

            int maxArtPt = -1;
            int currArtPt;

            for(int i=0; i<graph.getArtPtsSize(); i++){
                currArtPt = graph.getArtPt(i);
                if(getMaxSubgraphSize(currArtPt, graph)>maxArtPt)
                    maxArtPt = getMaxSubgraphSize(currArtPt, graph);
            }

            log.write("" + maxArtPt + "\n");
            log.close();
            
        }
        catch (IOException e){}
        //catch (FileNotFoundException f){}
        

    }
    
}