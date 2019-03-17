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
    private static int currLow = 0;
    private static int maxLow = 0;

    public static void dfs(int i, int p, Graph graph, int time, ArrayList<Integer> subgraph) {
        if(graph.getVisited(i)<0){  
            graph.setVisited(i, time);
            graph.setLow(i, time);     

            subgraph.add(i+1);

            int nIndChildren = 0;

            System.out.println("running dfs for " + i + " whose parent is " + p);

            for (int j = 0; j < graph.getLength(i); j++) {//enqt ha um proximo
                int curr = graph.getAdj(i,j);

                System.out.println("finding adjacency no." + j + " for vertex " + i);

                if(graph.getVisited(curr)<0){//next ainda n foi visitado
                    nIndChildren++;

                    System.out.println("setting vertex " + curr + "s parent as " + i);
                    graph.setParent(curr, i); 

                    System.out.println("adjacency no." + j + " for vertex " + i + " had not been visited yet");
                    
                    dfs(curr, i, graph, ++time, subgraph);

                    //temos que estar no atual e ver o filho

                    if(graph.getLow(curr)<graph.getLow(i)){
                        graph.setLow(i, graph.getLow(curr));
                        currLow++;
                        System.out.println("1. setting low for " + i + " as " + graph.getLow(curr) + " by son " + curr);
                    }

                    if(graph.getVisited(i)<=graph.getLow(curr) && p>=0 && !graph.isArtPt(i)){
                        System.out.println("" + i + " isnt root and is now ArtPt");
                        graph.addArtPt(i);
                        graph.setLow(curr, graph.getLow(i));
                        currLow++;
                    }

                    if(nIndChildren >= 2 && p<0 && !graph.isArtPt(i)){
                        System.out.println("" + i + " is root and is now ArtPt");
                        graph.addArtPt(i);
                    }
                }
                
                else if (curr!=graph.getParent(i)){//next ja foi visitado
                    System.out.println("adjacency no." + j + " for vertex " + i + " had been visited and isnt its parent");
                    if(graph.getLow(curr)<graph.getLow(i)){
                        System.out.println("2. setting low for " + i + " as " + graph.getLow(curr) + " by son " + curr );
                        graph.setLow(i, graph.getLow(curr));
                        currLow++;
                    }
                }
                else
                    System.out.println("adjacency no." + j + " for vertex " + i + " had been visited and is its parent");
            }

            if(currLow > maxLow)
                maxLow = currLow;
            currLow = 0;
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

            ArrayList<Integer> subgraph = new ArrayList<Integer>(); //para colocar os elementos de cada subrede
            ArrayList<Integer> r = new ArrayList<Integer>(); //R (maiores indentificadores de cada subrede)
            int count = 0;

            for(int i = 0; i < numOfRouters; i++) {
                if(graph.getVisited(i)<0) {
                    dfs(i, -1, graph, 0, subgraph);
                    r.add(Collections.max(subgraph));
                    subgraph = new ArrayList<Integer>();
                    ++count;
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


            log.write("" + maxLow + "\n");
            log.close();
            
        }
        catch (IOException e){}
        //catch (FileNotFoundException f){}
        

    }
    
}