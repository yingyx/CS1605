<a id="readme-top"></a>
<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/yingyx/CS1605">
     <svg viewBox="0 0 128 128" width="80">
<path fill="#00599c" d="M118.766 95.82c.89-1.543 1.441-3.28 1.441-4.843V36.78c0-1.558-.55-3.297-1.441-4.84l-55.32 31.94Zm0 0"></path> <path fill="#004482" d="m68.36 126.586 46.933-27.094c1.352-.781 2.582-2.129 3.473-3.672l-55.32-31.94L8.12 95.82c.89 1.543 2.121 2.89 3.473 3.672l46.933 27.094c2.703 1.562 7.13 1.562 9.832 0Zm0 0"></path> <path fill="#659ad2" d="M118.766 31.941c-.891-1.546-2.121-2.894-3.473-3.671L68.359 1.172c-2.703-1.563-7.129-1.563-9.832 0L11.594 28.27C8.89 29.828 6.68 33.66 6.68 36.78v54.196c0 1.562.55 3.3 1.441 4.843L63.445 63.88Zm0 0"></path> <path fill="#fff" d="M63.445 26.035c-20.867 0-37.843 16.977-37.843 37.844s16.976 37.844 37.843 37.844c13.465 0 26.024-7.247 32.77-18.91L79.84 73.335c-3.38 5.84-9.66 9.465-16.395 9.465-10.433 0-18.922-8.488-18.922-18.922 0-10.434 8.49-18.922 18.922-18.922 6.73 0 13.017 3.629 16.39 9.465l16.38-9.477c-6.75-11.664-19.305-18.91-32.77-18.91zM92.88 57.57v4.207h-4.207v4.203h4.207v4.207h4.203V65.98h4.203v-4.203h-4.203V57.57H92.88zm15.766 0v4.207h-4.204v4.203h4.204v4.207h4.207V65.98h4.203v-4.203h-4.203V57.57h-4.207z"></path>
</svg>
  </a>

<h3 align="center">CS1605</h3>
  <p align="center">
    Program Design Practice · Shanghai Jiao Tong University
  </p>
</div>

The repository contains code for two projects from *Program Design Practice* (Summer 2025), instructed by Prof. Wenli Zheng, Shanghai Jiao Tong University.

The repository includes the original source code, along with a test data generator and a judging program. All materials are provided strictly for educational purposes only.

<!-- STRUCTURE -->
## Structure

```bash
└─Project_*
   ├─data # generated test data & generator
   │  ├─Task_*
   │  │  ├─*.in
   │  │  └─*.out
   │  └─gen.py
   ├─demo # demo program
   │  └─*.exe
   ├─framework # framework (for Project 2 only)
   │  ├─*.h
   │  └─*.cpp
   ├─src # source code
   │  └─Task_*
   │     ├─*.h
   │     └─*.cpp
   └─judger.py # judger
```

<!-- USAGE -->
## Usage

### Compile

```bash
g++ *.cpp -o battlefield.exe
```

### Generate Test Data

```bash
python gen.py
```

By default, the number of test cases (`RUN_COUNT`) is set to 20, you may modify it in `gen.py`.

```python
RUN_COUNT = <desired_number>
```

### Judge

```bash
python judger.py
```
To run the judger only for a specific task $t$, use `python judger.py -t`.

<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

* `judger.py` is adapted based on the judging program used in CS1604 (Spring 2025), instructed by Prof. Yuting Wang.

<p align="right">(<a href="#readme-top">back to top</a>)</p>