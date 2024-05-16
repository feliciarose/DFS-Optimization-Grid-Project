/*
Felicia Drysdale: Algorithms Project 3: Jumping Jim
Jumping Jim code traverses a weighted graph beginning at the top left corner of the trampolines Jim can jump on to try to
find the correct path to his goal (the bottom right corner). This code uses recursive DFS to complete the traversal.
This code uses the boost library to build adjacency list from the input matrix.
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>

using namespace std;
using namespace boost;

//Define the adj list using Boost
typedef adjacency_list<vecS, vecS, undirectedS,
    property<edge_weight_t, int> 
    > Graph;

//Directions
enum Direction { NORTH, SOUTH, EAST, WEST };

//Structure for vertex and direction
struct VertexDirection {
    Graph::vertex_descriptor vertex;
    Direction direction;
};

//DFS function to traverse jumping Jim's trampolines
bool dfs(Graph& g, Graph::vertex_descriptor curr_vertex, Graph::vertex_descriptor end_vertex, vector<vector<int>>& grid, vector<VertexDirection>& path, map<Graph::vertex_descriptor, bool>& visited) {

    //Mark the current vertex as visited
    visited[curr_vertex] = true;

    //Put curr vertex in path
    VertexDirection vertex_direc;
    vertex_direc.vertex = curr_vertex;

    //Check if the current vertex is the end vertex (goal)
    if (curr_vertex == end_vertex) {
        path.push_back(vertex_direc);
        return true;
    }

    //Get the row and column index of the current vertex
    int row = curr_vertex / grid.size();
    int col = curr_vertex % grid.size();

    //Directions we can travel
    vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    //Iterate over directions
    for (vector<pair<int, int>>::iterator i = directions.begin(); i != directions.end(); ++i) {
        pair<int, int>& direc = *i;
        int position_x = direc.first;
        int position_y = direc.second;

        //Calculate the next vertex index
        int next_x_index = row + position_x * grid[row][col];
        int next_y_index = col + position_y * grid[row][col];

        //Check if the next vertex is in the grid
        if (next_x_index >= 0 && next_x_index < grid.size() && next_y_index >= 0 && next_y_index < grid.size()) {

            Graph::vertex_descriptor next_vertex = next_x_index * grid.size() + next_y_index;

            //Check if visited
            if (!visited[next_vertex]) {

                //Store the direction in the path and put corresponding direction based on movw
                Direction direction;
                if (position_x == -1 && position_y == 0) direction = NORTH;
                else if (position_x == 1 && position_y == 0) direction = SOUTH;
                else if (position_x == 0 && position_y == -1) direction = WEST;
                else if (position_x == 0 && position_y == 1) direction = EAST;
                vertex_direc.direction = direction;
                path.push_back(vertex_direc);

                //Recursively explore next vertex
                if (dfs(g, next_vertex, end_vertex, grid, path, visited)) {
                    return true;
                }

                //Remove last vertex added to path if the path if we reach dead end / path is not found
                path.pop_back();
            }
        }
    }

    return false;
}

//Function to print the path 
void print_path(const vector<VertexDirection>& path, int grid_size, const string& output_file) {

    //Create output file stream
    ofstream outputFile(output_file);

    //Check if we can write to file
    if (outputFile.is_open()) {
        for (vector<VertexDirection>::const_iterator i = path.begin(); i != path.end() - 1; ++i) {
            const VertexDirection& vertex_direc = *i;
            int vertex = vertex_direc.vertex;
            int row = vertex / grid_size;
            int col = vertex % grid_size;

            //Print direction to the output file
            if (vertex_direc.direction == NORTH) {
                outputFile << "N ";
            } else if (vertex_direc.direction == SOUTH) {
                outputFile << "S ";
            } else if (vertex_direc.direction == EAST) {
                outputFile << "E ";
            } else if (vertex_direc.direction == WEST) {
                outputFile << "W ";
            }
        }
        outputFile << endl;
        outputFile.close();
    }
}


int main() {

    ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        cerr << "Cannot open input file." << endl;
        return 1;
    }

    //Get the dimensions of the grid
    int rows, cols;
    inputFile >> rows >> cols;
    inputFile.ignore();

    //Define the grid graph
    vector<vector<int>> grid(rows, vector<int>(cols));

    //Read grid data from the file
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            inputFile >> grid[i][j];
        }
    }

    inputFile.close();

    /*
    //Print dimensions to check (I'm keeping this here in case any issues occur!)
    cout << "Rows: " << rows << ", Cols: " << cols << endl;

    //Print grid to check
    for (vector<vector<int>>::const_iterator i = grid.begin(); i != grid.end(); ++i) {
        const vector<int>& row = *i;
        for (vector<int>::const_iterator k = row.begin(); k != row.end(); ++k) {
            int num = *k;
            cout << num << " ";
        }
        cout << endl;
    }
    */
    
    Graph g;

    //Define directions
    vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    //Iterate over each cell in the grid
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {

            int vertex_index = i * cols + j;

            //Add the current cell as a vertex
            add_vertex(vertex_index, g);

            //Iterate over each direction
            for (vector<pair<int, int>>::iterator k = directions.begin(); k != directions.end(); ++k) {
                pair<int, int>& direc = *k;
                int new_row_index = i + direc.first;
                int new_col_index = j + direc.second;

                //Check if the new cell is within the bounds of the grid
                if (new_row_index >= 0 && new_row_index < rows && new_col_index >= 0 && new_col_index < cols) {
                    int neighbor_index = new_row_index * cols + new_col_index;

                    //Check if the neighbor vertex already exists
                    bool exists = false;
                    Graph::vertex_iterator curr_vertex = vertices(g).first;
                    Graph::vertex_iterator end_vertex = vertices(g).second;

                    for (; curr_vertex != end_vertex; ++curr_vertex) {
                        if (*curr_vertex == neighbor_index) {
                            exists = true;
                            break;
                        }
                    }

                    //If the neighbor vertex doesn't exist, add it
                    if (!exists && neighbor_index != vertex_index) {
                        add_vertex(neighbor_index, g);
                    }

                    //Check if an edge already exists
                    bool edge_exists = edge(vertex_index, neighbor_index, g).second;

                    // If an edge doesn't exist then add it
                    if (!edge_exists) {
                        add_edge(vertex_index, neighbor_index, g);
                    }
                }
            }
        }
    }

    //Set the starting and ending vertices (always top left and bottom right), vector and map for storage
    Graph::vertex_descriptor start_vertex = 0;
    Graph::vertex_descriptor end_vertex = rows * cols - 1; 
    vector<VertexDirection> path;
    map<Graph::vertex_descriptor, bool> visited;

    //Call DFS
    bool found = dfs(g, start_vertex, end_vertex, grid, path, visited);

    string output_file = "output.txt";

    //Print the path if exists
    if (found) {
        print_path(path, grid.size(), output_file);
    } else {
        cout << "No path exists!" << endl;
    }

    return 0;
}
