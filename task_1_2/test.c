# test.py
import subprocess
import sys

tests = [
    (["main.exe", "/?"], 0),      # Ожидаем код 0
    (["main.exe"], 1),            # Ожидаем код 1
    (["main.exe", "C:\\test", "*.txt"], 0),
]

for args, expected_code in tests:
    result = subprocess.run(args)
    if result.returncode == expected_code:
        print(f"✓ {args} - PASS")
    else:
        print(f"✗ {args} - FAIL (got {result.returncode}, expected {expected_code})")
        sys.exit(1)
