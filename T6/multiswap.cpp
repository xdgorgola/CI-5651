#include <stdlib.h>
#include <iostream>
#include <vector>

using namespace std;

// Nodo treap
typedef struct node
{
    int prior, size, val;
    struct node *l, *r;

} node;
typedef node *pnode;

// Impresion inorder
void print(pnode t)
{
    if (!t)
        return;

    print(t->l);
    cout << t->val << ",";
    print(t->r);
}


// Inicializacion de un nodo.
pnode init(int val)
{
    pnode ret = (pnode)malloc(sizeof(node));
    ret->prior = rand();
    ret->size = 1;
    ret->val = val;
    return ret;
}

// Obtencion de valor de un nodo
int sz(pnode t)
{
    return t ? t->size : 0;
}

// Actualizacion de valor de un nodo.
void upd_sz(pnode t)
{
    if (t)
        t->size = sz(t->l) + 1 + sz(t->r);
}

// Operacion split
// Operacion O(log n)
void split(pnode t, pnode &l, pnode &r, int pos, int add = 0)
{
    if (!t)
        return void(l = r = NULL);

    int curr_pos = add + sz(t->l);
    if (curr_pos <= pos)
        split(t->r, t->r, r, pos, curr_pos + 1), l = t;
    else
        split(t->l, l, t->l, pos, add), r = t;
    upd_sz(t);
}

// Operacion merge
// Operacion O(log n)
void merge(pnode &t, pnode l, pnode r)
{
    if (!l || !r)
        t = l ? l : r;
    else if (l->prior > r->prior)
        merge(l->r, l->r, r), t = l;
    else
        merge(r->l, l, r->l), t = r;
    upd_sz(t);
}

// Insercion de elementos
// Operacion O(log n)
void insert(pnode &t, pnode it)
{
    if (!t)
        t = it;
    else if (it->prior > t->prior)
        split(t, it->l, it->r, it->val), t = it;
    else
        insert(t->val <= it->val ? t->r : t->l, it);
    upd_sz(t);
}


// Multiswap consiste de 3 splits o 2 splits , y 3 o 2.
// Asi O(3 log n + 2 log n) = O(5 log n) = O(log n)
void multiswap(pnode &t, int a, int b, int n)
{
    a--; --b; // Queremos hacer los cortes antes de los elementos

    // Caso borde, a = 1 y b elegido de tal forma que multiswap llegue al final del array
    if (a == -1 && 2 * b >= n)
    {
        pnode t0, t1, t2;
        pnode tmp1, tmp2;
        split(t, t0, tmp1, n - b - 2);
        split(tmp1, t1, t2, 2 * (b + 1) - n - 1);

        merge(tmp2, t2, t1);
        merge(t, tmp2, t0);
        return;
    }

    pnode t0, t1, t2, t3;
    pnode tmp1, tmp2, tmp3, tmp4, tmp5;
    bool out_range = 2 * b - a >= n;
    int snd_split = (out_range ? n - b - 2 : b - a - 1);
    int trd_split = (out_range ? 2 * b - n - 2 : b - a - 1);

    split(t, t0, tmp1, a);
    split(tmp1, t1, tmp2, snd_split);
    split(tmp2, t2, t3, trd_split);

    if (!out_range)
    {
        merge(tmp4, t0, t2);
        merge(tmp5, tmp4, t1);
        merge(t, tmp5, t3);
        return;
    }

    merge(tmp4, t0, t3);
    merge(tmp5, tmp4, t2);
    merge(t, tmp5, t1);
}

// La inicializacion es O(n log n) dado a que la insercion es O(log n) y se realiza n veces.
// y tenemos que multiswap es O(log n). Multiswap se hace n veces, por ende tenemos que 
// n_multiswaps es O(2 n log n) = O(n log n)
void n_multiswaps(int n, vector<pair<int,int>> swaps)
{
    pnode root = init(1);
    for (int i = 2; i <= n; ++i)
        insert(root, init(i));

    for (pair<int, int> op : swaps)
        multiswap(root, op.first - 1, op.second - 1, n);

    print(root);
    cout << endl;
}