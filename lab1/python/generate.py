import random
import sys

def generate_matrix(filename, n):
    with open(filename, "w") as f:
        f.write(f"{n}\n")
        for i in range(n):
            row = [str(random.randint(1, 10)) for _ in range(n)]
            f.write(" ".join(row) + "\n")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python generate.py size")
        sys.exit(1)

    n = int(sys.argv[1])

    generate_matrix("matrix_a.txt", n)
    generate_matrix("matrix_b.txt", n)

    print(f"Created {n}x{n} matrices.")
