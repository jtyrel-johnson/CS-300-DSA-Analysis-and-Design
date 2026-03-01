# CS-300-DSA-Analysis-and-Design
## Project Overview
In this course, I completed two major projects focused on data structures and algorithm analysis.
- Project One: Analyze and compare the run-time and memory complexity of a vector, hash table, and binary search tree (BST) for storing and retrieving course data.
- Project Two: Implement a working C++ program that loads course data from a CSV file, stores it in a binary search tree, and prints the courses in alphanumeric order.

Together, these projects demonstrate my ability to evaluate algorithm efficiency, justify architectural decisions using Big-O analysis, and implement data structures in C++ to solve real-world organizational problems.

### What Problem Was I Solving?
The core problem was designing an advising assistance program for ABCU that could:
- Load course data from a file.
- Print a list of all Computer Science courses in alphanumeric order.
- Display detailed information (including prerequisites) for a specific course.

The challenge was not just making the program work — it was choosing the right data structure to ensure efficient searching, sorting, and storage.
This required balancing:
- Run-time complexity
- Memory usage
- Maintainability
- Scalability for future growth

### How I Approached the Problem
I approached the problem analytically before writing code.
First, I evaluated three data structures:

| Data Structure |	Search	| Insert	| Sorted Output |	Memory Characteristics |
|----------------|----------|---------|---------------|------------------------|
|     Vector	   |    O(n)	|   O(n)	|O(n log n) (if sorted)|	Contiguous, cache-friendly|
|Hash Table|	O(1) avg / O(n) worst|	O(1) avg|	Not naturally sorted|	Extra memory for buckets|
|BST|	O(log n) |avg / O(n) worst	|O(log n) avg	|O(n) (in-order traversal)	|Pointer-based structure|

Because one of the primary requirements was printing courses in alphanumeric order, the Binary Search Tree was the most appropriate choice. It naturally maintains order and allows efficient searching without requiring a separate sorting step.

Understanding data structures was critical here. The same program could function with any structure, but performance and scalability would differ significantly. This project reinforced that good software design begins with selecting the correct data structure for the requirements.

### Roadblocks and How I Overcame Them
I encountered several technical challenges:
- Implementing recursive traversal correctly (especially in-order traversal for sorted output).
- Designing clean file parsing logic for CSV input.
- Managing dynamic memory safely (constructors, destructors, and pointer management).
- Handling edge cases such as missing prerequisites or malformed input data.
  
To overcome these challenges, I:
- Broke problems into smaller testable pieces.
- Used pseudocode before implementing logic.
- Revisited Big-O principles to confirm algorithm efficiency.
- Refactored code to simplify recursive calls and improve readability.
One of the biggest breakthroughs was fully understanding how recursion works in tree traversal. Once I grasped that in-order traversal naturally produces sorted output in a BST, the design became much cleaner.

### This course fundamentally changed how I think about software design.
Previously, I focused primarily on whether code worked. Now I evaluate:
- Is this scalable?
- What is the worst-case complexity?
- How does memory usage compare?
- Is this structure appropriate for the problem domain?

I now approach problems by:
- Analyzing requirements first.
- Comparing possible data structures.
- Justifying the choice using complexity analysis.
- Designing before implementing.

This is a much more professional and engineering-focused mindset.

### My approach to writing maintainable and adaptable code evolved significantly:
1. Modularity
   
I separated concerns (file loading, tree insertion, traversal, display). This makes the program easier to extend or modify.

2. Readability

I used:
- Clear function names
- Consistent indentation
- Comments that explain why, not just what
- Structured control flow

3. Error Handling

I implemented input validation and file-open checks to prevent crashes.

4. Adaptability

Because I abstracted the data structure operations, the system could be refactored later to use a different structure (such as a balanced tree) with minimal changes to higher-level logic.
