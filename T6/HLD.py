class Tree():
    def __init__(self) -> None:
        self.adj: dict[int, list[tuple[int,bool]]] = {}
        self.sub_size: dict[int, int] = {}
        self.par: dict[int, int] = {}
        self.heavy: dict[int, int] = {}
        self.st: dict[int, (list[bool], list[bool])] = {}
        self.depth: dict[int, int] = {}

        self.chain_head: dict[int, int] = {}
        self.pos_n_st: dict[int, int] = {}
        self.st: dict[int, tuple[list[int], list[int]]] = {}

    def add_edge(self, f: int, t: int, v: bool):
        self.par[t] = f
        if not f in self.adj:
            self.adj[f] = []
        
        self.adj[f].append((t,v))


def dfs(cur: int, tree: Tree, depth: int = 0):
    tree.depth[cur] = depth
    if not cur in tree.adj:
        tree.sub_size[cur] = 1
        tree.heavy[cur] = None
        return 1
    
    max_child_size, max_child = None, None
    for c,_ in tree.adj[cur]:
        c_size = dfs(c, tree, depth + 1)
        if not max_child:
            max_child_size = c_size
            max_child = c
            continue

        if c_size > max_child_size:
            max_child_size = c_size
            max_child = c

    tree.sub_size[cur] = max_child_size + 1
    tree.heavy[cur] = max_child
    return tree.sub_size[cur]


def lca(a: int, b: int, tree: Tree):
    da = tree.depth[a]
    db = tree.depth[b]
    while da != db or a != b:
        if da > db:
            a = tree.par[a]
            da = da - 1
            continue
        elif da < db:
            b = tree.par[b]
            db = db - 1
            continue

        a = tree.par[a]
        b = tree.par[b]

    return a


def chain_preds(head: int, tree: Tree):
    preds = []
    print(head)
    while head in tree.heavy:
        print(tree.adj[tree.heavy[head]][1])
        preds.append(tree.adj[tree.heavy[head]][1])
        head = tree.adj[tree.heavy[head]][1]
        print(head)
    return preds


def st_cadena(head: int, tree: Tree):
    preds   = chain_preds(head, tree)
    forall  = [preds[0]]
    exists  = [preds[0]]
    for p in preds[1:]:
        forall.append(forall[-1] and p)
        exists.append(forall[-1] or p)

    for i in range(len(preds) - 1, 0, -1) : 
        forall[i] = forall[i << 1] and forall[i << 1 | 1]; 
        exists[i] = exists[i << 1] or exists[i << 1 | 1]; 

    return (forall, exists)


# Se llama inicialmente con cur en la raiz st_pos 0 chain_head 0
def decompose(cur: int, tree: Tree, st_pos: int, chain_head: int):
    tree.chain_head[cur] = chain_head
    tree.pos_n_st[cur] = st_pos

    if tree.heavy[cur] in tree.heavy:
        decompose(tree.heavy[cur], tree, st_pos + 1, chain_head)

    if not cur in tree.adj:
        return
    
    for c,_ in tree.adj[cur]:
        if tree.heavy[cur] == c:
            continue
        decompose(c, tree, 0, c)


def precond(root: int, tree: Tree):
    dfs(root, tree)
    decompose(root, tree, 0, root)

    for k,v in tree.chain_head.items():
        if k != v:
            continue
        tree.st[k] = st_cadena(k, tree)


def imprimir_cadena(cur: int, tree: Tree):
    print(f'cadena de {cur}: {cur}', end=',')
    while cur in tree.heavy:
        cur = tree.heavy[cur]
        if cur:
            print(cur, end=',')
    print()


t = Tree()
t.add_edge(0, 1, True)
t.add_edge(0, 2, True)
t.add_edge(0, 3, True)

t.add_edge(1, 4, True)
t.add_edge(1, 5, True)

t.add_edge(2, 6, True)
t.add_edge(2, 7, True)

t.add_edge(4, 8, True)

t.add_edge(5, 9, True)

t.add_edge(6, 10, True)

t.add_edge(9, 11, True)
t.add_edge(9, 12, True)

precond(0, t)


