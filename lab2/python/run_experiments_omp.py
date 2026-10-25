import subprocess
import os
import csv
from pathlib import Path
import shutil

def run_cmd(cmd, env=None):
    result = subprocess.run(cmd, capture_output=True, text=True, env=env or os.environ)
    return result.stdout, result.stderr, result.returncode

def parse_metrics(output):
    metrics = {}
    for line in output.split('\n'):
        if ':' in line:
            key, val = line.split(':', 1)
            metrics[key.strip()] = val.strip()
    return metrics

def main():
    sizes = [400, 800, 1200, 1600, 2000]
    thread_counts = [1, 2, 4, 8]
    
    print("🔨 Компиляция OpenMP версии...")
    run_cmd(['g++', '-O3', '-fopenmp', '-o', 'build/matrix_mult_omp', '../src/main_omp.cpp'])
    
    Path('../results').mkdir(exist_ok=True)
    results_data = []
    
    for size in sizes:
        print(f"\n🧪 Тестирование размера матрицы: {size}x{size}")
        run_cmd(['python', 'generate.py', str(size)])
        
        row_data = {'size': size}
        
        for threads in thread_counts:
            print(f"  ⏱  OpenMP ({threads} потоков)...")
            env = os.environ.copy()
            env['OMP_NUM_THREADS'] = str(threads)
            
            out, err, code = run_cmd(
                ['../build/matrix_mult_omp', 'matrix_a.txt', 'matrix_b.txt', 'result_omp.txt'], 
                env=env
            )
            omp_metrics = parse_metrics(out)
            omp_time = float(omp_metrics.get('TIME_SECONDS', 0))
            
            row_data[f'time_{threads}t'] = omp_time
            print(f"     Время: {omp_time:.6f} сек")
        
        shutil.copy('result_omp.txt', '../result.txt')
        out, err, code = run_cmd(['python', '../verify.py'])
        row_data['verification'] = 'PASSED' if 'PASSED' in out else 'FAILED'
        print(f"  ✅ Верификация: {row_data['verification']}")
        
        results_data.append(row_data)

    csv_path = Path('../results/results_omp.csv')
    with open(csv_path, 'w', newline='', encoding='utf-8') as f:
        writer = csv.DictWriter(f, fieldnames=results_data[0].keys())
        writer.writeheader()
        writer.writerows(results_data)
    
    print(f"\n🎉 Готово! Результаты сохранены в {csv_path}")

if __name__ == "__main__":
    Path('../build').mkdir(exist_ok=True)
    main()