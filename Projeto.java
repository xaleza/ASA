import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.*;

public class Projeto{

    public static void dfs(int i, int[][] graph, boolean[] visited, ArrayList<Integer> nodes) {
        if(!visited[i]){        
            visited[i] = true; // Mark node as "visited"
            nodes.add(i+1);
    
            for (int j = 0; j < graph[i].length; j++) {
                if (graph[i][j]==1 && !visited[j]) {   
                    dfs(j, graph, visited, nodes); // Visit node
                }
            }
        }   
    }
    public static void main(String[] args){
        
        String filename = args[0];
        int numOfRouters = 0;
        int numOfCon = 0;
        int x;
        int y;
        try{
            BufferedReader in = new BufferedReader(new FileReader(filename));
            String s = new String();

            numOfRouters = Integer.parseInt(in.readLine());
            numOfCon = Integer.parseInt(in.readLine());
            int[][] matrix = new int[numOfRouters][numOfRouters];

            while((s = in.readLine()) != null){
                String[] fields = s.split(" ");
                x = Integer.parseInt(fields[0]) - 1;
                y = Integer.parseInt(fields[1]) - 1;
                matrix[x][y] = 1;
                matrix[y][x] = 1;
            }

            boolean [] visited = new boolean[numOfRouters];
            ArrayList<Integer> nodes = new ArrayList<Integer>();
            ArrayList<Integer> r = new ArrayList<Integer>();
            int count = 0;
            for(int i = 0; i < numOfRouters; i++) {
                if(!visited[i]) {
                    dfs(i,matrix,visited,nodes);
                    r.add(Collections.max(nodes));
                    nodes = new ArrayList<Integer>();
                    ++count;
                }
            }
            System.out.println(count);
            for(int i = 0; i < count; i++)
                System.out.print(r.get(i) + " ");
            System.out.println();
            


            // System.out.println("numOfRouters = " + numOfRouters);
            // System.out.println("numOfCon = " + numOfCon);
            // for(int i = 0; i < numOfRouters; i++){
            //     for(int j = 0; j < numOfRouters; j++){
            //         d = matrix[i][j];
            //         System.out.print(d +" ");
            //     }
            //     System.out.print("\n");
            // }
        }
        catch (IOException e){}
        //catch (FileNotFoundException f){}
        

    }
    
}