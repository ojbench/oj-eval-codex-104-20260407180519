#include <bits/stdc++.h>
using namespace std;

// Problem 1028: Find shortest path from start(2) to home(3)
// while visiting at least one umbrella shop(4) along the way.
// Grid up to 1000x1000, obstacles 1 are blocked; 0/2/3/4 are passable.
// Move in 4 directions. Output minimal moves.

static const int INF = 1e9;

struct Pos { int r, c; };

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;
    // Input states given as m lines each with n numbers (README uses n,m first then m rows of n columns)
    // So columns = n, rows = m.
    int cols = n, rows = m;
    vector<vector<int>> a(rows, vector<int>(cols));
    vector<Pos> shops;
    Pos S{-1,-1}, T{-1,-1};
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int x; cin >> x; a[i][j] = x;
            if (x == 2) S = {i, j};
            else if (x == 3) T = {i, j};
            else if (x == 4) shops.push_back({i, j});
        }
    }

    auto inb = [&](int r, int c){ return r>=0 && r<rows && c>=0 && c<cols; };

    auto bfs = [&](Pos src){
        vector<int> dist(rows*cols, INF);
        deque<int> q;
        auto idx = [&](int r,int c){ return r*cols + c; };
        if (src.r!=-1) {
            dist[idx(src.r, src.c)] = 0;
            q.push_back(idx(src.r, src.c));
        }
        const int dr[4]={-1,1,0,0};
        const int dc[4]={0,0,-1,1};
        while(!q.empty()){
            int id = q.front(); q.pop_front();
            int r = id/cols, c = id%cols;
            for(int k=0;k<4;++k){
                int nr=r+dr[k], nc=c+dc[k];
                if(!inb(nr,nc)) continue;
                if(a[nr][nc]==1) continue;
                int nid = idx(nr,nc);
                if(dist[nid] > dist[id] + 1){
                    dist[nid] = dist[id] + 1;
                    q.push_back(nid);
                }
            }
        }
        return dist;
    };

    // Distances from S and from T
    auto dS = bfs(S);
    auto dT = bfs(T);

    auto idx = [&](int r,int c){ return r*cols + c; };

    long long best = LLONG_MAX;

    // Consider paths S->shop->T: sum dS[shop]+dT[shop]
    for (auto p: shops){
        int is = idx(p.r,p.c);
        if (dS[is] >= INF || dT[is] >= INF) continue;
        best = min(best, (long long)dS[is] + dT[is]);
    }

    // Edge case: if S or T is itself a shop location (value 4), above handles since it's in shops list
    // Another potential: There might be multiple shops, but the shortest route passes any one of them.

    if (best==LLONG_MAX){
        // If unreachable, problem statement likely ensures reachable; but be safe and print -1.
        cout << -1 << '\n';
    } else {
        cout << best << '\n';
    }

    return 0;
}

