import subprocess
import random
import re
import os

# ======= 配置区域 =======
EXES = [
    {"path": r"../demo/Task1.exe", "save_dir": r"./Task_1"},
    {"path": r"../demo/Task2.exe", "save_dir": r"./Task_2"},
    {"path": r"../demo/Task3.exe", "save_dir": r"./Task_3"},
]
RUN_COUNT = 20
INPUT_OPTIONS = ['1', '2', '3']
END_MARKERS = ["You Win", "You Lose", "Draw"]

def line_contains_select(line: str):
    return "Select" in line and "(" in line and ")" in line

def run_game(exe_path, save_dir, run_id):
    os.makedirs(save_dir, exist_ok=True)

    proc = subprocess.Popen(
        [exe_path],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        bufsize=0,
        text=True,
        universal_newlines=True,
    )

    input_log = []
    output_log = []
    buffer = ""

    try:
        while True:
            char = proc.stdout.read(1)
            if char == '':
                break

            buffer += char
            output_log.append(char)
            print(char, end='', flush=True)

            if any(marker in buffer for marker in END_MARKERS):
                break

            lines = re.split(r'[\r\n]', buffer)
            last_line = lines[-1]

            if line_contains_select(last_line):
                choice = random.choice(INPUT_OPTIONS)
                proc.stdin.write(choice + '\n')
                proc.stdin.flush()
                input_log.append(choice)
                buffer = ""

    finally:
        with open(os.path.join(save_dir, f"{run_id}.in"), "w") as f_in:
            f_in.write("\n".join(input_log))
        with open(os.path.join(save_dir, f"{run_id}.out"), "w", encoding='utf-8') as f_out:
            f_out.write("".join(output_log))

        proc.kill()
        proc.wait()

for exe in EXES:
    print(f"== Running {exe['path']} ==")
    for i in range(1, RUN_COUNT + 1):
        print(f"-- Run {i}")
        run_game(exe['path'], exe['save_dir'], i)