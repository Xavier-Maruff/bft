# 🧠 bft
## A multi-target brainf*ck transpiler
bft is an optimizing modular brainf*ck transpiler which currently has backends for:
* C
* JavaScript for the NodeJS runtime
* Rust

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
bft input.bf -o output.rs --target=rust
```

## Testing
Run the `test.sh` script to run the C, JavaScript, and Rust tests. DISCLAIMER: You will need to modify the `libc` crate path in the `test/rust/test.sh` script for rustc to correctly include it, see the *Rust notes* section below

## Rust notes
The rust target references the `libc` crate, so to compile either create a Cargo project and add `libc` to the dependencies, or compile with `rustc` with the `--extern libc={path to libc rlib}`
