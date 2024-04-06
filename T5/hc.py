import math

INF     = 9999999
DUMMY   = 0

def es_primo(x):
    if x <= 1:
        return False
    
    if x == 2 or x == 3:
        return True

    for i in range(2, int(math.sqrt(x)) + 1):
        if x % i == 0:
            return False

    return True


def bfs(uc, adjs, dist, matchU, matchV):
    q = []
    for u in range(1, uc+1):
        if matchU[u] == DUMMY:      # Si el nodo de U no ha sido emparejado, hacemos BFS desde el
            dist[u] = 0
            q.append(u)
            continue
        dist[u] = INF

    dist[DUMMY] = INF
    while len(q) > 0:
        u = q.pop(0)
        if dist[u] == dist[DUMMY]:
            continue

        for v in adjs[u]:
            if dist[matchV[v]] == INF:
                dist[matchV[v]] = dist[u] + 1
                q.append(matchV[v])

    return dist[DUMMY] != INF  # Si llegamos al dummy del lado V, aun hay camino en aumento!


def dfs(u, adjs, dist, matchU, matchV):
    if u == DUMMY:
        return True
        
    # Recorremos los caminos generados por el BFS hasta encontrar un vertice no emparejado
    for v in adjs[u]:
        if dist[matchV[v]] == dist[u] + 1 and dfs(matchV[v], adjs, dist, matchU, matchV):
            matchV[v] = u
            matchU[u] = v
            return True
            
    dist[u] = INF  # No se encontro forma de swappear :(
    return False

 
def eliminar_suma_primos(C : list[int]):
    u, v = [], [] 
    uId, vId = 1, 1

    # Separamos pares e impares. Cada uno es un vertice.
    for c in C:
        if c % 2 == 0:
            print(c, "par", uId)
            u.append((c, uId))
            uId += 1
        else:
            print(c, "impar", vId)
            v.append((c, vId))
            vId += 1

    uc, vc  = len(u), len(v)
    dist    = [DUMMY for _ in range(uc+1)]
    matchU  = [DUMMY for _ in range(uc+1)]
    matchV  = [DUMMY for _ in range(vc+1)]
    adjs    = [[]] + [[v[1] for v in v if es_primo(v[0] + u[0])] for u in u] # Hacemos adyacencias entre nodos cuya suma generen primos
    print(adjs)
    matchs  = 0
    while bfs(uc, adjs, dist, matchU, matchV): # BFS para obtener caminos en aumento
        for u in range(1, uc+1):               # Revisamos los vertices del lado U a ver si tienen caminos en aumento
            if matchU[u] == DUMMY and dfs(u, adjs, dist, matchU, matchV):  # Conseguimos un camino aunque sea :) y sabemos que siempre aumenta en 1
                print(u, matchU[u])
                matchs += 1

    return matchs

print(eliminar_suma_primos([1,2,3,4,5]))