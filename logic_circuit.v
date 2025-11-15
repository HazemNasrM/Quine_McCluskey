// Function: F = B'CD' + BC' + A'D + A'C
// Generated using Verilog primitives.

module logic_function (
input  A,
input  B,
input  C,
input  D
,
output F
);
// Wires for inverted inputs
wire not_A;
wire not_B;
wire not_C;
wire not_D;
// Wires for product terms (AND gate outputs)
wire term_0;
wire term_1;
wire term_2;
wire term_3;

// --- Input Inverters ---
not inv_A (not_A, A);
not inv_B (not_B, B);
not inv_C (not_C, C);
not inv_D (not_D, D);

// --- Product Terms (AND gates) ---
// Term 0: B'CD'
and and_0 (term_0, not_D, C, not_B);
// Term 1: BC'
and and_1 (term_1, not_C, B);
// Term 2: A'D
and and_2 (term_2, D, not_A);
// Term 3: A'C
and and_3 (term_3, C, not_A);

// --- Sum Term (OR gate) ---
or or_final (F, term_0, term_1, term_2, term_3);

endmodule
