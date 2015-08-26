#include <bits/stdc++.h>
using namespace std;

typedef pair<int, int> Arista;

const int MAXN = 102;
const int MAXV = 10000002;
const int INF = 1 << 30;

int slack[MAXN];
int pareja[MAXN];
int retorno[MAXN];
int etiqueta[MAXN];
int costo[MAXN][MAXN];
bool visitado[MAXN];

vector<Arista> EmparejaCostoMaxBipartito(
    const vector<int>& l, const vector<int>& r) {
    int n = l.size() + r.size();
    fill(pareja, pareja + n, -1);
    fill(etiqueta, etiqueta + n, 0);
    for (int i = 0; i < l.size(); ++i)
        for (int j = 0; j < r.size(); ++j)
            etiqueta[l[i]] = max(etiqueta[l[i]],
                costo[l[i]][r[j]]);
            
    for (int i = 0; i < l.size(); ++i) {
        for (int j = 0; j < r.size(); ++j)
            slack[r[j]] = -costo[l[i]][r[j]] +
                etiqueta[l[i]] + etiqueta[r[j]];
        fill(visitado, visitado + n, false);
        fill(retorno, retorno + n, l[i]);
        visitado[l[i]] = true;
        
        bool emparejado = false;
        for (int j = 0; !emparejado; ++j) {
            int t = 0; for (; t < r.size(); ++t) {
                if (visitado[r[t]]) continue;
                if (!slack[r[t]]) break;
            }
            if (t < r.size()) {
                visitado[t = r[t]] = true;
                if (pareja[t] == -1) {
                    emparejado = true;
                    for (int p; ; t = p) {
                        pareja[t] = retorno[t];
                        p = pareja[retorno[t]];
                        pareja[retorno[t]] = t;
                        if (retorno[t] == l[i]) break;
                    }
                } else {
                    visitado[t = pareja[t]] = true;
                    for (int k = 0; k < r.size(); ++k) {
                        int new_slack = etiqueta[t] +
                            etiqueta[r[k]] - costo[t][r[k]];
                        if (new_slack < slack[r[k]]) {
                            slack[r[k]] = new_slack;
                            retorno[r[k]] = t;
                        }
                    }
                }
            } else {
                int d = INF;
                for (int k = 0; k < r.size(); ++k)
                    if (slack[r[k]]) d = min(d, slack[r[k]]);
                for (int k = 0; k < l.size(); ++k)
                    if (visitado[l[k]]) etiqueta[l[k]] -= d;
                for (int k = 0; k < r.size(); ++k)
                    if (!visitado[r[k]]) slack[r[k]] -= d;
                    else etiqueta[r[k]] += d;
            }
        }
    }
    vector<Arista> parejas;
    for (int i = 0; i < l.size(); ++i)
        parejas.push_back(Arista(
            l[i], pareja[l[i]]));
    return parejas;
}

int bucket[MAXV];

int main() {
    int count = 0;
    int t, n, m, x;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d", &m, &n);
        vector<int> deater(m);
        for (int i = 0; i < m; ++i)
            scanf("%d", &deater[i]);
        int s = max(n, m);
        for (int i = 0; i < s + s; ++i)
            fill(costo[i], costo[i] + s + s, 0);

        for (int i = 0; i < n; ++i) {
            scanf("%d", &x);
            vector<int> gold(x);
            for (int j = 0; j < x; ++j)
                scanf("%d", &gold[j]);

            ++count; int l = x + 1>> 1;
            for (int j = 0; j < 1 << l; ++j) {
                int sum = 0;
                for (int k = 0; k < l; ++k)
                    if (j & 1 << k) sum += gold[k];
                if (sum < MAXV) bucket[sum] = count;
            }
            int r = x >> 1;
            for (int j = 0; j < 1 << r; ++j) {
                int sum = 0;
                for (int k = 0; k < r; ++k)
                    if (j & 1 << k) sum += gold[l + k];
                for (int k = 0; k < m; ++k) {
                    if (deater[k] - sum < 0) continue;
                    if (bucket[deater[k] - sum] == count)
                        costo[i][k + s] = deater[k];
                }
            }
        }
        int max_gold = 0;
        vector<int> l(s), r(s);
        for (int i = 0; i < s; ++i) l[i] = i;
        for (int i = 0; i < s; ++i) r[i] = i + s;
        vector<Arista> max_match =
            EmparejaCostoMaxBipartito(l, r);
        for (int i = 0; i < max_match.size(); ++i)
            max_gold += costo[max_match[i].first]
                            [max_match[i].second];
        printf("%d\n", max_gold);
    }
    return 0;
}
