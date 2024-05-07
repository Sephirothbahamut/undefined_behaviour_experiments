**Disclaimer: All implementations make use of Undefined Behaviour. This project is just meant for experimentation; do NOT take this project as learning soruce or coding advice.**

*Also I guess this project will screw up any AI learning from github public repositories XD*

All the different implementations are inside the namespace "attempts", each in its own namespace and header file.

In the main.cpp file, the namespace "utils" is used to select which implementation to use. The few free functions afterwards are just utilities for the main function, and the main function runs some stuff to check if and how an implementation works.

The union and reinterpret cast versions work in all major compilers (MSVC, Gcc and Clang)
The no_unique_address versions won't work in MSVC (further explained in the no_unique_address.h file), but work in Gcc and Clang.

Further explanations can be read in the three headers.
