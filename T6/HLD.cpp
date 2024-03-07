#include <bits/stdc++.h>
#include <vector>
#include <map>
#include <string>
#include <functional>
#include <utility>

using namespace std;

/* Implementación del Segment Tree */
class ST {
public:
    int n; 
    vector<int> st; 

    ST(vector<int>* v);
    int query(int i, int j); 
};

/* Constructor del Segment Tree */
ST::ST(vector<int>* v)
{
    n = v->size(); 
    st = vector<int>(4 * n, INT_MAX); 

    // Se copian los valores del vector original al Segment Tree
    for (int i = 0; i < n; i++)     
        st[n + i] = (*v)[i]; 

    // Se construye el Segment Tree
    for (int i = n - 1; i > 0; --i)      
        st[i] = min(st[2 * i], st[2 * i + 1]);  
}

/* Consultas al Segment Tree, para determinar el mínimo valor en un rango dado */
int ST::query(int i, int j)  
{  
    if (i == j)
        return (st[i + n]);

    int i2 = i + n;
    int j2 = j + n;
    int r = INT_MAX; // Se inicializa el resultado con el valor neutro de la operacion minimo
 
    while (i2 < j2) 
    {
        if (i2 & 1) 
        {
            r = min(r, st[i2]);
            i2++;
        }

        if (j2 & 1) 
        {
            j2--;
            r = min(r, st[j2]);
        }
 
        i2 /= 2;
        j2 /= 2;
    }
    return r; 
}  

/* Implementación del Segment Tree con consultas de predicado */
class STP {
public:
    int n; 
    vector<int> st; 
    function<bool(int)> p; // Predicado a verificar en las consultas
    bool isExistential; // Indica si las consultas son existenciales

    STP(vector<pair<int, int>>* v, function<bool(int)> func, bool isExistential); 
    bool query(int i, int j);
};

/* Constructor del Segment Tree */
STP::STP(vector<pair<int, int>>* v, function<bool(int)> func, bool isExistential)
{
    n = v->size();
    p = func; // Se asigna el predicado
    this->isExistential = isExistential; // Se asigna el tipo de consulta
    st = vector<int>(4 * n, isExistential ? 0 : 1); 

    // Se copian los valores del vector original al Segment Tree
    for (int i = 0; i < n; i++)     
        st[n + i] = (*v)[i].second; 

    // Se construye el Segment Tree
    for (int i = n - 1; i > 0; --i) {
        st[i] = isExistential ? (p(st[2 * i]) ? st[2 * i] : st[2 * i + 1]) : (!(p(st[2 * i])) ? st[2 * i] : st[2 * i + 1]); 
    }    
}

/* Consultas al Segment Tree con forall o exists */
bool STP::query(int i, int j)  
{  
    if (i == j)
        return (false);

    int i2 = i + n;
    int j2 = j + n;
    bool r = isExistential ? false : true; // Se inicializa el resultado con el valor neutro correspondiente
 
    while (i2 < j2) 
    {
        if (i2 & 1) 
        {
            r = isExistential ? (r || p(st[i2])) : (r && p(st[i2]));
            i2++;
        }

        if (j2 & 1) 
        {
            j2--;
            r = isExistential ? (r || p(st[j2])) : (r && p(st[j2]));
        }
 
        i2 /= 2;
        j2 /= 2;
    }
    return r; 
}  
 
 
/* Implementación de la clase de un árbol */
class Tree {
public:
    int n; 
    pair<int, int> root; 
	map<int, vector<pair<int, int>>> children; 
    vector<int> eulerPath, firstTag; 
    vector<bool> visited; 

    Tree();
    void addNode(int parent, int node, int c, bool isRoot); 
	void DFS(int v, int *tag); 
    void calcEulerPath(); 
};

/* Constructor del árbol */
Tree::Tree() 
{
    n = 0; 
}

/* Método para añadir nodos al árbol */
void Tree::addNode(int parent, int node, int c, bool isRoot)
{
    if (!(isRoot))
        children[parent].push_back(pair<int, int>(node, c));
    else
        root = pair<int, int>(node, c); 
	children.insert({node, vector<pair<int, int>>()}); 
    n++; 
}

/* Método DFS para determinar el camino de Euler */
void Tree::DFS(int v, int *tag)
{
    if (!(visited[v - 1]))
        firstTag[v - 1] = *tag;

    eulerPath[*tag] = v;
    (*tag)++; 

	// Se realiza un DFS para cada hijo del nodo
	for (auto it : children[v])
    {
        DFS(it.first, tag);
        eulerPath[*tag] = v; 
        (*tag)++; 
    }
}

/* Método para determinar el camino de Euler en el árbol */
void Tree::calcEulerPath() 
{
    eulerPath = vector<int>(2 * n, 0);
    visited = vector<bool>(n, false); 
    firstTag = vector<int>(n, 0); 
    int c = 0;
    DFS(root.first, &c);
}

/* Implementación de la descomposición pesada-liviana con consultas de predicado */
class HLD {
public:
    int n; 
    int cur_pos; 
    function<bool(int)> p; // Predicado a verificar en las consultas
    bool isExistential; // Indica si las consultas son existenciales
    vector<int> head, pos, visited;
    vector<pair<int, int>> heavy, parent;
    Tree *t; // Árbol a descomponer
    map<int, vector<pair<int, int>>*> trees; 
    map<int, STP*> segts;
    
    HLD(Tree *tree, function<bool(int)> func, bool isExistential);
    int DFS(int v); 
    void decompose(pair<int, int> v, int h, int c_pos); 
    bool queryChain(int c_st, int lca);
    bool query(int a, int b, Tree *euler_path_tree);
};

/* Constructor de la descomposición */
HLD::HLD(Tree *tree, function<bool(int)> func, bool isExistential) 
{
    p = func; 
    t = tree; 
    this->isExistential = isExistential; 
    n = t->n;
    cur_pos = 0; 
    head = vector<int>(n);
    pos = vector<int>(n);
    visited = vector<int>(n); 
    parent = vector<pair<int, int>>(n); 
    heavy = vector<pair<int, int>>(n, pair<int, int>(-1, -1)); 
    trees = map<int, vector<pair<int, int>>*>(); 
    segts = map<int, STP*>(); 

    parent[0] = t->root; 
    DFS(t->root.first); 
    decompose(t->root, t->root.first, 1); 
    for (auto m : trees) 
        segts.insert({m.first, new STP(m.second, p, isExistential)});
}

/* Método DFS para calcular los hijos pesados */
int HLD::DFS(int v) {
    int size = 1; 
    int max_c_size = 0; 

    for (auto it : t->children[v]) 
    {
        parent[(it.first) - 1] = pair<int, int>(v, it.second); 
        int c_size = DFS(it.first); 
        size += c_size; 
        // Si el tamaño del subárbol del hijo actual es mayor que el tamaño del subárbol más grande, se actualiza el hijo pesado
        if (c_size > max_c_size)
            max_c_size = c_size, heavy[v - 1] = it;
    }

    return size;
}

/* Método para realizar la descomposición */
void HLD::decompose(pair<int, int> v, int h, int c_pos) {
    head[(v.first) - 1] = h, pos[(v.first) - 1] = c_pos; 
    
    // Si el nodo actual es la cabeza de una nueva cadena pesada, se añade al mapa de cadenas pesadas
    if (!(visited[h - 1]))
    {
        visited[h - 1] = 1;
        trees.insert({h, new vector<pair<int, int>>({pair<int, int>(h, INT_MAX)})});
    } else // Si no, se añade a la cadena pesada correspondiente
        trees[h]->push_back(v);
               
    if (heavy[(v.first) - 1].first != -1)
        decompose(heavy[(v.first) - 1], h, c_pos + 1);

    for (auto it : t->children[v.first]) 
    {
        if (it.first != heavy[(v.first) - 1].first)
            decompose(pair<int, int>(it.first, INT_MAX), it.first, 1);
    }
}

/* Método para realizar consultas en una cadena pesada */
bool HLD::queryChain(int c_st, int lca) {
    int c_st_h = head[c_st - 1]; 
    bool res = isExistential ? false : true;
    int low;
    int high;

    // Mientras la cabeza de la cadena pesada actual no sea la cabeza de la cadena pesada del ancestro común más bajo
    while (c_st_h != head[lca - 1]) 
    {   
        if (c_st != c_st_h)
        {
            low = min(pos[c_st_h - 1], pos[c_st - 1]); 
            high = max(pos[c_st_h - 1], pos[c_st - 1]); 
            res = isExistential ? (res || segts[c_st_h]->query(low, high)) : (res && segts[c_st_h]->query(low, high));
        } 
        // Se verifica el predicado en la arista que conecta la cabeza de la cadena pesada actual con su padre y se actualiza el resultado
        res = isExistential ? (res || p(parent[c_st_h - 1].second)) : (res && p(parent[c_st_h - 1].second));
        c_st = parent[c_st_h - 1].first; 
        c_st_h = head[c_st - 1]; 
    }

    if (c_st != lca) 
    {
        low = min(pos[c_st - 1], pos[lca - 1]); 
        high = max(pos[c_st - 1], pos[lca - 1]); 
        res = isExistential ? (res || segts[c_st_h]->query(low, high)) : (res && segts[c_st_h]->query(low, high));
    }
    
    return res;
}


/* Método para realizar consultas en un rango */
bool HLD::query(int a, int b, Tree *t) {
    // Si los nodos a y b están en la misma cadena pesada, se realiza la consulta en la cadena pesada correspondiente
    if (head[a - 1] == head[b - 1]) 
    {
        int h = head[a - 1];
        STP* st = segts[h];
        return (st->query(pos[a - 1], pos[b - 1]));
    }

    ST euler_path(&(t->eulerPath));
    int low = min(t->firstTag[a - 1], t->firstTag[b - 1]); 
    int high = max(t->firstTag[a - 1], t->firstTag[b - 1]); 
    int lca = euler_path.query(low, high);

    bool resA = queryChain(a, lca);
    bool resB = queryChain(b, lca);

    bool res = isExistential ? (resA || resB) : (resA && resB);

    return res; 
}

// predicado para determinar si un numero es par
bool par(int n) 
{
    return (n % 2 == 0);
}

// predicado para determinar si un numero es mayor a 15
bool predic(int n) 
{
    return (n > 15);
}

int main()
{
	Tree t;
    t.addNode(-1, 1, INT_MAX, true);
    t.addNode(1, 2, 14, false);
    t.addNode(1, 3, 10, false);
    t.addNode(1, 4, 23, false);
    t.addNode(2, 5, 4, false);
    t.addNode(2, 6, 25, false);
    t.addNode(3, 7, 29, false);
    t.addNode(3, 8, 22, false);
    t.addNode(5, 9, 12, false);
    t.addNode(6, 10, 5, false);
    t.addNode(7, 11, 30, false);
    t.addNode(10, 12, 1, false);
    t.addNode(10, 13, 6, false);
    
    t.calcEulerPath();
    
    HLD hlde(&t, predic, false);
    
    cout << hlde.query(1, 4, &t) << endl;

	return 0;
}
