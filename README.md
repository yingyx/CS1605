<a id="readme-top"></a>
![GitHub last commit](https://img.shields.io/github/last-commit/yingyx/CS1605)

<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/yingyx/CS1605">
     <img height="80" src="https://cdn.jsdelivr.net/gh/devicons/devicon@latest/icons/cplusplus/cplusplus-original.svg" />
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