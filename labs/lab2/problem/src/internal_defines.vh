////
//// Internal signal constants
////


// ALUOp
// arithmetic operation
`define ALUOp_ADD       4'b0000
`define ALUOp_SUB       4'b0001

// bit operation
`define ALUOp_AND       4'b0010
`define ALUOp_OR        4'b0011
`define ALUOp_XOR       4'b0100
`define ALUOp_NOR       4'b0101

// shift operation
`define ALUOp_SHL       4'b0110
`define ALUOp_SHLV      4'b0111
`define ALUOp_SHRL      4'b1000
`define ALUOp_SHRLV     4'b1001
`define ALUOp_SHRA      4'b1010
`define ALUOp_SHRAV     4'b1011


// ALUSrc
// ALUSrc1
`define ALUSrc1_REG     1'b0    // rs
`define ALUSrc1_SHAMT   1'b1    // Shift Amount

// ALUSrc2
`define ALUSrc2_REG     2'b00   // rt
`define ALUSrc2_ZEIMM   2'b01   // Zero-Extended
`define ALUSrc2_SEIMM   2'b10   // Sign-Extended
