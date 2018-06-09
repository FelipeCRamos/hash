# Hash Table
This project consists in the implementation of an abstract data type called **hash table**, for the Basic Data Structure I course on UFRN.

In computing, a **hash table** (_hash map_) is a **data structure** which implements an associative array abstract data type, a structure that can map keys to values.

A hash table uses a hash function to cumpute an index into an array of buckets or slots, from which the desired value can be found.

Ideally, the hash function will assign each key to a unique bucket, but most hash tables designs employ an imperfect hash function, which might cause hash _collisions_ where the hash function generates the same index for more than one key. Such collisions must be accommodated in some way. That's our objective.

## Dependencies
In order to compile and execute this project, you need to have a C++ compiler in your choosen operational system. For this tutorial, i will demonstrate on a Linux distro.

So, you will need:
+ g++ compiler (install via terminal: `sudo apt-get install g++`)
+ git utility (install via terminal: `sudo apt-get install git`)

### Clone and compile
First of all, you will need to clone this git repo onto your computer. You can do-it by typing on terminal:
```bash
git clone https://github.com/FelipeCRamos/hash.git
cd hash
```
And to compile, type inside the hash generated folder:
```bash
make
```

## Execution
Since this project consists in a class implementation, i will provide a driver code that will run mostly of classes methods for a consistent test.

So, in order to execute the driver code, just type onto terminal after compile:
```bash
./hash-table
```

## Authorship
This entire project was made by Felipe Ramos, a computer science student at _Universidade Federal do Rio Grande do Norte_.

All this project is under the **MIT License**.