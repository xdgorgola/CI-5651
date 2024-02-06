import numpy as np

def elevar_matriz(mat, pow):
    if pow == 0:
        return np.identity(3)
    
    p = elevar_matriz(mat, pow // 2)
    if pow % 2 == 0:
        return np.matmul(p, p)
    
    return np.matmul(np.matmul(mat, p), p)


def perrin(n: int):
    if n == 0:
        return 3
    if n == 1:
        return 0
    if n == 2:
        return 2
    
    R = np.array([[0,1,1],[1,0,0],[0,1,0]])
    V = np.array([2,0,3])

    return int(np.matmul(elevar_matriz(R, n-2), V)[0])