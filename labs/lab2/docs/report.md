# Lab 2: Single Cycle MIPS

This is a lab report of a CMU course 18-447 (Spring 2015).

## 0 Configure VSCode and Verilog Tutorial

Before using VSCode for verilog programming, you need to configure VSCode first. 

1. Install verilog plugin from VSCode extension market. 
   https://marketplace.visualstudio.com/items?itemName=mshr-h.VerilogHDL

2. Install iverilog and gtkwave. Icarus Verilog is intended to compile ALL of the Verilog HDL as described in the IEEE-1364 standard.

   ```sh
   sudo apt install iverilog gtkwave
   ```

3. Install ctags. [Universal Ctags](https://ctags.io/) (abbreviated as u-ctags) is a *maintained* implementation of `ctags`. `ctags` generates an index (or tag) file of language objects found in source files for programming languages. This index makes it easy for text editors and other tools to locate the indexed items.

   ```sh
   sudo apt install universal-ctags
   ```

   ([Exuberant Ctags](http://ctags.sourceforge.net/) maintained by Darren Hiebert, the ancestor of Universal Ctags, stalled, so install Universal Ctags instead.)

4. Select iverilog for the verilog linter and configure iverilog args in VSCode settings.

   ```json
   {
       "verilog.linting.linter": "iverilog",
   	"verilog.linting.iverilog.arguments": "-I"
   }
   ```

https://www.chipverify.com/ represents a good tutorial for beginners. 



## 1 What I Should Do?

Implement a single-cycle MIPS machine in System Verilog.