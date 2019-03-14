import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.StreamTokenizer;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.util.*;


class Graph{
    private int numVertices;
    private LinkedList<Integer> adjLists[];

    Graph(int vertices)
    {
        numVertices = vertices;
        adjLists = new LinkedList[vertices];
    
        for (int i = 0; i < vertices; i++)
            adjLists[i] = new LinkedList();
    }

    void addEdge(int src, int dest)
    {
        adjLists[src].add(dest);
        adjLists[dest].add(src);
    }

    int getLenght(int i){
        return adjLists[i].size();
    }

    int getAdj(int i, int j){
        return adjLists[i].get(j);
    }
}

public class Projeto{


    public static void dfs(int i, Graph graph, boolean[] visited, ArrayList<Integer> subgraph) {
        if(!visited[i]){        
            visited[i] = true; // Mark node as "visited"
            subgraph.add(i+1);
            for (int j = 0; j < graph.getLenght(i); j++) {
                int curr = graph.getAdj(i,j);
                if (!visited[curr]) {   
                    dfs(curr, graph, visited, subgraph); // Visit node
                }
            }
        }   
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
            
            boolean [] visited = new boolean[numOfRouters];
            ArrayList<Integer> subgraph = new ArrayList<Integer>(); //para colocar os elementos de cada subrede
            ArrayList<Integer> r = new ArrayList<Integer>(); //R (maiores indentificadores de cada subrede)
            int count = 0;
            for(int i = 0; i < numOfRouters; i++) {
                if(!visited[i]) {
                    dfs(i,graph,visited,subgraph);
                    
                    r.add(Collections.max(subgraph));
                    subgraph = new ArrayList<Integer>();
                    ++count;
                }
            }
            //OUTPUT
            BufferedWriter log = new BufferedWriter(new OutputStreamWriter(System.out));
            log.write(count + "\n");
            for(int i = 0; i < count; i++)
                log.write(r.get(i) + " ");
            log.write("\n");
            log.close();
            
        }
        catch (IOException e){}
        //catch (FileNotFoundException f){}
        

    }
    
}