import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.StreamTokenizer;
import java.io.InputStreamReader;
import java.util.*;

public class Projeto{

    public static void dfs(int i, int[][] graph, boolean[] visited, ArrayList<Integer> subgraph) {
        if(!visited[i]){        
            visited[i] = true; // Mark node as "visited"
            subgraph.add(i+1);
    
            for (int j = 0; j < graph[i].length; j++) {
                if (graph[i][j]==1 && !visited[j]) {   
                    dfs(j, graph, visited, subgraph); // Visit node
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
            int[][] matrix = new int[numOfRouters][numOfRouters];
            st.nextToken();
            numOfCon = (int) st.nval;
            
            for(int i = 0; i < numOfCon; i++){
                st.nextToken();
                x = (int) st.nval -1;
                st.nextToken();
                y = (int) st.nval -1;
                matrix[x][y]= 1;
                matrix[y][x]= 1;
            }
            System.out.println(numOfRouters);
            System.out.println(numOfCon);

            for(int i = 0; i< numOfRouters; i++){
                for(int j = 0; j<numOfRouters; j++)
                    System.out.print(matrix[i][j]);
                System.out.println();
            }
            
            boolean [] visited = new boolean[numOfRouters];
            ArrayList<Integer> subgraph = new ArrayList<Integer>(); //para colocar os elementos de cada subrede
            ArrayList<Integer> r = new ArrayList<Integer>(); //R (maiores indentificadores de cada subrede)
            int count = 0;
            for(int i = 0; i < numOfRouters; i++) {
                if(!visited[i]) {
                    dfs(i,matrix,visited,subgraph);
                    r.add(Collections.max(subgraph));
                    subgraph = new ArrayList<Integer>();
                    ++count;
                }
            }
            System.out.println(count);
            for(int i = 0; i < count; i++)
                System.out.print(r.get(i) + " ");
            System.out.println();
            
        }
        catch (IOException e){}
        //catch (FileNotFoundException f){}
        

    }
    
}