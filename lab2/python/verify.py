import numpy as np

try:
    # Читаем размер матрицы из первой строки файла результата
    with open("result.txt", "r") as f:
        n = int(f.readline().strip())
    
    # Загружаем ровно N строк, пропуская первую строку с размером
    A = np.loadtxt("matrix_a.txt", skiprows=1, max_rows=n)
    B = np.loadtxt("matrix_b.txt", skiprows=1, max_rows=n)
    C_cpp = np.loadtxt("result.txt", skiprows=1, max_rows=n)
    
    # Умножаем в Python
    C_python = A @ B
    
    # Сравниваем с допуском 1e-3 (достаточно для float вычислений)
    if np.allclose(C_cpp, C_python, rtol=1e-3, atol=1e-3):
        print("VERIFICATION PASSED")
    else:
        print("VERIFICATION FAILED")
        print("Максимальная ошибка:", np.max(np.abs(C_cpp - C_python)))
        
except Exception as e:
    print("ОШИБКА:", e)