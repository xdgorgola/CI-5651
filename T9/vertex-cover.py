
def approxVC(G: list[tuple[int,int]]) -> list[int]:
    C: list[int] = []
    while len(G) > 0:
        e = G.pop()
        C.append(e[0])
        C.append(e[1])
        G = list(filter(lambda t: t[0] != e[0] and t[0] != e[1] and t[1] != e[0] and t[1] != e[1], G))
    return C

print(approxVC([(0,1), (1,2), (2,3), (2,4), (3,4), (3,5), (3,6), (4,5)]))