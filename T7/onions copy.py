from functools import cmp_to_key

import matplotlib as mpl
import matplotlib.pyplot as plt
import random

type vector = tuple[float, float]

def add_vect(v1: vector, v2: vector) -> vector:
    return (v1[0]+v2[0], v1[1]+v2[1])


def sub_vect(v1: vector, v2: vector) -> vector:
    return (v1[0]-v2[0], v1[1]-v2[1])


def vector_from_points(fp: vector, tp: vector):
    return sub_vect(tp, fp)


def cross_sign(v1: vector, v2: vector) -> float:
    return v1[0] * v2[1] - v2[0] * v1[1]


# > 0 es giro izq. < 0 es giro der.
def points_rotation(p1: vector, p2: vector, p3: vector) -> float:
    return cross_sign(vector_from_points(p1, p2), vector_from_points(p2, p3))


# Esta p1 a la derecha de p2?
# > 0 a la der. < 0 a la izq.
def is_left(p1: vector, p2: vector) -> tuple[bool, float]:
    cs = cross_sign(p1, p2)
    return (cs > 0, cs)


def lowest_right_point(points: list[vector]) -> vector:
    lr, li, i = points[0], 0, 0
    for p in points:
        if p[1] < lr[1] or (p[1] == lr[1] and p[0] > lr[0]):
            lr = p
            li = i
        i = i + 1
    return li


def sqrd_distance(p0: vector, p1: vector):
    return (p0[0] - p1[0])**2 + (p0[1] - p1[1])**2


def polar_comparator(p1, p2, p3):
    v1 = vector_from_points(p1, p2)
    v2 = vector_from_points(p1, p3)
    il, mag = is_left(v1, v2)
    if mag == 0: # Coplanares. Revisar como quiero devolverlo
        return 1 if sqrd_distance(p1, p3) >= sqrd_distance(p1, p2) else -1
        
    return -1 if il else 1


def graph_ch(ch: list[vector], points: list[vector], show: bool = False):
    plt.scatter([p[0] for p in points], [p[1] for p in points])
    plt.scatter(ch[0][0], ch[0][1], c='red')
    for i in range(1,len(ch)):
        plt.plot([ch[i-1][0], ch[i][0]], [ch[i-1][1], ch[i][1]], c='magenta')

    plt.plot([ch[-1][0], ch[0][0]], [ch[-1][1], ch[0][1]], c='magenta')
    if show:
        plt.show()
        

def convex_hull(points: list[vector]) -> list[vector]:
    ch, nch = [], []
    points = [p for p in points]
    p = points.pop(lowest_right_point(points)) # O(n)
    points = sorted(points, key=cmp_to_key(lambda p1,p2: polar_comparator(p,p1,p2))) # O(n log n)
    
    # Dibujo de ordenamiento polar
    plt.scatter(p[0], p[1], c = 'red')
    plt.scatter([p[0] for p in points], [p[1] for p in points])
    plt.annotate("lrm", p)
    for i in range(len(points)):
        plt.annotate(i, points[i])  
    plt.show()

    # Chequeo de giros!
    ch.append(p)
    ch.append(points[0])
    for i in range(1,len(points)):
        while cross_sign(vector_from_points(points[i-1], points[i]), vector_from_points(p, ch[-1])) < 0:
            
            plt.plot([p[0], ch[-1][0]], [p[1], ch[-1][1]])
            plt.plot([points[i-1][0], points[i][0]], [points[i-1][1], points[i][1]])
            for i in range(1,len(ch)):
                plt.plot([ch[i-1][0], ch[i][0]], [ch[i-1][1], ch[i][1]], c='magenta')
            plt.scatter(p[0], p[1], c = 'red')
            plt.scatter([p[0] for p in points], [p[1] for p in points])
            plt.show()
            print(cross_sign(vector_from_points(points[i-1], points[i]), vector_from_points(p, ch[-1])))
            nch.append(ch.pop())
        plt.plot([p[0], ch[-1][0]], [p[1], ch[-1][1]])
        plt.plot([points[i-1][0], points[i][0]], [points[i-1][1], points[i][1]])
        for i in range(1,len(ch)):
            plt.plot([ch[i-1][0], ch[i][0]], [ch[i-1][1], ch[i][1]], c='magenta')
        plt.scatter(p[0], p[1], c = 'red')
        plt.scatter([p[0] for p in points], [p[1] for p in points])
        plt.show()
        print(cross_sign(vector_from_points(points[i-1], points[i]), vector_from_points(p, ch[-1])))
        ch.append(points[i])
    return ch, nch
    

def onion_ch(points: list[vector]) -> int:
    layers = 0
    chs = []
    while len(points) >= 3:
        ch, nch = convex_hull(points)
        chs.append(ch)
        graph_ch(ch, points, True)

        points = nch
        layers = layers + 1

    for ch in chs:
        graph_ch(ch, ch, False)

    plt.show()
    return layers

pr, pn = 100, 25
points = [(random.randint(-pr, pr), random.uniform(-pr, pr)) for _ in range(pn)]
# Caso problematico
#from numpy import arange
#points = [(a,0) for a in arange(0, 10, 2.5)] + [(-a,0) for a in arange(0, 10, 2.5)] + [(0,a) for a in arange(0, 10, 2.5)] + [(0,-a) for a in arange(0, 10, 2.5)]
print(onion_ch(points))



