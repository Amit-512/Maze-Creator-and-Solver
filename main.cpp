#include "bits/stdc++.h"
using namespace std;
struct DSU
{
    vector<int> parent, sze;
    void build(int n)
    {
        parent.resize(n, 0), sze.resize(n, -1);
        for (int k = 0; k < n; k++)
            parent[k] = k, sze[k] = 1;
    }
    int find(int v)
    {
        if (parent[v] == v)
            return v;
        else
            return parent[v] = find(parent[v]);
    }
    void unite(int a, int b)
    {
        a = find(a), b = find(b);
        if (a != b)
        {
            if (sze[a] < sze[b])
                swap(a, b);
            parent[b] = a, sze[a] += sze[b];
        }
    }
};
int dirx[4] = {0,0,-1,1};
int diry[4] = {1,-1,0,0};
class Maze
{
public:
    vector<vector<char>> maze;
    vector<vector<bool>> vis;//for dfs
    int start;
    int end;
    int n;
    void printmaze();
    void buildmaze();
    int AssignWeight(); 
    Maze(int t);
 
private:
    set<tuple<int, int, int>> edges; // {weight,cell1,cell2} edge between cell1 and cell2
    set<pair<int, int>> mst; // {cell1,cell2} edge between cell1 and cell2
    DSU dsu;
};


Maze::Maze(int t) //Generating maze with no cells connected yet
{
    n = t;
    maze.resize(2 * n + 1, vector<char>(2 * n + 1, '-'));
    vis.resize(2 * n + 1, vector<bool> (2*n+1));
    dsu.build(n * n + 1);
    for (int i = 1; i <= 2 * n + 1; i += 2)
    {
        for (int j = 1; j <= 2 * n + 1; j += 2)
        {
            if (i != 2 * n + 1 && j != 2 * n + 1)
                maze[i][j] = ' ';
            if (j != 2 * n + 1)
                maze[i - 1][j] = '-';
            maze[i - 1][j - 1] = '+';
            if (i != 2 * n + 1)
                maze[i][j - 1] = '|';
        }
    }
}


void Maze::printmaze() 
{
    int st = 2 * start + 1, ed = 2 * end + 1;
    while (st--)
        cout << ' ';
    cout << 'v' << endl;
    for (int i = 0; i < 2 * n + 1; i++)
    {
        for (int j = 0; j < 2 * n + 1; j++)
            cout << maze[i][j];
        cout << endl;
    }
    while (ed--)
        cout << ' ';
    cout << 'v' << endl;
}
void Maze::buildmaze()
{
    for (int i = 1; i <= n; i++) // cells are numbered from 1 to n*n
    {
        int f = (i - 1) * n;
        for (int j = f + 1; j <= f + n; j++)
        {
            if (j < f + n) 
            {
                int getWeight = AssignWeight();
                edges.insert({getWeight, j, j + 1});  // Edge between current cell and right cell
            }
            if (j + n <= n * n)
            {
                int getWeight = AssignWeight();
                edges.insert({getWeight, j, j + n});  // Edge between current cell and bottom cell
            }
        }
    }
    start = rand() % n;
    end = rand() % n;
    for (auto [w, u, v] : edges)
    {
        if (dsu.find(u) != dsu.find(v)) // unite if u and v dont belong to same component
        {
            dsu.unite(u, v);
            mst.insert({u, v});
        }
        //cout<<n*n-n+end<<"found"<<endl;
        //cout<<start<<n<<endl;
        if(dsu.find(start+1) == dsu.find(n*n-n+end+1))
        {
            break;
        }
    }
    for (auto [u, v] : mst) // Removing edges that are present in MST to generate the maze
    {
        int i = (u - 1) / n, j = (u - 1) % n;
        if (v == u + 1) maze[2 * i + 1][2 * j + 2] = ' ';
        else maze[2 * i + 2][2 * j + 1] = ' ';
    }
 
    
    maze[0][2 * start + 1] = ' ';
    maze[2 * n][2 * (end) + 1] = ' ';
}
 
int Maze::AssignWeight() // Generates Random Weight
{
    return rand()%1000;
}

bool isValid(int start,int row,Maze &m)
{
    if(start<0||row<0)return 0;
    if(start>=2*m.n||row>2*m.n)
    {
        return 0;
    }
    if(m.vis[row][start]==1)
    {
        //cout<<"vis"<<endl;
        return 0;
    }
    if(m.maze[row][start]!=' ')
    {
        //cout<<"not sapce"<<endl;
        return 0;
    }
    return 1;
}

bool dfs(int start,int row,int end,Maze &m)
{
    
    m.vis[row][start]=1;
    if(start==end&&row==2*m.n)
    {
        m.maze[row][start]='o';
        return 1;
        //cout<<"found"<<endl<<endl;
    }
    //cout<<end<<endl;
    for(int i=0;i<4;i++)
    {
        if(isValid(start+dirx[i],row+diry[i],m))
        {
            //cout<<start+dirx[i]<<" "<<row+diry[i]<<endl;
            if(dfs(start+dirx[i],row+diry[i],end,m))
            {
                m.maze[row][start]='o';
                return 1;
            }
        }
    }
    return 0;
}
 
int main()
{
 
    cout << "Enter the size of the maze : ";
    srand(time(0));
    int n;
    cin >> n;
    cout << "\n\n";
    Maze m(n);
    cout << "Building the Maze........\n\n";
    m.buildmaze();

    m.printmaze();
    cout << "Solution to the Maze using dfs Algorithm: \n\n";
    cout<<n<<endl;
    // for(int i=0;i<=2*m.n;i++)
    // {
    //     for(int j=0;j<=2*m.n;j++)
    //     {
    //         cout<<m.maze[i][j];
    //     }
    //     cout<<endl;
    // }
    // cout<<m.start<<" "<<m.end<<endl;
    dfs(2*m.start+1,0,2*m.end+1,m);
    //m.maze[0][2*m.start+1]='o';
    //cout<<isValid(0,3,m)<<endl;
    m.printmaze();
}
