from random import randint
import numpy as np

def isInverse(A: np.ndarray, B: np.ndarray, k: int) -> bool:
    n: int = A.shape[0]
    I = np.identity(n)

    for _ in range(k):
        r = np.array([[randint(0,1) for _ in range(n)]])
        xab = (r @ A) @ B
        xi  = r @ I
        if ((xi != xab).any()):
            return False
    return True

r = isInverse(np.array([[4, 3], [3, 2]]), np.array([[-2, 3], [3, -4]]), 10)
print(r)