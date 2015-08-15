#include <bits/stdc++.h>
using namespace std;

typedef vector< vector<double> > Matriz;
const double ERROR = 1e-9;

bool Igual(double a, double b) {
    return fabs(a - b) < ERROR;
}

void EliminaGaussiana(Matriz& m) {
    for (int i = 0; i < m.size(); ++i) {
        int fila_mayor = i;
        for (int j = i + 1; j < m.size(); ++j)
            if (fabs(m[fila_mayor][i]) <
                fabs(m[j][i])) fila_mayor = j;
        swap(m[i], m[fila_mayor]);
        if (Igual(m[i][i], 0)) continue;
        for (int j = m[i].size() - 1; j >= i; --j)
            m[i][j] /= m[i][i];
        for (int j = 0; j < m.size(); ++j) {
            if (i == j || Igual(m[j][i], 0)) continue;
            double ratio = m[j][i] / m[i][i];
            for (int k = i; k < m[j].size(); ++k)
                m[j][k] -= m[i][k] * ratio;
        }
        
    }
}

const int MAXW = 12;
const int MAXH = 12;

double blur[MAXH][MAXW];

int main() {
    int w, h, d;
    bool first = true;
    while (true) {
        scanf("%d%d%d", &w, &h, &d);
        if (!w && !h && !d) break;
        if (!first) printf("\n"); first = false;
        for (int i = 0; i < h; ++i)
            for (int j = 0; j < w; ++j)
                scanf("%lf", &blur[i][j]);
        Matriz original(w * h);
        for (int i = 0; i < w * h; ++i)
            original[i].resize(w * h + 1);
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                int conteo = 0;
                for (int k = 0; k < h; ++k)
                    for (int l = 0; l < w; ++l)
                        if (abs(k - i) + abs(l - j) <= d)
                            conteo += original[i * w + j][k * w + l] = 1;
                original[i * w + j][w * h] = blur[i][j] * conteo;
            }
        }
        EliminaGaussiana(original);
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j)
                printf("%8.2lf", original[i * w + j][w * h]);
            printf("\n");
        }
    }
    return 0;
}