# 🧠 bft
## A multi-target brainf*ck transpiler
bft is an optimizing modular brainf*ck transpiler which currently has backends for:
* C
* JavaScript for the NodeJS runtime
* Rust
* Ada

## Command line options
bft accepts the following command-line arguments:
* --target={js, rust, c, or ada}, -t {js, rust, c, or ada} -> the transpilation target langauge
* positional, --source={source file}, -s {source file} -> the source brainf*ck file
* --verbose, -v -> increases the logging verbosity
* --output={output file}, -o {output file} -> the transpiled output file
* --help, -h -> displays the options guide

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
bft input.bf -o output.adb --target=ada
```

## Testing
Run the `test.sh` script to run the C, JavaScript, and Rust tests. DISCLAIMER: You will need to modify the `libc` crate path in the `test/rust/test.sh` script for rustc to correctly include it, see the *Rust notes* section below

## Rust notes
The rust target references the `libc` crate, so to compile either create a Cargo project and add `libc` to the dependencies, or compile with `rustc` with `--extern libc={path to libc rlib}`
