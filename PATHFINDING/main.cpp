#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <utility> // for pair
using namespace std;
// Function to represent the matrix as a graph

void matrixToGraph(const vector<vector<int>>& matrix){
    int numVertices = matrix.size();
    cout << "Matrix: " << endl;
    for(int i = 0; i < numVertices; i++){
        for(int j = 0; j < numVertices; j++){
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}
vector<pair<int, int>> bfs(const vector<vector<int>>& matrix, int startRow, int startCol, int goal){
    int numVertices = matrix.size();
    vector<vector<bool>> visited(numVertices, vector<bool>(numVertices, false));
    vector<vector<pair<int, int>>> parent(numVertices, vector<pair<int, int>>(numVertices, {-1, -1}));
    queue<pair<int, int>> q;
    q.push({startRow, startCol});
    visited[startRow][startCol] = true;
    
    while(!q.empty()){
        auto current = q.front();
        int currentRow = current.first;
        int currentCol = current.second;
        q.pop();
        if(matrix[currentRow][currentCol] == goal){
            vector<pair<int, int>> path;
            while(currentRow != -1 && currentCol != -1){
                path.push_back({currentRow, currentCol});
                current = parent[currentRow][currentCol];
                currentRow = current.first;
                currentCol = current.second;
            }
            return path;
        }
        
        int dr[] = {0, 1, 0, -1};
        int dc[] = {1, 0, -1, 0};
        for(int i = 0; i < 4; i++){
            int newRow = currentRow + dr[i];
            int newCol = currentCol + dc[i];
            if(newRow > 0 && newRow < numVertices && newCol >= 0 && newCol < numVertices && matrix[newRow][newCol] != 1 && !visited[newRow][newCol]){
                q.push({newRow, newCol});
                visited[newRow][newCol] = true;
                parent[newRow][newCol] = {currentRow, currentCol};
            }
        }
    }
    return {};
}   

int main() {
    const int MATRIX_SIZE = 8;
    const int GOAL_VALUE = 2;

    ifstream inFile("matrix.txt");

    if (!inFile) {
        cerr << "Unable to open file!";
        return 1;
    }

    vector<vector<int>> matrix(MATRIX_SIZE, vector<int>(MATRIX_SIZE));
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            inFile >> matrix[i][j];
        }
    }

    inFile.close();

    // Call the function to represent the matrix as a graph
    matrixToGraph(matrix);

    // Find the shortest path using BFS
    vector<pair<int, int>> path = bfs(matrix, 0, 0, GOAL_VALUE);
    int step = 0; 
    // Output the path
    cout << "Path to goal" << endl;
    if (!path.empty()) {
        for (int i = path.size() - 1; i >= 0; --i) {
            cout << "matrix[" << path[i].first << "][" << path[i].second << "]";
            if (i != 0)
                cout << " -> ";
            step ++;
        }
        cout << endl;
    } else {
        cout << "No path found to the goal." << endl;
    }
    cout << "Steps take: " << step << endl;


    return 0;
}