# Lab 1: Instruction-Level MIPS Simulator

This is a lab report of a CMU course 18-447 (Spring 2015).



## 1 What I Should Do

Implement simulations of several MIPS instructions using the *C* language.



## 2 Key Notes in the Lab Guide Doc

1. The architecture is little-endian. 
2. Memory addresses are 32-bit-aligned. So it is not weird that the bottom two bits of a target address of jump/branch instructions are always zero, e.g. when implementing the Jump instruction we typically shift the 26-bit target address to the left by two bits.
3. Branch instructions can update `NEXT STATE.PC` directly to the branch target when the branch is taken.
4. “jump-and-link” instructions (JAL, JALR, BLTZAL, BGEZAL) store PC + 4 in *R31*, rather than *PC* + 8 as specified in the manual in these instructions’ descriptions.
5. Overflow exceptions that can be raised by certain arithmetic instructions (e.g., ADDI) is not required to be handled.



## 3 Points to Be Noticed in the Lab

### Data Type Conversion

Explicit and implicit data type conversions are widely used in coding. We must know when the implicit conversion will occur, when the explicit (mandatory) conversion should be applied, what the type conversion is actually doing, etc., otherwise there will be difficult to troubleshoot and unexpected errors.

In the C language, if an unsigned number is operated on with a signed number, a type conversion is performed. The specific conversion method is as follows:

- If the type of the signed number is longer than the type of the unsigned number (e.g. int and unsigned short are operated), the unsigned number is converted to the type of the signed number.
- If the type of the unsigned number is longer than the type of the signed number (e.g. long and unsigned int are operated), the signed number is converted to the type of the unsigned number.
- If the two operand types are the same length, the signed number is converted to the unsigned type before the operation is performed.

The conversion rules are roughly as follows:

- promotion
  - unsigned -> unsigned: pad with 0s in front.
  - signed -> signed: pad with the highest digit in front.
  - unsigned -> signed: pad with the highest digit in front.
  - signed -> unsigned: pad with 0s in front.
- demotion
  - unsigned -> unsigned: truncate high bits.
  - signed -> signed: truncate high bits.
  - unsigned -> signed: truncate high bits.
  - signed -> unsigned: truncate high bits.

Moreover, we should review the C operator precedence: (the following form comes from https://en.cppreference.com/w/c/language/operator_precedence)

| Precedence | Operator           | Description                                       | Associativity |
| :--------: | :----------------- | :------------------------------------------------ | :------------ |
|     1      | `++` `--`          | Suffix/postfix increment and decrement            | Left-to-right |
|            | `()`               | Function call                                     |               |
|            | `[]`               | Array subscripting                                |               |
|            | `.`                | Structure and union member access                 |               |
|            | `->`               | Structure and union member access through pointer |               |
|            | `(*type*){*list*}` | Compound literal(C99)                             |               |
|     2      | `++` `--`          | Prefix increment and decrement                    | Right-to-left |
|            | `+` `-`            | Unary plus and minus                              |               |
|            | `!` `~`            | Logical NOT and bitwise NOT                       |               |
|            | `(*type*)`         | Cast                                              |               |
|            | `*`                | Indirection (dereference)                         |               |
|            | `&`                | Address-of                                        |               |
|            | `sizeof`           | Size-of                                           |               |
|            | `_Alignof`         | Alignment requirement(C11)                        |               |
|     3      | `*` `/` `%`        | Multiplication, division, and remainder           | Left-to-right |
|     4      | `+` `-`            | Addition and subtraction                          |               |
|     5      | `<<` `>>`          | Bitwise left shift and right shift                |               |
|     6      | `<` `<=`           | For relational operators < and ≤ respectively     |               |
|            | `>` `>=`           | For relational operators > and ≥ respectively     |               |
|     7      | `==` `!=`          | For relational = and ≠ respectively               |               |
|     8      | `&`                | Bitwise AND                                       |               |
|     9      | `^`                | Bitwise XOR (exclusive or)                        |               |
|     10     | `|`                | Bitwise OR (inclusive or)                         |               |
|     11     | `&&`               | Logical AND                                       |               |
|     12     | `||`               | Logical OR                                        |               |
|     13     | `?:`               | Ternary conditional                               | Right-to-left |
|     14     | `=`                | Simple assignment                                 |               |
|            | `+=` `-=`          | Assignment by sum and difference                  |               |
|            | `*=` `/=` `%=`     | Assignment by product, quotient, and remainder    |               |
|            | `<<=` `>>=`        | Assignment by bitwise left shift and right shift  |               |
|            | `&=` `^=` `|=`     | Assignment by bitwise AND, XOR, and OR            |               |
|     15     | `,`                | Comma                                             | Left-to-right |

### What is the difference between ADDI and ADDIU?

In the MIPS R4000 User Manual (32-bit mode only), ADDI represents for "Add Immediate" and ADDIU represents for "Add Immediate Unsigned", but the immediate numbers are both sign-extended and added to the contents of general register *rs* to form the result. The only difference is that the manual asserts ADDI could raise integer overflow exception while ADDIU could not.

So, does "U" really stand for unsigned, even if the immediate is sign-extended in ADDIU? 

According to the book *See MIPS Run Linux*, the writer says, a "U" suffix is usually read as "unsigned" on assembly mnemonics, but this is not always the meaning. In most arithmetic operations, "U" stands for "no overflow check". When using the arithmetic operation ADD without a "U" suffix, the result overflow will cause an exception. However, when the arithmetic operation ADD with a "U" suffix is used, the same result will be obtained for the same operands but no exception will be raised. By the way, *C* and *C++* do not support integer overflow exceptions, so always use the "U" form. 

### 
