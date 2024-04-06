from random import randint
import numpy as np

def isInverse(A: np.ndarray, B: np.ndarray, k: int):
    n: int = A.shape[0]
    r = np.array([[randint(0,1) for _ in range(n)]])
    I = np.identity(n)

    for _ in range(k):
        xab = r @ A @ B # Quizas hacer manual
        xi  = r @ I     # Quizas hacer manual
        if ((xi != xab).all()):
            return False
    return True


r = isInverse(np.array([[4, 3], [3, 2]]), np.array([[-2, 3], [3, -4]]), 20)
