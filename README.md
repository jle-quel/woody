# Woody Woodpacker 

## Introduction

This project is part of the System/Unix cursus at 42.

The idea is quite simple, the packer will receive an executable (ET_EXEC or ET_DYN), and create a new binary that has the same comportement.

## Installation

Must be run on a Linux distribution

```
make
./woody_woodpacker binary_example
./woody
```

## Description

The step are represented like this:
- verification of the given file
- mapping of binary into memory
- modification of segment
- modification of section
- modification of header + signature (sign the binary as packed)
- creation of new file and code injection (payload)

### Creation of the new binary

The modification of the given file is the implementation of the segment padding infection (Silvio Cesare's infection).
The creation of the file will encrypt all of the section text.
It will not encrypt the section fini, which is where the payload will reside.

### Execution of the new binary

At this point the text section is crypted, so if you wanted to execute this code, you will crash.
But in the modification of the header we changed the entry_point to our payload.

So when the TEXT segment is loaded in memory, when control is given to the binary, it will first execute the payload.
It will print "...WOODY...", then the payload will unencrypt the text section and finally jump back to the original entry_point to continue the original flow of the binary.
When the binary will exit, the file on disk will still be uncrypted.

## Author

Jefferson Le Quellec 🐜
