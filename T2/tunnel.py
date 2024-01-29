def carteles(car : [(int, int)]):
    ap = 0; r = []
    s = sorted(car, key=lambda t: sum(t))

    for c in s:
        if c[0] < ap:
            continue
        r.append(c)
        ap = sum(c)

    return r