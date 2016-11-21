/* Centroid Decomposition Code Programmed By Dipjal Chhetri */
/* Problem Link - http://codeforces.com/problemset/problem/342/E */
#include "bits/stdc++.h"
using namespace std;
const int N = 1e5 + 5;
const int LGN = 20;
const int lim = 1e6;
int n;
int m;
int u;
int v;
int level[N];
int pa[N][LGN];
bool centroid[N];
int parentCentroid[N];
int subsize[N];
int ans[N];
vector < int > centroidTree[N];
vector < int > adj[N];
int root;
void dfs(int u , int par) {
    subsize[u] = 1;
    int to = (int) adj[u].size();
    for(int i = 0; i < to; ++i) {
        if(adj[u][i] != par && !centroid[adj[u][i]]) {
            dfs(adj[u][i] , u);
            subsize[u] += subsize[adj[u][i]];
        }
    }
}
void reset(int u , int par) {
    subsize[u] = 0;
    int to = (int) adj[u].size();
    for(int i = 0; i < to; ++i) {
        if(adj[u][i] != par && !centroid[adj[u][i]]) {
            reset(adj[u][i] , u);
        }
    }
}
int findCentroid(int u , int par , int root) {
    int to = (int) adj[u].size();
    int mx = 0;
    for(int i = 0; i < to; ++i) {
        if(adj[u][i] != par && !centroid[adj[u][i]]) {
             mx = max(mx , subsize[adj[u][i]]);
        }
    }
    if(mx <= (subsize[root] >> 1) && (subsize[root] - subsize[u]) <= (subsize[root] >> 1) ) {
        return u;
    }
    int special = -1;
    int mxweight = 0;
    for(int i = 0; i < to; ++i) {
        if(adj[u][i] != par && !centroid[adj[u][i]]) {
            if(subsize[adj[u][i]] > mxweight) {
                mxweight = subsize[adj[u][i]];
                special = adj[u][i];
            }
        }
    }
    return findCentroid(special , u , root);
}
int getCentroid(int u) {
    dfs(u , -1);
    int cent = findCentroid(u , -1 , u);
    reset(u , -1);
    centroid[cent] = true;
    return cent;
}

int decompose(int u) {
    int Centroid = getCentroid(u) ;
    int to = (int) adj[Centroid].size();
    for(int i = 0; i < to; i++) {
       if(!centroid[adj[Centroid][i]]) {
         int cent_v = decompose(adj[Centroid][i]);
         centroidTree[Centroid].push_back(cent_v);
         centroidTree[cent_v].push_back(Centroid);
       }
    }
    return Centroid;
}
void par_centroid(int root , int par) {
    parentCentroid[root] = par;
    int to = (int) centroidTree[root].size();
    for(int i = 0; i < to; ++i) {
        if(centroidTree[root][i] != par) {
            par_centroid(centroidTree[root][i] , root);
        }
    }
}
void setpa(int u , int par , int l) {
    pa[u][0] = par;
    level[u] = l;
    int to = (int) adj[u].size();
    for(int i = 0; i < to; ++i) {
        if(adj[u][i] != par) {
            setpa(adj[u][i] , u , l + 1);
        }
    }
}
int LCA(int u , int v) {
    if(level[u] < level[v]) {
        swap(u , v);
    }
    int diff = level[u] - level[v];
    for(int i = 0; i < LGN; ++i) {
        if((diff >> i) & 1) {
            u = pa[u][i];
        }
    }
    if(u == v) {
        return u;
    }
    for(int i = LGN - 1; i >= 0; --i) {
        if(pa[u][i] != pa[v][i]) {
            u = pa[u][i] , v = pa[v][i];
        }
    }
    return pa[u][0];
}
int getDistance(int u , int v) {
    return level[u] + level[v] - 2 * level[LCA(u , v)];
}
void change(int x) {
    int u = x;
    while(x != -1) {
        ans[x] = min(ans[x] , getDistance(u , x) );
        x = parentCentroid[x];
    }
}
void query(int x) {
    int u = x;
    int mn = lim;
    while(x != -1) {
        mn = min(mn , getDistance(u , x) + ans[x]);
        x = parentCentroid[x];
    }
    printf("%d\n" , mn);
}
int main() {
    scanf("%d %d" , &n , &m);
    for(int i = 1; i < n; ++i) {
        scanf("%d %d" , &u , &v);
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    root = decompose(1);
    par_centroid(root , -1);
    memset(pa, -1 , sizeof pa);
    setpa(1 , -1 , 0);
    for(int i = 1; i < LGN; ++i)
        for(int j = 1; j <= n; ++j)
           pa[j][i] = pa[j][i - 1] != -1 ? pa[pa[j][i - 1] ] [i - 1] : -1;

    for(int i = 1; i < N; ++i)
        ans[i] = lim;

    change(1);

    int type , x;
    while(m--) {
        scanf("%d %d" , &type , &x);
        if(type == 1) {
            change(x);
        } else {
            query(x);
        }
    }
    return 0;
}
