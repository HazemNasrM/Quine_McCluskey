# Quine-McCluskey Algorithm Implementation

A C++ implementation of the Quine-McCluskey algorithm for minimizing Boolean functions. This program generates minimized sum-of-products (SOP) expressions and produces synthesizable Verilog code using primitives.

## Features

- **Boolean Function Minimization**: Uses the Quine-McCluskey tabular method to find all prime implicants
- **Essential Prime Implicant Detection**: Automatically identifies and selects essential prime implicants
- **Don't Care Support**: Handles don't care conditions for further optimization
- **Verilog Generation**: Automatically generates structural Verilog code using NOT, AND, and OR primitives
- **Detailed Output**: Shows prime implicant charts, essential prime implicants, and step-by-step minimization process

## Input Format

Create a text file with the following format:

```
number_of_variables
comma_separated_minterms
comma_separated_dont_cares
```

### Example Input Files

**Basic example** (4 variables):
```
4
m1, m3, m4, m11, m13
d2, d9, d15
```

**Edge cases**:
- Empty minterms: Use `m` for tautology (always 0)
- Empty don't cares: Use `d` for no don't cares
- Maxterms: Use `M` instead of `m` for product-of-sums form

## Usage

### Compilation

```bash
g++ -o main main.cpp 
```

### Running

```bash
./main
```

When prompted, enter the input filename (with or without `.txt` extension):
```
Input File: test
```

The program will search for the file in multiple locations:
- Current working directory
- Source code directory
- Executable directory
- Absolute path if provided

### Output

The program generates:
1. **Console output** (`out.txt` via redirection):
   - Prime implicant chart
   - Essential prime implicants
   - Step-by-step minimization process
   - Final minimized Boolean expression

2. **Verilog file** (`logic_circuit.v`):
   - Synthesizable Verilog module
   - Uses only primitive gates (NOT, AND, OR)
   - Fully combinational logic implementation

## Test Cases

The `Test Inputs/` folder contains 10 test cases covering:

- **input1**: Tautology (edge case - always true)
- **input2**: Contradiction (edge case - always false)
- **input3**: Minterms with don't cares
- **input4**: Maxterms with don't cares
- **input5**: Full adder sum function
- **input6**: Full adder carry-out function
- **input7**: 10-bit exactly-two-ones detector
- **input8**: 12-bit equality comparator
- **input9**: 16-bit odd parity generator
- **input10**: 20-bit majority function

## Algorithm Overview

1. **Parse Input**: Read minterms and don't cares from file
2. **Generate Prime Implicants**: 
   - Group terms by number of 1's
   - Iteratively combine terms differing by one bit
   - Mark combined terms and track dashes (don't cares)
3. **Create Prime Implicant Chart**: Matrix showing which minterms each PI covers
4. **Find Essential Prime Implicants**: PIs that are the only cover for at least one minterm
5. **Select Remaining PIs**: Greedily or exhaustively choose PIs to cover remaining minterms
6. **Generate Output**: Convert to Boolean expression and Verilog code

## Code Structure

```
main.cpp
├── Data Structures
│   └── binaryInt: Represents numbers with don't care positions
├── Input Parsing
│   ├── takeInput(): Reads and validates input file
│   └── toBinaryInt(): Converts decimal strings to binary representation
├── Prime Implicant Generation
│   ├── getPrimeImplicants(): Main QM algorithm
│   ├── are1BitOff(): Checks if two terms differ by one bit
│   └── combine(): Merges compatible terms
├── Chart Operations
│   ├── createPrimeImplicantChart(): Builds coverage matrix
│   └── getEssentialPrimeImplicants(): Finds EPIs
├── Minimization
│   └── generateExpression(): Selects final set of PIs
├── Output Generation
│   ├── toBooleanExpression(): Converts to algebraic form
│   ├── displayPrimeImplicantChart(): Formatted chart display
│   └── generateVerilogFile(): Creates .v file with primitives
```

## Technical Details

- **Variable Limit**: Supports up to 31 variables (limited by 32-bit unsigned integers).
- **Don't Care Representation**: Uses separate bitfield for dash positions
- **Minimization Strategy**: Essential PIs first, then greedy selection by coverage count
- **Verilog Output**: Two-level logic (AND-OR) using Verilog primitives

## Requirements

- C++17 or later
- Standard library with `<filesystem>` support
- Compiler: GCC, Clang, or MSVC

## Limitations

- Greedy selection may not always find the absolute minimum in complex cases
- Large functions (>20 variables) may have performance/memory constraints
- Verilog output uses two-level logic (may not be optimal for all synthesis tools)

## Authors

Ebram Hanin, Hazem Nasr, and Mohamed Mohamed 
American University in Cairo  
CSCE 2301 - Digital Design I  
Fall 2025

## License

Educational project - free to use and modify.
