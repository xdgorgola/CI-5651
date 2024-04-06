from random import uniform
from math import ceil

def BTest(a: int, n: int):
    s: int = 0
    t: int = n - 1
    while True:
        s = s + 1
        t = t // 2
        if t % 2 == 1:
            break
    x: int = pow(a, t, n)
    if x == 1 or x == n-1:
        return True
    
    for _ in range(1, s):
        x = pow(x, 2) % n
        if x == n - 1:
            return True
    return False

def millRab(n: int) -> bool:
    a: int = ceil(uniform(2,n-2))
    print(f'mra: {a}')
    return BTest(a, n)


def millRabRep(n: int, k: int) -> bool:
    for _ in range (0, k):
        mrn: bool = millRab(n)
        print(f'k: {k} mrn: {mrn}')
        if not mrn:
            return False
    return True

print(f'{millRabRep(int("1511264"*3),10)}')