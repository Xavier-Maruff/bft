# 🧠 bft
## A multi-target brainf*ck transpiler
bft is an optimizing modular brainf*ck transpiler which currently has backends for:
* C
* JavaScript

## Building
Dependencies:
* CMake
* Make
* Boost program options

To build bft, run `./build.sh {{Debug or Release}}`

## Running
To transpile a brainf*ck program, simply run `bft {{input file}} -o {{output file}} --target={{target language}}`
Examples:
```
bft input.bf -o output.js --target=js
bft input.bf -o output.c --target=c
```