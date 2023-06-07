// Include the MIPS constants
`include "mips_defines.vh"
`include "internal_defines.vh"

////
//// IR: Instruction register
////
//// inst  (input)  - Instruction
////
//// op         (output) - Operation code
//// rs         (output) - Register rs
//// rt         (output) - Register rt
//// rd         (output) - Register rd
//// shamt      (output) - Shift amount
//// funct      (output) - Funtion field
//// imm        (output) - Immediate/Offset
//// target     (output) - Target address
////
module IR(
   // Outputs
   output [5:0] op,
   output [4:0] rs,
   output [4:0] rt,
   output [4:0] rd,
   output [4:0] shamt,
   output [5:0] funct,
   output [15:0] imm,
   output [25:0] target,
   // Inputs
   input [31:0] inst
);

   assign op = inst[31:26];
   assign rs = inst[25:21];
   assign rt = inst[20:16];
   assign rd = inst[15:11];
   assign shamt = inst[10:6];
   assign funct = inst[5:0];
   assign imm = inst[15:0];
   assign target = inst[25:0];

endmodule