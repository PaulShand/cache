# Cacher.c - Cache Implementation Overview

## Introduction

`cacher.c` is a comprehensive implementation of cache mechanisms in C, featuring three distinct cache designs: FIFO (First-In, First-Out), LRU (Least Recently Used), and Clock. This implementation simulates caching behavior using doubly linked lists and provides insights into the workings of different caching algorithms.

## Implementation Details

### Cache Policies Implemented

1. **FIFO**: Implements the First-In, First-Out caching strategy.
2. **LRU**: Uses the Least Recently Used algorithm for cache management.
3. **Clock**: Incorporates the Clock caching mechanism.

### Core Components

- **Main Function**: Validates input arguments and calls the respective cache function based on the policy (FIFO, LRU, Clock).
- **Cache Functions**: Each function (FIFO, LRU, Clock) handles cache operations depending on the capacity and specific algorithm rules.
- **Double Linked List**: Used to store cache items, with additional variables for LRU and Clock operations.
- **Capacity Management**: Checks whether the cache capacity has been reached and replaces items based on the policy.
- **Node Structure**: Each node in the doubly linked list has `value` and `reference` variables for LRU and Clock usage, respectively.

### Optimization and Testing

- **Compulsory and Capacity Variables**: Track cache misses, differentiating between compulsory and capacity misses.
- **All Items Array**: Maintains a record of all cached items, capped at 10,000 entries.
- **Testing**: Includes various test cases and edge case evaluations, ensuring the robustness of the implementation.
- **Memory Management**: Verified with Valgrind to ensure no memory leaks.

This project offers valuable insights into cache algorithms and their implementations, serving as a practical exploration of caching strategies in computer systems.
