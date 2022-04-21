import subprocess
import os
from pathlib import Path
import time

project_path = Path(__file__).absolute().parent
tests_path = str(project_path) + f'/tests/'

files_in = []
files_out = []

for f in os.listdir(tests_path):
    if os.path.isfile(os.path.join(tests_path, f)):
        if str(f)[-2:] == 'in':
            files_in.append(f)
        elif str(f)[-3:] == 'out':
            files_out.append(f)

passes = 0
fails = 0

files_in = sorted(files_in)
files_out = sorted(files_out)

for i in range(len(files_in)):
    start = time.perf_counter()
    
    test_path = tests_path + files_in[i]
    check_path = tests_path + files_out[i]

    output_path = tests_path + f'output_{files_in[i]}.result'

    input = open(test_path)
    output = open(output_path, 'w')

    exec = subprocess.run(
        [f"./airportmanager"], stdin=input, stdout=output)
    diff = subprocess.run(
        ["diff", check_path, output_path], stdout=subprocess.PIPE)

    end = time.perf_counter()

    if diff.returncode != 0:
        print(f"\nFAILED - Test {files_in[i]} - Time: {round(end - start), 3}s\n")
        print(diff.stdout.decode("utf-8"))
        fails += 1
    else:
        print(f"PASSED - Test {files_in[i]} - Time: {round(end - start, 3)}s")
        passes += 1

    os.remove(output_path)


print(f"""

--> Total Tests: {len(files_in)}

-> Passed: {passes}
-> Failed: {fails}
""")
