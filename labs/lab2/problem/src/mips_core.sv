/*
 *
 * Redistributions of any form whatsoever must retain and/or include the
 * following acknowledgment, notices and disclaimer:
 *
 * This product includes software developed by Carnegie Mellon University. 
 *
 * Copyright (c) 2004 by Babak Falsafi and James Hoe,
 * Computer Architecture Lab at Carnegie Mellon (CALCM), 
 * Carnegie Mellon University.
 *
 * This source file was written and maintained by Jared Smolens 
 * as part of the Two-Way In-Order Superscalar project for Carnegie Mellon's 
 * Introduction to Computer Architecture course, 18-447. The source file
 * is in part derived from code originally written by Herman Schmit and 
 * Diana Marculescu.
 *
 * You may not use the name "Carnegie Mellon University" or derivations 
 * thereof to endorse or promote products derived from this software.
 *
 * If you modify the software you must place a notice on or within any 
 * modified version provided or made available to any third party stating 
 * that you have modified the software.  The notice shall include at least 
 * your name, address, phone number, email address and the date and purpose 
 * of the modification.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY WARRANTY OF ANY KIND, EITHER 
 * EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO ANYWARRANTY 
 * THAT THE SOFTWARE WILL CONFORM TO SPECIFICATIONS OR BE ERROR-FREE AND ANY 
 * IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, 
 * TITLE, OR NON-INFRINGEMENT.  IN NO EVENT SHALL CARNEGIE MELLON UNIVERSITY 
 * BE LIABLE FOR ANY DAMAGES, INCLUDING BUT NOT LIMITED TO DIRECT, INDIRECT, 
 * SPECIAL OR CONSEQUENTIAL DAMAGES, ARISING OUT OF, RESULTING FROM, OR IN 
 * ANY WAY CONNECTED WITH THIS SOFTWARE (WHETHER OR NOT BASED UPON WARRANTY, 
 * CONTRACT, TORT OR OTHERWISE).
 *
 */

//////
////// MIPS 447: A single-cycle MIPS ISA simulator
//////

// Include the MIPS constants
`include "mips_defines.vh"
`include "internal_defines.vh"

////
//// The MIPS standalone processor module
////
////   clk          (input)  - The clock
////   inst_addr    (output) - Address of instruction to load
////   inst         (input)  - Instruction from memory
////   inst_excpt   (input)  - inst_addr not valid
////   mem_addr     (output) - Address of data to load
////   mem_data_in  (output) - Data for memory store
////   mem_data_out (input)  - Data from memory load
////   mem_write_en (output) - Memory write mask
////   mem_excpt    (input)  - mem_addr not valid
////   halted       (output) - Processor halted
////   reset        (input)  - Reset the processor
////   

module mips_core(/*AUTOARG*/
   // Outputs
   inst_addr, mem_addr, mem_data_in, mem_write_en, halted,
   // Inputs
   clk, inst_excpt, mem_excpt, inst, mem_data_out, rst_b
   );
   
   parameter text_start  = 32'h00400000; /* Initial value of $pc */

   // Core Interface
   input         clk, inst_excpt, mem_excpt;
   output [29:0] inst_addr;
   output [29:0] mem_addr;
   input  [31:0] inst, mem_data_out;
   output [31:0] mem_data_in;
   output [3:0]  mem_write_en;
   output        halted;
   input         rst_b;

   // Forced interface signals -- required for synthesis to work OK.
   // This is probably not what you want!
   assign        mem_addr = 0;
   assign        mem_data_in = mem_data_out;
   assign        mem_write_en = 4'b0;

   // Internal signals
   wire [5:0] op_code,
   wire [4:0] rs,
   wire [4:0] rt,
   wire [4:0] rd,
   wire [4:0] shamt,
   wire [5:0] funct,
   wire [15:0] imm,
   wire [25:0] target,

   wire RegDst,
   wire RegWrite,
   wire Jump,
   wire Branch,
   wire MemRead,
   wire MemWrite,
   wire MemtoReg,
   wire [3:0] ALUOp,
   wire ALUSrc1,
   wire [1:0] ALUSrc2,

   wire [31:0] rs_data,
   wire [31:0] rt_data,
   wire [31:0] rd_data;

   
   // PC Management
   PC #(text_start) program_counter (.clk(clk), .rst(rst_b), .Address_in(pc), .Address_out(nextpc));
   Add_PC(pc, nextpc);

   assign        inst_addr = nextpc[31:2];

   // Instruction decoding
   IR instruction_register(
      .inst(inst),
      .op_code(op_code),
      .rs(rs),
      .rt(rt),
      .rd(rd),
      .shamt(shamt),
      .funct(funct),
      .imm(imm),
      .target(target))

   // synthesis translate_off
   always @(posedge clk) begin
     // useful for debugging, you will want to comment this out for long programs
     if (rst_b) begin
       $display ( "=== Simulation Cycle %d ===", $time );
       $display ( "[pc=%x, inst=%x] [op=%x, rs=%d, rt=%d, rd=%d, imm=%x, f2=%x] [reset=%d, halted=%d]",
                   pc, inst, dcd_op, dcd_rs, dcd_rt, dcd_rd, dcd_imm, dcd_funct2, ~rst_b, halted);
     end
   end
   // synthesis translate_on

   // Generate control signals
   CU control_unit(
      // Inputs
      .op_code    (op_code),
      .rt         (rt),
      .funct      (funct),
      // Outputs
      .RegDst     (RegDst),
      .RegWrite   (RegWrite)
      .Jump       (Jump),
      .Branch     (Branch),
      .MemRead    (MemRead),
      .MemWrite   (MemWrite),
      .MemtoReg   (MemtoReg),
      .ALUOp      (ALUOp),
      .ALUSrc1    (ALUSrc1),
      .ALUSrc2    (ALUSrc2));
 
   // Register File
   // Instantiate the register file from reg_file.v here.
   // Don't forget to hookup the "halted" signal to trigger the register dump 
   regfile reg_file (
		       // Outputs
		       .rs_data   (rs_data),
		       .rt_data   (rt_data),
		       // Inputs
		       .rs_num    (rs),
		       .rt_num    (rt),
		       .rd_num    (rd),
		       .rd_data   (rd_data),
		       .rd_we     (RegWrite),
		       .clk       (clk),
		       .rst_b     (rst_b),
		       .halted    (halted));

   // synthesis translate_off
   initial begin
     // Delete this block when you are ready to try for real
     $display(""); 
     $display(""); 
     $display(""); 
     $display(""); 
     $display(">>>>> This works much better after you have hooked up the reg file. <<<<<");
     $display(""); 
     $display(""); 
     $display(""); 
     $display(""); 
     $finish;
   end
   // synthesis translate_on

   // Execute
   mips_ALU ALU(.alu__out(alu__out), 
                .alu__op1(rs_data),
                .alu__op2(dcd_se_imm),
                .alu__sel(alu__sel));
 
   // Miscellaneous stuff (Exceptions, syscalls, and halt)
   exception_unit EU(.exception_halt(exception_halt), .pc(pc), .rst_b(rst_b),
                     .clk(clk), .load_ex_regs(load_ex_regs),
                     .load_bva(load_bva), .load_bva_sel(load_bva_sel),
                     .cause(cause_code),
                     .IBE(inst_excpt),
                     .DBE(1'b0),
                     .RI(ctrl_RI),
                     .Ov(1'b0),
                     .BP(1'b0),
                     .AdEL_inst(pc[1:0]?1'b1:1'b0),
                     .AdEL_data(1'b0),
                     .AdES(1'b0),
                     .CpU(1'b0));

   assign r_v0 = 32'h0a; // Good enough for now. To support syscall for real,
                         // you should read the syscall
                         // argument from $v0 of the register file 

   syscall_unit SU(.syscall_halt(syscall_halt), .pc(pc), .clk(clk), .Sys(ctrl_Sys),
                   .r_v0(r_v0), .rst_b(rst_b));
   assign        internal_halt = exception_halt | syscall_halt;
   register #(1, 0) Halt(halted, internal_halt, clk, 1'b1, rst_b);
   register #(32, 0) EPCReg(epc, pc, clk, load_ex_regs, rst_b);
   register #(32, 0) CauseReg(cause,
                              {25'b0, cause_code, 2'b0}, 
                              clk, load_ex_regs, rst_b);
   register #(32, 0) BadVAddrReg(bad_v_addr, pc, clk, load_bva, rst_b);

endmodule // mips_core


////
//// mips_ALU: Performs all arithmetic and logical operations
////
//// out (output) - Final result
//// in1 (input)  - Operand modified by the operation
//// in2 (input)  - Operand used (in arithmetic ops) to modify in1
//// sel (input)  - Selects which operation is to be performed
////
module mips_ALU(alu__out, alu__op1, alu__op2, alu__sel);

   output [31:0] alu__out;
   input [31:0]  alu__op1, alu__op2;
   input [3:0]   alu__sel;

   adder AdderUnit(alu__out, alu__op1, alu__op2, alu__sel[0]);

endmodule


//// register: A register which may be reset to an arbirary value
////
//// q      (output) - Current value of register
//// d      (input)  - Next value of register
//// clk    (input)  - Clock (positive edge-sensitive)
//// enable (input)  - Load new value?
//// reset  (input)  - System reset
////
module register(q, d, clk, enable, rst_b);

   parameter
            width = 32,
            reset_value = 0;

   output [(width-1):0] q;
   reg [(width-1):0]    q;
   input [(width-1):0]  d;
   input                 clk, enable, rst_b;

   always @(posedge clk or negedge rst_b)
     if (~rst_b)
       q <= reset_value;
     else if (enable)
       q <= d;

endmodule // register


////
//// adder
////
//// out (output) - adder result
//// in1 (input)  - Operand1
//// in2 (input)  - Operand2
//// sub (input)  - Subtract?
////
module adder(out, in1, in2, sub);
   output [31:0] out;
   input [31:0]  in1, in2;
   input         sub;

   assign        out = sub?(in1 - in2):(in1 + in2);

endmodule // adder


////
//// add_const: An adder that adds a fixed constant value
////
//// out (output) - adder result
//// in  (input)  - Operand
////
module add_const(out, in);

   parameter add_value = 1;

   output   [31:0] out;
   input    [31:0] in;

   assign   out = in + add_value;

endmodule // adder

// Local Variables:
// verilog-library-directories:("." "../447rtl")
// End:
