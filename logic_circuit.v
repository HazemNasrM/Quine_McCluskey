// Function: F = F'G'H + E'GH' + FH' + EG' + B'D + A'C + BD' + AC' + DF' + CE' + B'EH + A'FG
// Generated using Verilog primitives.

module logic_function (
input  A,
input  B,
input  C,
input  D,
input  E,
input  F,
input  G,
input  H
,
output F
);
// Wires for inverted inputs
wire not_A;
wire not_B;
wire not_C;
wire not_D;
wire not_E;
wire not_F;
wire not_G;
wire not_H;
// Wires for product terms (AND gate outputs)
wire term_0;
wire term_1;
wire term_2;
wire term_3;
wire term_4;
wire term_5;
wire term_6;
wire term_7;
wire term_8;
wire term_9;
wire term_10;
wire term_11;

// --- Input Inverters ---
not inv_A (not_A, A);
not inv_B (not_B, B);
not inv_C (not_C, C);
not inv_D (not_D, D);
not inv_E (not_E, E);
not inv_F (not_F, F);
not inv_G (not_G, G);
not inv_H (not_H, H);

// --- Product Terms (AND gates) ---
// Term 0: F'G'H
and and_0 (term_0, H, not_G, not_F);
// Term 1: E'GH'
and and_1 (term_1, not_H, G, not_E);
// Term 2: FH'
and and_2 (term_2, not_H, F);
// Term 3: EG'
and and_3 (term_3, not_G, E);
// Term 4: B'D
and and_4 (term_4, D, not_B);
// Term 5: A'C
and and_5 (term_5, C, not_A);
// Term 6: BD'
and and_6 (term_6, not_D, B);
// Term 7: AC'
and and_7 (term_7, not_C, A);
// Term 8: DF'
and and_8 (term_8, not_F, D);
// Term 9: CE'
and and_9 (term_9, not_E, C);
// Term 10: B'EH
and and_10 (term_10, H, E, not_B);
// Term 11: A'FG
and and_11 (term_11, G, F, not_A);

// --- Sum Term (OR gate) ---
or or_final (F, term_0, term_1, term_2, term_3, term_4, term_5, term_6, term_7, term_8, term_9, term_10, term_11);

endmodule
