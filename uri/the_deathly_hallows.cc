#include <bits/stdc++.h>
using namespace std;

typedef double Coord;
const double ERROR = 1e-9;

bool Igual(Coord a, Coord b) { 
    return fabs(a - b) < ERROR;
}

struct Punto {
    Coord x, y;
    Punto(Coord x_, Coord y_)
        : x(x_), y(y_) {}

    bool operator<(const Punto& cmp) const {
        if (!Igual(x, cmp.x)) return x < cmp.x;
        return Igual(y, cmp.y)? false: y < cmp.y;
    }
};

double Distancia(const Punto& p, const Punto& q) {
    return hypot(p.x - q.x, p.y - q.y);
}

double Magnitud(const Punto& v) {
    return hypot(v.x, v.y);
}

double Dot(const Punto& v, const Punto& w) {
    return v.x * w.x + v.y * w.y;
}

double Cruz(const Punto& v, const Punto& w) {
    return v.x * w.y - v.y * w.x;
}

Punto Trasladar(const Punto& o, const Punto& p) {
    return Punto(p.x - o.x, p.y - o.y);
}

Punto Escalar(const Punto& v, double s) {
    return Punto(v.x * s, v.y * s);
}

Punto Opuesto(const Punto& v) {
    return Punto(-v.x, -v.y);
}

int ManoDerecha(const Punto& o, const Punto& p, const Punto& q) {
    double ccw = Cruz(Trasladar(o, p), Trasladar(o, q));
    return Igual(ccw, 0)? 0: (ccw < 0)? -1: 1;
}

struct Linea {
    Punto p, q;
    Linea(const Punto& p_, const Punto& q_)
        : p(p_), q(q_) { if (q < p) swap(p, q); }
};

bool PuntoEnRecta(const Punto& p, const Linea& r) {
    return !ManoDerecha(r.p, r.q, p);
}

bool PuntoEnSegmento(const Punto& p, const Linea& s) {
    return PuntoEnRecta(p, s) && !(p < s.p || s.q < p);
}

Punto ProyeccionEnRecta(const Punto& p, const Linea& r) {
    Punto a = Trasladar(r.p, p), b = Trasladar(r.p, r.q);
    return Trasladar(Opuesto(r.p), Escalar(
        b, Dot(a, b) / pow(Magnitud(b), 2)));
}

double DistanciaPuntoSegmento(const Punto& p, const Linea& s) {
    Punto proy = ProyeccionEnRecta(p, s);
    if (proy < s.p) return Distancia(s.p, p);
    if (s.q < proy) return Distancia(s.q, p);
    return Distancia(proy, p);
}

bool InterseccionSegmentos(const Linea& s, const Linea& t) {
    int t1 = ManoDerecha(s.p, s.q, t.p);
    int t2 = ManoDerecha(s.p, s.q, t.q);
    if (t1 == t2) {
        if (!t1 && (PuntoEnSegmento(s.p, t) ||
            PuntoEnSegmento(s.q, t) ||
            PuntoEnSegmento(t.p, s) ||
            PuntoEnSegmento(t.q, s)))
            return true;
        return false;
    }
    return ManoDerecha(t.p, t.q, s.p) !=
           ManoDerecha(t.p, t.q, s.q);
}

double DistanciaSegmenSegmen(const Linea& s, const Linea& r) {
    if (InterseccionSegmentos(s, r)) return 0;
    return min(min(DistanciaPuntoSegmento(s.p, r),
                   DistanciaPuntoSegmento(s.q, r)),
               min(DistanciaPuntoSegmento(r.p, s),
                   DistanciaPuntoSegmento(r.q, s)));
}

typedef pair<int, int> Arista;
typedef pair<double, Arista> AristaPeso;

struct UnionFind {
    vector<int> padre;

    UnionFind(int n) : padre(n) {
        for(int i = 0; i < n; ++i)
            padre[i] = i;
    }

    int Encontrar(int u) {
        if (padre[u] == u) return u;
        return padre[u] = Encontrar(padre[u]);
    }

    void Unir(int u, int v) {
        int Ru = Encontrar(u);
        int Rv = Encontrar(v);
        if (Ru == Rv) return;
        padre[Ru] = Rv;
    }

    bool MismoConjunto(int u, int v) {
        return Encontrar(u) == Encontrar(v);
    }
};

int main() {
    int n;
    scanf("%d", &n);
    int x1, x2, y1, y2;
    vector<Punto> puntos;
    vector<Linea> segmentos;
    for (int i = 0; i < n; ++i) {
        scanf("%d%d%d%d", &x1, &y1, &x2, &y2);
        if (x1 == x2 && y1 == y2) puntos.push_back(Punto(x1, y1));
        else segmentos.push_back(Linea(Punto(x1, y1), Punto(x2, y2)));
    }
    priority_queue<AristaPeso,
                   vector<AristaPeso>,
                   greater<AristaPeso> > pq;
    for (int i = 0; i < segmentos.size(); ++i) {
        for (int j = i + 1; j < segmentos.size(); ++j) {
            double distancia = DistanciaSegmenSegmen(
                segmentos[i], segmentos[j]);
            pq.push(make_pair(distancia,
                make_pair(i, j)));
        }
    }
    int s = segmentos.size();
    for (int i = 0; i < puntos.size(); ++i) {
        for (int j = i + 1; j < puntos.size(); ++j) {
            double distancia = Distancia(puntos[i], puntos[j]);
            pq.push(make_pair(distancia, make_pair(i + s, j + s)));
        }
    }
    for (int i = 0; i < segmentos.size(); ++i) {
        for (int j = 0; j < puntos.size(); ++j) {
            double distancia = DistanciaPuntoSegmento(
                puntos[j], segmentos[i]);
            pq.push(make_pair(distancia,
                make_pair(i, j + s)));
        }
    }
    double total = 0;
    UnionFind grafo(n);
    while (!pq.empty()) {
        double d = pq.top().first;
        int u = pq.top().second.first;
        int v = pq.top().second.second; pq.pop();
        if (grafo.MismoConjunto(u, v)) continue;
        grafo.Unir(u, v); total += d;
    }
    printf("%.0lf\n", ceil(total));
    return 0;
}
