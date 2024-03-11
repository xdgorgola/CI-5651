def lps(w: str) -> tuple[list[int], int]:
    b = [-1 for _ in range(len(w))]
    j = -1
    for i in range(1,len(w)):
        while j >= 0 and w[i-1] != w[j]:
            j = b[j]
        j = j + 1
        b[i] = j
        if j == 0:
            ps = i
    return "" if w[0] != w[ps] and ps > 0 else w[ps:]

ws = ["SEVENTY SEVEN ", "abracadabra", "arepera", "algoritmo"]
for w in ws:
    print(lps(w))