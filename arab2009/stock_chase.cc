#include <bits/stdc++.h>
using namespace std;

typedef pair<int, int> Arista;
const int MAXN = 236;

int vis[MAXN];
vector<int> grafo[MAXN];

bool HayCiclo(int u) {
    vis[u] = 1;
    for (int i = 0; i < grafo[u].size(); ++i) {
        if (vis[grafo[u][i]] == 2) continue;
        if (vis[grafo[u][i]] == 1 ||
            HayCiclo(grafo[u][i]))
            return true;
    }
    vis[u] = 2;
    return false;
}

int main() {
    int n, t, a, b;
    for (int c = 1; ; ++c) {
        scanf("%d%d", &n, &t);
        if (!n && !t) break;

        int validas = 0;
        map<Arista, int> previas;
        for (int i = 0; i < n; ++i)
            grafo[i].clear();

        for (int i = 0; i < t; ++i) {
            scanf("%d%d", &a, &b);
            Arista arista(--a, --b);

            if (!previas.count(arista)) {
                grafo[a].push_back(b);
                fill(vis, vis + n, 0);
                if (HayCiclo(a)) {
                    previas[arista] = 1;
                    grafo[a].pop_back();
                } else previas[arista] = 0;
            }
            validas += previas[arista];
        }
        printf("%d. %d\n", c, validas);
    }
    return 0;
}