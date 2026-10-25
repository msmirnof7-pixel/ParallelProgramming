import numpy as np

A = np.loadtxt("matrix_a.txt", skiprows=1)
B = np.loadtxt("matrix_b.txt", skiprows=1)
C_cpp = np.loadtxt("result.txt", skiprows=1)

C_python = A @ B

if np.allclose(C_cpp, C_python, rtol=1e-5, atol=1e-5):
    print("VERIFICATION PASSED")
else:
    print("VERIFICATION FAILED")
    print("Expected:")
    print(C_python)
    print("Got:")
    print(C_cpp)
