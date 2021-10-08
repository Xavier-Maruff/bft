# 🧠 bft
## A multi-target brainf*ck transpiler
bft is an optimizing modular brainf*ck transpiler which currently has backends for:
* C
* JavaScript

## Building
To build bft, ensure that you have the CMake and Boost program options library installed. Then run `./build.sh {{Debug or Release}}`

## Running
To transpile a brainf*ck program, simply run `bft {{input file}} -o {{output file}} --target={{target language}}`
Examples:
```
bft input.bf -o output.js --target=js
bft input.bf -o output.c --target=c
```