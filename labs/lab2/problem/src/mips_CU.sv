// Include the MIPS constants
`include "mips_defines.vh"
`include "internal_defines.vh"

////
//// CU: Control Unit
////
//// op_code  (input)  - Instruction opcode
//// rt       (input)  - BGEZ or BGEZAL or BLTZ or BLTZAL 
//// funct    (input)  - Function field
////
//// RegDst   (output) - Register destination control signal
//// RegWrite (output) - Register write control signal
//// Jump     (output) - Jump control signal
//// Branch   (output) - Branch control signal
//// MemRead  (output) - Memory read control signal
//// MemWrite (output) - Memory write control signal
//// MemtoReg (output) - Memory to register data control signal
//// ALUop    (output) - ALU operation control signal
//// ALUSrc1  (output) - ALU operand 1 control signal
//// ALUSrc2  (output) - ALU operand 2 control signal
////
module CU(
   // Inputs
   op_code,
   rt,
   funct,
   // Outputs
   RegDst,
   RegWrite
   Jump,
   Branch,
   MemRead,
   MemWrite,
   MemtoReg,
   ALUOp,
   ALUSrc1,
   ALUSrc2
);

   input [5:0] op_code;
   input [5:0] funct;
   output reg  RegDst, RegWrite, Jump, Branch, MemRead, MemWrite, MemtoReg, ALUSrc1;
   output reg  [1:0] ALUSrc2;
   output reg  [3:0] ALUOp;

   always @(*) begin
      case(op_code)

         `OP_OTHER0:  // SPECIAL
            begin
               case(funct)
                  
                  `OP0_ADD:
                     begin
                        RegDst = 1;
                        RegWrite = 1;
                        Jump = 0;
                        Branch = 0;
                        MemRead = 0;
                        MemWrite = 0;
                        MemtoReg = 0;
                        ALUOp = `ALUOp_ADD;
                        ALUSrc1 = `ALUSrc1_REG;
                        ALUSrc2 = `ALUSrc2_REG;
                     end

                  `OP0_ADDU:
                     begin
                        RegDst = 1;
                        RegWrite = 1;
                        Jump = 0;
                        Branch = 0;
                        MemRead = 0;
                        MemWrite = 0;
                        MemtoReg = 0;
                        ALUOp = `ALUOp_ADD;
                        ALUSrc1 = `ALUSrc1_REG;
                        ALUSrc2 = `ALUSrc2_REG;
                     end

                  `OP0_AND:
                     begin
                        RegDst = 1;
                        RegWrite = 1;
                        Jump = 0;
                        Branch = 0;
                        MemRead = 0;
                        MemWrite = 0;
                        MemtoReg = 0;
                        ALUOp = `ALUOp_AND;
                        ALUSrc1 = `ALUSrc1_REG;
                        ALUSrc2 = `ALUSrc2_REG;
                     end

                  `OP0_JALR:
                     begin

                     end

                  `OP0_JR:
                     begin

                     end

                  `OP0_NOR:
                     begin
                        RegDst = 1;
                        RegWrite = 1;
                        Jump = 0;
                        Branch = 0;
                        MemRead = 0;
                        MemWrite = 0;
                        MemtoReg = 0;
                        ALUOp = `ALUOp_NOR;
                        ALUSrc1 = `ALUSrc1_REG;
                        ALUSrc2 = `ALUSrc2_REG;
                     end

                  `OP0_OR:
                     begin
                        RegDst = 1;
                        RegWrite = 1;
                        Jump = 0;
                        Branch = 0;
                        MemRead = 0;
                        MemWrite = 0;
                        MemtoReg = 0;
                        ALUOp = `ALUOp_OR;
                        ALUSrc1 = `ALUSrc1_REG;
                        ALUSrc2 = `ALUSrc2_REG;
                     end

                  `OP0_SLL:
                     begin
                        RegDst = 1;
                        RegWrite = 1;
                        Jump = 0;
                        Branch = 0;
                        MemRead = 0;
                        MemWrite = 0;
                        MemtoReg = 0;
                        ALUOp = `ALUOp_SHL;
                        ALUSrc1 = `ALUSrc1_SHAMT;
                        ALUSrc2 = `ALUSrc2_REG;
                     end

                  `OP0_SLLV:
                     begin
                        RegDst = 1;
                        RegWrite = 1;
                        Jump = 0;
                        Branch = 0;
                        MemRead = 0;
                        MemWrite = 0;
                        MemtoReg = 0;
                        ALUOp = `ALUOp_SHLV;
                        ALUSrc1 = `ALUSrc1_REG;
                        ALUSrc2 = `ALUSrc2_REG;
                     end

                  `OP0_SLT:
                     begin
                        
                     end

                  `OP0_SRA:
                     begin
                        RegDst = 1;
                        RegWrite = 1;
                        Jump = 0;
                        Branch = 0;
                        MemRead = 0;
                        MemWrite = 0;
                        MemtoReg = 0;
                        ALUOp = `ALUOp_SHRA;
                        ALUSrc1 = `ALUSrc1_SHAMT;
                        ALUSrc2 = `ALUSrc2_REG;
                     end

                  `OP0_SRAV:
                     begin
                        RegDst = 1;
                        RegWrite = 1;
                        Jump = 0;
                        Branch = 0;
                        MemRead = 0;
                        MemWrite = 0;
                        MemtoReg = 0;
                        ALUOp = `ALUOp_SHRAV;
                        ALUSrc1 = `ALUSrc1_REG;
                        ALUSrc2 = `ALUSrc2_REG;
                     end

                  `OP0_SRL:
                     begin
                        RegDst = 1;
                        RegWrite = 1;
                        Jump = 0;
                        Branch = 0;
                        MemRead = 0;
                        MemWrite = 0;
                        MemtoReg = 0;
                        ALUOp = `ALUOp_SHRL;
                        ALUSrc1 = `ALUSrc1_SHAMT;
                        ALUSrc2 = `ALUSrc2_REG;
                     end

                  `OP0_SRLV:
                     begin
                        RegDst = 1;
                        RegWrite = 1;
                        Jump = 0;
                        Branch = 0;
                        MemRead = 0;
                        MemWrite = 0;
                        MemtoReg = 0;
                        ALUOp = `ALUOp_SHRLV;
                        ALUSrc1 = `ALUSrc1_REG;
                        ALUSrc2 = `ALUSrc2_REG;
                     end

                  `OP0_SUB:
                     begin
                        RegDst = 1;
                        RegWrite = 1;
                        Jump = 0;
                        Branch = 0;
                        MemRead = 0;
                        MemWrite = 0;
                        MemtoReg = 0;
                        ALUOp = `ALUOp_SUB;
                        ALUSrc1 = `ALUSrc1_REG;
                        ALUSrc2 = `ALUSrc2_REG;
                     end

                  `OP0_SUBU:
                     begin
                        RegDst = 1;
                        RegWrite = 1;
                        Jump = 0;
                        Branch = 0;
                        MemRead = 0;
                        MemWrite = 0;
                        MemtoReg = 0;
                        ALUOp = `ALUOp_SUB;
                        ALUSrc1 = `ALUSrc1_REG;
                        ALUSrc2 = `ALUSrc2_REG;
                     end

                  `OP0_SYSCALL:
                     begin
                        
                     end

                  `OP0_XOR:
                     begin
                        RegDst = 1;
                        RegWrite = 1;
                        Jump = 0;
                        Branch = 0;
                        MemRead = 0;
                        MemWrite = 0;
                        MemtoReg = 0;
                        ALUOp = `ALUOp_XOR;
                        ALUSrc1 = `ALUSrc1_REG;
                        ALUSrc2 = `ALUSrc2_REG;
                     end

                  default:
                     begin
                        
                     end
              endcase

           end

         `OP_ADDI:
            begin
               RegDst = 0;
               RegWrite = 1;
               Jump = 0;
               Branch = 0;
               MemRead = 0;
               MemWrite = 0;
               MemtoReg = 0;
               ALUOp = `ALUOp_ADD;
               ALUSrc1 = `ALUSrc1_REG
               ALUSrc2 = `ALUSrc2_SEIMM;
            end

         `OP_ADDIU:
            begin
               RegDst = 0;
               RegWrite = 1;
               Jump = 0;
               Branch = 0;
               MemRead = 0;
               MemWrite = 0;
               MemtoReg = 0;
               ALUOp = `ALUOp_ADD;
               ALUSrc1 = `ALUSrc1_REG
               ALUSrc2 = `ALUSrc2_SEIMM;
            end

         `OP_ANDI:
            begin
               RegDst = 0;
               RegWrite = 1;
               Jump = 0;
               Branch = 0;
               MemRead = 0;
               MemWrite = 0;
               MemtoReg = 0;
               ALUOp = `ALUOp_AND
               ALUSrc1 = `ALUSrc1_REG;
               ALUSrc2 = `ALUSrc2_ZEIMM;
            end

         `OP_BEQ:
            begin

            end

         `OP_OTHER1: // REGIMM
            begin
               case(rt)

                  `OP1_BGEZ:
                     begin
                        
                     end

                  `OP1_BGEZAL
                     begin
                        
                     end

                  `OP1_BLTZ:
                     begin
                        
                     end

                  `OP1_BLTZAL:
                     begin
                        
                     end
               endcase
            end

         `OP_BGTZ:
            begin
               
            end

         `OP_BLEZ:
            begin
               
            end

         `OP_BNE:
            begin
               
            end

         `OP_J:
            begin

            end

         `OP_JAL:
            begin
               
            end

         `OP_LBU:
            begin
               
            end

         `OP_LH:
            begin
               
            end

         `OP_LHU:
            begin
               
            end

         `OP_LUI:
            begin
               
            end

         `OP_LW:
            begin

            end

         `OP_ORI:
            begin
               RegDst = 0;
               RegWrite = 1;
               Jump = 0;
               Branch = 0;
               MemRead = 0;
               MemWrite = 0;
               MemtoReg = 0;
               ALUOp = `ALUOp_OR;
               ALUSrc = `ALUSrc_ZEIMM;
            end

         `OP_SB:
            begin
               
            end

         `OP_SH:
            begin
               
            end

         `OP_SLTI:
            begin
               
            end

         `OP_SLTIU:
            begin
               
            end

         `OP_SW:
            begin
               
            end

         `OP_XORI:
            begin
               RegDst = 0;
               RegWrite = 1;
               Jump = 0;
               Branch = 0;
               MemRead = 0;
               MemWrite = 0;
               MemtoReg = 0;
               ALUOp = `ALUOp_XOR;
               ALUSrc = `ALUSrc_ZEIMM;
            end
     endcase
   end

endmodule
