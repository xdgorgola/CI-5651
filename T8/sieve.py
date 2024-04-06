from math import sqrt

def sieve(n: int) -> list[int]:
    a = [i for i in range(2,n+1)]
    f = [1]
    #for i in range(2, int(sqrt(n)) + 1):
    for i in range(2, n):
        if n % i == 0:
            f.append(i)

        if not a[i - 2]:
            continue

        for j in range(i, (n // i) + 1):
            a[i*j - 2] = False

    return ([i for i in a if i and n % i == 0], f + [n])

def prime_fact(n: int) -> tuple[list[int], list[int]]:
    (p, d) = sieve(n)
    f = [0 for _ in range(len(p))]
    i = 0
    while n != 1:
        f[i] = f[i] + 1
        n = n // p[i]
        if n % p[i] != 0:
            i = i + 1
    return (p,f,d) 

print(prime_fact(360))

print(len([x for x in range(1,361) if 360%x==0]))