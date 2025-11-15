// Function: F = A'B'C'D'E'F'G'H'I'J'K'L' + A'B'C'D'E'FG'H'I'J'K'L + A'B'C'D'EF'G'H'I'J'KL' + A'B'C'D'EFG'H'I'J'KL + A'B'C'DE'F'G'H'I'JK'L' + A'B'C'DE'FG'H'I'JK'L + A'B'C'DEF'G'H'I'JKL' + A'B'C'DEFG'H'I'JKL + A'B'CD'E'F'G'H'IJ'K'L' + A'B'CD'E'FG'H'IJ'K'L + A'B'CD'EF'G'H'IJ'KL' + A'B'CD'EFG'H'IJ'KL + A'B'CDE'F'G'H'IJK'L' + A'B'CDE'FG'H'IJK'L + A'B'CDEF'G'H'IJKL' + A'B'CDEFG'H'IJKL + A'BC'D'E'F'G'HI'J'K'L' + A'BC'D'E'FG'HI'J'K'L + A'BC'D'EF'G'HI'J'KL' + A'BC'D'EFG'HI'J'KL + A'BC'DE'F'G'HI'JK'L' + A'BC'DE'FG'HI'JK'L + A'BC'DEF'G'HI'JKL' + A'BC'DEFG'HI'JKL + A'BCD'E'F'G'HIJ'K'L' + A'BCD'E'FG'HIJ'K'L + A'BCD'EF'G'HIJ'KL' + A'BCD'EFG'HIJ'KL + A'BCDE'F'G'HIJK'L' + A'BCDE'FG'HIJK'L + A'BCDEF'G'HIJKL' + A'BCDEFG'HIJKL + AB'C'D'E'F'GH'I'J'K'L' + AB'C'D'E'FGH'I'J'K'L + AB'C'D'EF'GH'I'J'KL' + AB'C'D'EFGH'I'J'KL + AB'C'DE'F'GH'I'JK'L' + AB'C'DE'FGH'I'JK'L + AB'C'DEF'GH'I'JKL' + AB'C'DEFGH'I'JKL + AB'CD'E'F'GH'IJ'K'L' + AB'CD'E'FGH'IJ'K'L + AB'CD'EF'GH'IJ'KL' + AB'CD'EFGH'IJ'KL + AB'CDE'F'GH'IJK'L' + AB'CDE'FGH'IJK'L + AB'CDEF'GH'IJKL' + AB'CDEFGH'IJKL + ABC'D'E'F'GHI'J'K'L' + ABC'D'E'FGHI'J'K'L + ABC'D'EF'GHI'J'KL' + ABC'D'EFGHI'J'KL + ABC'DE'F'GHI'JK'L' + ABC'DE'FGHI'JK'L + ABC'DEF'GHI'JKL' + ABC'DEFGHI'JKL + ABCD'E'F'GHIJ'K'L' + ABCD'E'FGHIJ'K'L + ABCD'EF'GHIJ'KL' + ABCD'EFGHIJ'KL + ABCDE'F'GHIJK'L' + ABCDE'FGHIJK'L + ABCDEF'GHIJKL' + ABCDEFGHIJKL
// Generated using Verilog primitives.

module logic_function (
input  A,
input  B,
input  C,
input  D,
input  E,
input  F,
input  G,
input  H,
input  I,
input  J,
input  K,
input  L
,
output Final
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
wire not_I;
wire not_J;
wire not_K;
wire not_L;
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
wire term_12;
wire term_13;
wire term_14;
wire term_15;
wire term_16;
wire term_17;
wire term_18;
wire term_19;
wire term_20;
wire term_21;
wire term_22;
wire term_23;
wire term_24;
wire term_25;
wire term_26;
wire term_27;
wire term_28;
wire term_29;
wire term_30;
wire term_31;
wire term_32;
wire term_33;
wire term_34;
wire term_35;
wire term_36;
wire term_37;
wire term_38;
wire term_39;
wire term_40;
wire term_41;
wire term_42;
wire term_43;
wire term_44;
wire term_45;
wire term_46;
wire term_47;
wire term_48;
wire term_49;
wire term_50;
wire term_51;
wire term_52;
wire term_53;
wire term_54;
wire term_55;
wire term_56;
wire term_57;
wire term_58;
wire term_59;
wire term_60;
wire term_61;
wire term_62;
wire term_63;

// --- Input Inverters ---
not inv_A (not_A, A);
not inv_B (not_B, B);
not inv_C (not_C, C);
not inv_D (not_D, D);
not inv_E (not_E, E);
not inv_F (not_F, F);
not inv_G (not_G, G);
not inv_H (not_H, H);
not inv_I (not_I, I);
not inv_J (not_J, J);
not inv_K (not_K, K);
not inv_L (not_L, L);

// --- Product Terms (AND gates) ---
// Term 0: A'B'C'D'E'F'G'H'I'J'K'L'
and and_0 (term_0, not_L, not_K, not_J, not_I, not_H, not_G, not_F, not_E, not_D, not_C, not_B, not_A);
// Term 1: A'B'C'D'E'FG'H'I'J'K'L
and and_1 (term_1, L, not_K, not_J, not_I, not_H, not_G, F, not_E, not_D, not_C, not_B, not_A);
// Term 2: A'B'C'D'EF'G'H'I'J'KL'
and and_2 (term_2, not_L, K, not_J, not_I, not_H, not_G, not_F, E, not_D, not_C, not_B, not_A);
// Term 3: A'B'C'D'EFG'H'I'J'KL
and and_3 (term_3, L, K, not_J, not_I, not_H, not_G, F, E, not_D, not_C, not_B, not_A);
// Term 4: A'B'C'DE'F'G'H'I'JK'L'
and and_4 (term_4, not_L, not_K, J, not_I, not_H, not_G, not_F, not_E, D, not_C, not_B, not_A);
// Term 5: A'B'C'DE'FG'H'I'JK'L
and and_5 (term_5, L, not_K, J, not_I, not_H, not_G, F, not_E, D, not_C, not_B, not_A);
// Term 6: A'B'C'DEF'G'H'I'JKL'
and and_6 (term_6, not_L, K, J, not_I, not_H, not_G, not_F, E, D, not_C, not_B, not_A);
// Term 7: A'B'C'DEFG'H'I'JKL
and and_7 (term_7, L, K, J, not_I, not_H, not_G, F, E, D, not_C, not_B, not_A);
// Term 8: A'B'CD'E'F'G'H'IJ'K'L'
and and_8 (term_8, not_L, not_K, not_J, I, not_H, not_G, not_F, not_E, not_D, C, not_B, not_A);
// Term 9: A'B'CD'E'FG'H'IJ'K'L
and and_9 (term_9, L, not_K, not_J, I, not_H, not_G, F, not_E, not_D, C, not_B, not_A);
// Term 10: A'B'CD'EF'G'H'IJ'KL'
and and_10 (term_10, not_L, K, not_J, I, not_H, not_G, not_F, E, not_D, C, not_B, not_A);
// Term 11: A'B'CD'EFG'H'IJ'KL
and and_11 (term_11, L, K, not_J, I, not_H, not_G, F, E, not_D, C, not_B, not_A);
// Term 12: A'B'CDE'F'G'H'IJK'L'
and and_12 (term_12, not_L, not_K, J, I, not_H, not_G, not_F, not_E, D, C, not_B, not_A);
// Term 13: A'B'CDE'FG'H'IJK'L
and and_13 (term_13, L, not_K, J, I, not_H, not_G, F, not_E, D, C, not_B, not_A);
// Term 14: A'B'CDEF'G'H'IJKL'
and and_14 (term_14, not_L, K, J, I, not_H, not_G, not_F, E, D, C, not_B, not_A);
// Term 15: A'B'CDEFG'H'IJKL
and and_15 (term_15, L, K, J, I, not_H, not_G, F, E, D, C, not_B, not_A);
// Term 16: A'BC'D'E'F'G'HI'J'K'L'
and and_16 (term_16, not_L, not_K, not_J, not_I, H, not_G, not_F, not_E, not_D, not_C, B, not_A);
// Term 17: A'BC'D'E'FG'HI'J'K'L
and and_17 (term_17, L, not_K, not_J, not_I, H, not_G, F, not_E, not_D, not_C, B, not_A);
// Term 18: A'BC'D'EF'G'HI'J'KL'
and and_18 (term_18, not_L, K, not_J, not_I, H, not_G, not_F, E, not_D, not_C, B, not_A);
// Term 19: A'BC'D'EFG'HI'J'KL
and and_19 (term_19, L, K, not_J, not_I, H, not_G, F, E, not_D, not_C, B, not_A);
// Term 20: A'BC'DE'F'G'HI'JK'L'
and and_20 (term_20, not_L, not_K, J, not_I, H, not_G, not_F, not_E, D, not_C, B, not_A);
// Term 21: A'BC'DE'FG'HI'JK'L
and and_21 (term_21, L, not_K, J, not_I, H, not_G, F, not_E, D, not_C, B, not_A);
// Term 22: A'BC'DEF'G'HI'JKL'
and and_22 (term_22, not_L, K, J, not_I, H, not_G, not_F, E, D, not_C, B, not_A);
// Term 23: A'BC'DEFG'HI'JKL
and and_23 (term_23, L, K, J, not_I, H, not_G, F, E, D, not_C, B, not_A);
// Term 24: A'BCD'E'F'G'HIJ'K'L'
and and_24 (term_24, not_L, not_K, not_J, I, H, not_G, not_F, not_E, not_D, C, B, not_A);
// Term 25: A'BCD'E'FG'HIJ'K'L
and and_25 (term_25, L, not_K, not_J, I, H, not_G, F, not_E, not_D, C, B, not_A);
// Term 26: A'BCD'EF'G'HIJ'KL'
and and_26 (term_26, not_L, K, not_J, I, H, not_G, not_F, E, not_D, C, B, not_A);
// Term 27: A'BCD'EFG'HIJ'KL
and and_27 (term_27, L, K, not_J, I, H, not_G, F, E, not_D, C, B, not_A);
// Term 28: A'BCDE'F'G'HIJK'L'
and and_28 (term_28, not_L, not_K, J, I, H, not_G, not_F, not_E, D, C, B, not_A);
// Term 29: A'BCDE'FG'HIJK'L
and and_29 (term_29, L, not_K, J, I, H, not_G, F, not_E, D, C, B, not_A);
// Term 30: A'BCDEF'G'HIJKL'
and and_30 (term_30, not_L, K, J, I, H, not_G, not_F, E, D, C, B, not_A);
// Term 31: A'BCDEFG'HIJKL
and and_31 (term_31, L, K, J, I, H, not_G, F, E, D, C, B, not_A);
// Term 32: AB'C'D'E'F'GH'I'J'K'L'
and and_32 (term_32, not_L, not_K, not_J, not_I, not_H, G, not_F, not_E, not_D, not_C, not_B, A);
// Term 33: AB'C'D'E'FGH'I'J'K'L
and and_33 (term_33, L, not_K, not_J, not_I, not_H, G, F, not_E, not_D, not_C, not_B, A);
// Term 34: AB'C'D'EF'GH'I'J'KL'
and and_34 (term_34, not_L, K, not_J, not_I, not_H, G, not_F, E, not_D, not_C, not_B, A);
// Term 35: AB'C'D'EFGH'I'J'KL
and and_35 (term_35, L, K, not_J, not_I, not_H, G, F, E, not_D, not_C, not_B, A);
// Term 36: AB'C'DE'F'GH'I'JK'L'
and and_36 (term_36, not_L, not_K, J, not_I, not_H, G, not_F, not_E, D, not_C, not_B, A);
// Term 37: AB'C'DE'FGH'I'JK'L
and and_37 (term_37, L, not_K, J, not_I, not_H, G, F, not_E, D, not_C, not_B, A);
// Term 38: AB'C'DEF'GH'I'JKL'
and and_38 (term_38, not_L, K, J, not_I, not_H, G, not_F, E, D, not_C, not_B, A);
// Term 39: AB'C'DEFGH'I'JKL
and and_39 (term_39, L, K, J, not_I, not_H, G, F, E, D, not_C, not_B, A);
// Term 40: AB'CD'E'F'GH'IJ'K'L'
and and_40 (term_40, not_L, not_K, not_J, I, not_H, G, not_F, not_E, not_D, C, not_B, A);
// Term 41: AB'CD'E'FGH'IJ'K'L
and and_41 (term_41, L, not_K, not_J, I, not_H, G, F, not_E, not_D, C, not_B, A);
// Term 42: AB'CD'EF'GH'IJ'KL'
and and_42 (term_42, not_L, K, not_J, I, not_H, G, not_F, E, not_D, C, not_B, A);
// Term 43: AB'CD'EFGH'IJ'KL
and and_43 (term_43, L, K, not_J, I, not_H, G, F, E, not_D, C, not_B, A);
// Term 44: AB'CDE'F'GH'IJK'L'
and and_44 (term_44, not_L, not_K, J, I, not_H, G, not_F, not_E, D, C, not_B, A);
// Term 45: AB'CDE'FGH'IJK'L
and and_45 (term_45, L, not_K, J, I, not_H, G, F, not_E, D, C, not_B, A);
// Term 46: AB'CDEF'GH'IJKL'
and and_46 (term_46, not_L, K, J, I, not_H, G, not_F, E, D, C, not_B, A);
// Term 47: AB'CDEFGH'IJKL
and and_47 (term_47, L, K, J, I, not_H, G, F, E, D, C, not_B, A);
// Term 48: ABC'D'E'F'GHI'J'K'L'
and and_48 (term_48, not_L, not_K, not_J, not_I, H, G, not_F, not_E, not_D, not_C, B, A);
// Term 49: ABC'D'E'FGHI'J'K'L
and and_49 (term_49, L, not_K, not_J, not_I, H, G, F, not_E, not_D, not_C, B, A);
// Term 50: ABC'D'EF'GHI'J'KL'
and and_50 (term_50, not_L, K, not_J, not_I, H, G, not_F, E, not_D, not_C, B, A);
// Term 51: ABC'D'EFGHI'J'KL
and and_51 (term_51, L, K, not_J, not_I, H, G, F, E, not_D, not_C, B, A);
// Term 52: ABC'DE'F'GHI'JK'L'
and and_52 (term_52, not_L, not_K, J, not_I, H, G, not_F, not_E, D, not_C, B, A);
// Term 53: ABC'DE'FGHI'JK'L
and and_53 (term_53, L, not_K, J, not_I, H, G, F, not_E, D, not_C, B, A);
// Term 54: ABC'DEF'GHI'JKL'
and and_54 (term_54, not_L, K, J, not_I, H, G, not_F, E, D, not_C, B, A);
// Term 55: ABC'DEFGHI'JKL
and and_55 (term_55, L, K, J, not_I, H, G, F, E, D, not_C, B, A);
// Term 56: ABCD'E'F'GHIJ'K'L'
and and_56 (term_56, not_L, not_K, not_J, I, H, G, not_F, not_E, not_D, C, B, A);
// Term 57: ABCD'E'FGHIJ'K'L
and and_57 (term_57, L, not_K, not_J, I, H, G, F, not_E, not_D, C, B, A);
// Term 58: ABCD'EF'GHIJ'KL'
and and_58 (term_58, not_L, K, not_J, I, H, G, not_F, E, not_D, C, B, A);
// Term 59: ABCD'EFGHIJ'KL
and and_59 (term_59, L, K, not_J, I, H, G, F, E, not_D, C, B, A);
// Term 60: ABCDE'F'GHIJK'L'
and and_60 (term_60, not_L, not_K, J, I, H, G, not_F, not_E, D, C, B, A);
// Term 61: ABCDE'FGHIJK'L
and and_61 (term_61, L, not_K, J, I, H, G, F, not_E, D, C, B, A);
// Term 62: ABCDEF'GHIJKL'
and and_62 (term_62, not_L, K, J, I, H, G, not_F, E, D, C, B, A);
// Term 63: ABCDEFGHIJKL
and and_63 (term_63, L, K, J, I, H, G, F, E, D, C, B, A);

// --- Sum Term (OR gate) ---
or or_final (Final, term_0, term_1, term_2, term_3, term_4, term_5, term_6, term_7, term_8, term_9, term_10, term_11, term_12, term_13, term_14, term_15, term_16, term_17, term_18, term_19, term_20, term_21, term_22, term_23, term_24, term_25, term_26, term_27, term_28, term_29, term_30, term_31, term_32, term_33, term_34, term_35, term_36, term_37, term_38, term_39, term_40, term_41, term_42, term_43, term_44, term_45, term_46, term_47, term_48, term_49, term_50, term_51, term_52, term_53, term_54, term_55, term_56, term_57, term_58, term_59, term_60, term_61, term_62, term_63);

endmodule
