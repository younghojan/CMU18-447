////
//// PC: A program counter
////
//// Address_out (output) - current program counter value
//// clk (input)  - clock signal
//// rst (input)  - reset signal
//// Address_in (input)  - input program counter value
////

module PC (
   output reg [31:0] Address_out,
   input  clk,
   input  rst,
   input  [31:0] Address_in
);

   parameter text_start  = 32'h00400000; /* Initial value of $pc */

   // PC register
   always @(posedge clk, negedge rst)
   begin
      if (!rst)
         Address_out <= text_start;  // Set initial value of the program counter
      else
         Address_out <= Address_in;   // Update the program counter value
   end

endmodule // PC


//// Add_PC: Add a constant value to the input address
////
//// Address_in  (input)  - input address
//// Address_out (output) - resulting address after addition
////
module Add_PC (
   input  [31:0] Address_in,
   output [31:0] Address_out
);

   assign Address_out = Address_in + 32'd4;  // Add a constant value of 4 to the input address

endmodule // Add_PC