/*
Class Project: The logical conclusion
CSCI-2500 Spring 2023
Prof. Slota
*/

/******************************************************************************/
/* Usual suspects to include  */
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define BIT type as a char (i.e., one byte)
typedef char BIT;
#define TRUE 1
#define FALSE 0
#define UNDEF -1

// useful constants
BIT ONE[32] = {TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
               FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
               FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
               FALSE};
BIT ZERO[32] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
                FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
                FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
                FALSE};
BIT REG_THIRTY_ONE[5] = {TRUE, TRUE, TRUE, TRUE, TRUE};
BIT THIRTY_TWO[32] = {FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE,
                      FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
                      FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
                      FALSE};

/******************************************************************************/
/* Function prototypes */
/******************************************************************************/
BIT not_gate(BIT A);
BIT or_gate(BIT A, BIT B);
BIT or_gate3(BIT A, BIT B, BIT C);
BIT and_gate(BIT A, BIT B);
BIT and_gate3(BIT A, BIT B, BIT C);
BIT and_gate6(BIT A, BIT B, BIT C, BIT X, BIT Y, BIT Z);
BIT xor_gate(BIT A, BIT B);
BIT nor_gate(BIT A, BIT B);
BIT nand_gate(BIT A, BIT B);

void decoder2l(BIT I0, BIT I1, BIT *O0, BIT *O1, BIT *O2, BIT *O3);
void decoder2(BIT* I, BIT EN, BIT* O);
BIT multiplexor2(BIT S, BIT I0, BIT I1);
void multiplexor2_5(BIT S, BIT *I0, BIT *I1, BIT *Output);
void multiplexor2_32(BIT S, BIT *I0, BIT *I1, BIT *Output);
BIT multiplexor4(BIT S0, BIT S1, BIT I0, BIT I1, BIT I2, BIT I3);
void multiplexor4_5(BIT S0, BIT S1, BIT *I0, BIT *I1, BIT *I2, BIT *I3, BIT *Output);
void multiplexor4_32(BIT S0, BIT S1, BIT *I0, BIT *I1, BIT *I2, BIT *I3, BIT *Output);
void adder1(BIT A, BIT B, BIT CarryIn, BIT* CarryOut, BIT* Sum);
void ALU1(BIT A, BIT B, BIT Binvert, BIT CarryIn, BIT Less, 
  BIT Op0, BIT Op1, BIT* Result, BIT* CarryOut, BIT* Set);
void ALU32(BIT* A, BIT* B, BIT Binvert, BIT CarryIn, 
  BIT Op0, BIT Op1, BIT* Zero, BIT* Result, BIT* CarryOut);

void copy_bits(BIT *A, BIT *B);
void print_binary(BIT *A);
void convert_to_binary(int a, BIT *A, int length);
void convert_to_binary_char(int a, char *A, int length);
int binary_to_integer(BIT *A);

int get_instructions(BIT Instructions[][32]);

void Instruction_Memory(BIT *ReadAddress, BIT *Instruction);
void Control(BIT *OpCode, BIT *funct,
             BIT *RegDst, BIT *Jump, BIT * JAL, BIT* JR, BIT *Branch, BIT *MemRead, BIT *MemToReg,
             BIT *ALUOp, BIT *MemWrite, BIT *ALUSrc, BIT *RegWrite);
void Read_Register(BIT *ReadRegister1, BIT *ReadRegister2,
                   BIT *ReadData1, BIT *ReadData2);
void Write_Register(BIT RegWrite, BIT *WriteRegister, BIT *WriteData);
void ALU_Control(BIT *ALUOp, BIT *funct, BIT *ALUControl);
void ALU(BIT *ALUControl, BIT *Input1, BIT *Input2, BIT* Zero, BIT *Result);
void Data_Memory(BIT MemWrite, BIT MemRead,
                 BIT *Address, BIT *WriteData, BIT *ReadData);
void Extend_Sign16(BIT *Input, BIT *Output);
void updateState();

/******************************************************************************/
/* Functions provided for your convenience */
/******************************************************************************/
BIT not_gate(BIT A)
{
  return (!A);
}

BIT or_gate(BIT A, BIT B)
{
  return (A || B);
}

BIT or_gate3(BIT A, BIT B, BIT C)
{
  return or_gate(A, or_gate(B, C));
}

BIT and_gate(BIT A, BIT B)
{
  return (A && B);
}

BIT and_gate3(BIT A, BIT B, BIT C)
{
  return and_gate(A, and_gate(B, C));
}

BIT and_gate6(BIT A, BIT B, BIT C, BIT X, BIT Y, BIT Z)
{
  return and_gate(and_gate3(A,B,C), and_gate3(X,Y,Z));
}

BIT xor_gate(BIT A, BIT B)
{
  BIT nA = not_gate(A);
  BIT nB = not_gate(B);
  BIT x0 = and_gate(A, nB);
  BIT x1 = and_gate(nA, B);
  return or_gate(x0, x1);
}

BIT nor_gate(BIT A, BIT B)
{
  return not_gate(or_gate(A, B));
}

BIT nand_gate(BIT A, BIT B)
{
  return not_gate(and_gate(A, B));
}

void decoder2l(BIT I0, BIT I1, BIT *O0, BIT *O1, BIT *O2, BIT *O3)
{
  // Note: The input -> output mapping is modified from before
  BIT nI1 = not_gate(I1);
  BIT nI0 = not_gate(I0);
  *O0 = and_gate(nI1, nI0);
  *O1 = and_gate(nI1, I0);
  *O2 = and_gate(I1, nI0);
  *O3 = and_gate(I1, I0);

  return;
}

void decoder2(BIT* I, BIT EN, BIT* O)
{
  BIT nI1 = not_gate(I[1]);
  BIT nI0 = not_gate(I[0]);
  O[0] = and_gate(nI1, nI0);
  O[1] = and_gate(nI1, I[0]);
  O[2] = and_gate(I[1], nI0);
  O[3] = and_gate(I[1],  I[0]);
  
  // Note use of EN (enable) line below
  for (int i = 0; i < 4; ++i)
    O[i] = and_gate(EN, O[i]);
  
  return;
}

void decoder3(BIT* I, BIT EN, BIT* O)
{
  // TODO: implement 3-to-8 decoder
  BIT SECOND[4];
  decoder2(I, I[2], SECOND);
  BIT FIRST[4];
  decoder2(I, not_gate(I[2]), FIRST);
  for (int i = 0; i < 4; i++) {
    O[i] = and_gate(FIRST[i], EN);
  }
  for (int i = 0; i < 4; i++) {
    O[i+4] = and_gate(SECOND[i], EN);
  }
  return;
}

void decoder5(BIT* I, BIT EN, BIT* O)
{
  // TODO: implement 5-to-32 decoder using 2-to-4 and 3-to-8 decoders
  BIT input2[2];
  input2[0] = I[3];
  input2[1] = I[4];
  BIT output2[4];
  decoder2(input2, EN, output2);
  BIT FOURTH[8];
  BIT THIRD[8];
  BIT SECOND[8];
  BIT FIRST[8];
  decoder3(I, output2[3], FOURTH);
  decoder3(I, output2[2], THIRD);
  decoder3(I, output2[1], SECOND);
  decoder3(I, output2[0], FIRST);

  for (int i = 0; i < 8; i++) {
    O[i] = and_gate(FIRST[i], EN);
  }
  for (int i = 0; i < 8; i++) {
    O[i+8] = and_gate(SECOND[i], EN);
  }
  for (int i = 0; i < 8; i++) {
    O[i+16] = and_gate(THIRD[i], EN);
  }
  for (int i = 0; i < 8; i++) {
    O[i+24] = and_gate(FOURTH[i], EN);
  }
  return;
}

BIT multiplexor2(BIT S, BIT I0, BIT I1)
{
  BIT nS = not_gate(S);
  BIT x0 = and_gate(nS, I0);
  BIT x1 = and_gate(S, I1);
  return or_gate(x0, x1);
}

void multiplexor2_5(BIT S, BIT *I0, BIT *I1, BIT *Output)
{
  for (int i = 0; i < 5; ++i)
  {
    BIT nS = not_gate(S);
    BIT x0 = and_gate(nS, I0[i]);
    BIT x1 = and_gate(S, I1[i]);
    Output[i] = or_gate(x0, x1);
  }
}

void multiplexor2_32(BIT S, BIT *I0, BIT *I1, BIT *Output)
{
  for (int i = 0; i < 32; ++i)
  {
    BIT nS = not_gate(S);
    BIT x0 = and_gate(nS, I0[i]);
    BIT x1 = and_gate(S, I1[i]);
    Output[i] = or_gate(x0, x1);
  }
}

BIT multiplexor4(BIT S0, BIT S1, BIT I0, BIT I1, BIT I2, BIT I3)
{
  BIT x0, x1, x2, x3 = FALSE;
  decoder2l(S0, S1, &x0, &x1, &x2, &x3);

  BIT y0 = and_gate(x0, I0);
  BIT y1 = and_gate(x1, I1);
  BIT y2 = and_gate(x2, I2);
  BIT y3 = and_gate(x3, I3);

  BIT z0 = or_gate(y0, y1);
  BIT z1 = or_gate(y2, y3);

  return or_gate(z0, z1);
}

void multiplexor4_5(BIT S0, BIT S1, BIT *I0, BIT *I1, BIT *I2, BIT *I3, BIT *Output)
{
  for (int i = 0; i < 5; ++i)
  {
    BIT x0, x1, x2, x3 = FALSE;
    decoder2l(S0, S1, &x0, &x1, &x2, &x3);

    BIT y0 = and_gate(x0, I0[i]);
    BIT y1 = and_gate(x1, I1[i]);
    BIT y2 = and_gate(x2, I2[i]);
    BIT y3 = and_gate(x3, I3[i]);

    BIT z0 = or_gate(y0, y1);
    BIT z1 = or_gate(y2, y3);
    Output[i] = or_gate(z0, z1);
  }
}

void multiplexor4_32(BIT S0, BIT S1, BIT *I0, BIT *I1, BIT *I2, BIT *I3, BIT *Output)
{
  for (int i = 0; i < 32; ++i)
  {
    BIT x0, x1, x2, x3 = FALSE;
    decoder2l(S0, S1, &x0, &x1, &x2, &x3);

    BIT y0 = and_gate(x0, I0[i]);
    BIT y1 = and_gate(x1, I1[i]);
    BIT y2 = and_gate(x2, I2[i]);
    BIT y3 = and_gate(x3, I3[i]);

    BIT z0 = or_gate(y0, y1);
    BIT z1 = or_gate(y2, y3);
    Output[i] = or_gate(z0, z1);
  }
}

void adder1(BIT A, BIT B, BIT CarryIn, BIT* CarryOut, BIT* Sum)
{
  // TODO: implement a 1-bit adder
  // Note: you can probably copy+paste this from your (or my) Lab 5 solution
  BIT x0 = xor_gate(A, B);
  *Sum = xor_gate(CarryIn, x0);
  
  BIT y0 = and_gate(x0, CarryIn);
  BIT y1 = and_gate(A, B);
  *CarryOut = or_gate(y0, y1);
}

void ALU1(BIT A, BIT B, BIT Binvert, BIT CarryIn, BIT Less, 
  BIT Op0, BIT Op1, BIT* Result, BIT* CarryOut, BIT* Set)
{
  // TODO: implement a 1-bit ALU 
  // Note: this will need modifications from Lab 5 to account for 'slt'
  // See slide 30 in Chapter-3d
  BIT nB = not_gate(B);
  BIT x0 = multiplexor2(Binvert, B, nB);

  BIT y0 = and_gate(A, x0);
  BIT y1 = or_gate(A, x0);
  BIT y2 = FALSE;
  adder1(A, x0, CarryIn, CarryOut, &y2);

  *Set = y2;
  *Result = multiplexor4(Op0, Op1, y0, y1, y2, Less);
}


void ALU32(BIT* A, BIT* B, BIT Binvert, BIT CarryIn, 
  BIT Op0, BIT Op1, BIT* Zero, BIT* Result, BIT* CarryOut)
{
  // TODO: implement a 32-bit ALU
  // You'll need to essentially implement a 32-bit ripple addder here
  // See slide 31 in Chapter-3d

  BIT A_1[32], B_1[32];
  copy_bits(A, A_1);
  copy_bits(B, B_1);
  BIT set;
  //begin the first ALU in the ripple
  ALU1(A_1[0], B_1[0], Binvert, CarryIn, 0, Op0, Op1, &Result[0], CarryOut, &set);
  //calculate all the add/sub
  for (int i = 1; i < 32; i++) {
    ALU1(A_1[i], B_1[i], Binvert, *CarryOut, 0, Op0, Op1, &Result[i], CarryOut, &set);
  }
  //feedback, set is fed back into less
  ALU1(A_1[0], B_1[0], Binvert, CarryIn, set, Op0, Op1, &Result[0], CarryOut, &set);

  //zero
  BIT Z = FALSE;
  for (int i = 0; i < 31; i++) {
    Z = or_gate(Result[i], Z);
  }
  Z = not_gate(Z);
  copy_bits(&Z, Zero);
}

/******************************************************************************/
/* Helper functions */
/******************************************************************************/
void copy_bits(BIT *A, BIT *B)
{
  for (int i = 0; i < 32; ++i)
    B[i] = A[i];
}

void print_binary(BIT *A)
{
  for (int i = 31; i >= 0; --i)
    printf("%d", A[i]);
}

// void convert_to_binary_char(int a, char *A, int length)
// {
//   // TODO: perform conversion of integer to binary representation as char array
//   // This uses the same logic as your HW5 implementation. However, you're
//   // converting to a character array instead of a BIT array.
//   // This might be useful in your get_instructions() function, if you use the
//   // same approach that I use. It also might not be needed if you directly
//   // convert the instructions to the proper BIT format.
// }

void intconvert_to_binary(int a, BIT *A)
{

  // Set up if negative value
  int neg = 0;
  if (a < 0)
  {
    a = (a + 1) * (-1);
    neg = 1;
  }

  // Calculate bit values
  int i;
  for (i = 0; i < 31; i++)
  {
    A[i] = a % 2;
    if (a == 1)
      break;
    a /= 2;
  }
  // Set the rest to 0
  i++;
  for (; i < 31; i++)
    A[i] = 0;

  // If negative invert bits
  if (neg == 1)
  {
    for (int k = 0; k < 32; k++)
      A[k] = !A[k];
  }
}

int binary_to_integer(BIT *A)
{
  unsigned a = 0;
  unsigned mult = 1;

  for (int i = 0; i < 32; ++i)
  {
    a += A[i] * mult;
    mult *= 2;
  }

  return (int)a;
}

void operators(char *op, BIT *instruction)
{
  // this is just all the operator codes hardcoded into binary
  if (strcmp(op, "lw") == 0)
  {
    instruction[31] = 1;
    instruction[26] = 1;
    instruction[27] = 1;
  }
  else if (strcmp(op, "sw") == 0){
    instruction[31] = 1;
    instruction[29] = 1;
    instruction[27] = 1;
    instruction[26] = 1;
  }
  else if (strcmp(op, "beq") == 0){
    instruction[28] = 1;
  }
  else if (strcmp(op, "addi") == 0){
    instruction[29] = 1;
  }
  else if (strcmp(op, "and") == 0){
    instruction[5] = 1;
    instruction[2] = 1;
  }
  else if (strcmp(op, "or") == 0){
    instruction[5] = 1;
    instruction[2] = 1;
    instruction[0] = 1;
  }
  else if (strcmp(op, "add") == 0)
  {
    instruction[5] = 1;
  }
  else if (strcmp(op, "sub") == 0){
    instruction[1] = 1;
    instruction[5] = 1;
  }
  else if (strcmp(op, "slt") == 0){
    instruction[1] = 1;
    instruction[3] = 1;
    instruction[5] = 1;
  }
  else if (strcmp(op, "j") == 0)
  {
    instruction[27] = 1;
  }
  else if (strcmp(op, "jal") == 0){
    instruction[27] = 1;
    instruction[26] = 1;
  }
  else if (strcmp(op, "jr") == 0){
    instruction[3] = 1;
  }
}

void register_address(char *reg, BIT *address, int begin, BIT *instruction)
{
  // these are all of the registers that will be used
  if (strcmp(reg, "zero") == 0)
  {
    intconvert_to_binary(0, address);
  }
  else if (strcmp(reg, "v0") == 0)
  {
    intconvert_to_binary(2, address);
  }
  else if (strcmp(reg, "a0") == 0)
  {
    intconvert_to_binary(4, address);
  }
  else if (strcmp(reg, "t0") == 0)
  {
    intconvert_to_binary(8, address);
  }
  else if (strcmp(reg, "t1") == 0)
  {
    intconvert_to_binary(9, address);
  }
  else if (strcmp(reg, "s0") == 0)
  {
    intconvert_to_binary(16, address);
  }
  else if (strcmp(reg, "s1") == 0)
  {
    intconvert_to_binary(17, address);
  }
  else if (strcmp(reg, "sp") == 0)
  {
    intconvert_to_binary(29, address);
  }
  else if (strcmp(reg, "ra") == 0)
  {
    intconvert_to_binary(31, address);
  }
  for (int i = 0; i < 5; i++)
  {
    instruction[begin + i] = address[i];
  }
}

/******************************************************************************/
/* Parsing functions */
/******************************************************************************/

// TODO: Implement any helper functions to assist with parsing

int get_instructions(BIT Instructions[][32])
{
  char line[256] = {0};
  int instruction_count = 0;
  while (fgets(line, 256, stdin) != NULL)
  {
    // TODO: perform conversion of instructions to binary
    // Input: coming from stdin via: ./a.out < input.txt
    // Output: Convert instructions to binary in the instruction memory
    // Return: Total number of instructions
    // Note: you are free to use if-else and external libraries here
    // Note: you don't need to implement circuits for saving to inst. mem.
    // My approach:
    // - Use sscanf on line to get strings for instruction and registers
    // - Use instructions to determine op code, funct, and shamt fields
    // - Convert immediate field and jump address field to binary
    // - Use registers to get rt, rd, rs fields
    // Note: I parse everything as strings, then convert to BIT array at end
    char op[100];
    char rs[100];
    char rt[100];
    char rd[100];
    BIT temp[32];
    BIT Output[32] = {FALSE};
    sscanf(line, "%s", op);
    operators(op, Output);
    if (strcmp(op, "lw") == 0 ||strcmp(op, "sw") == 0 || strcmp(op, "beq") == 0 || strcmp(op, "addi")==0)
    {
      sscanf(line, "%s %s %s %s", op, rt, rs, rd);
      if (strcmp(op, "beq") == 0) {
        register_address(rt, temp, 21, Output);
        register_address(rs, temp, 16, Output);
      } else {
        register_address(rs, temp, 21, Output);
        register_address(rt, temp, 16, Output);
      }
      
      int num = atoi(rd);
      intconvert_to_binary(num, temp);

      for (int i = 0; i < 16; i++)
        Output[i] = temp[i];
    }
    else if (strcmp(op, "and") == 0 || strcmp(op, "or") == 0 || strcmp(op, "add") == 0 || strcmp(op, "sub") == 0 || strcmp(op, "slt") == 0)
    {
      sscanf(line, "%s %s %s %s", op, rd, rs, rt);

      register_address(rs, temp, 21, Output);
      register_address(rt, temp, 16, Output);
      register_address(rd, temp, 11, Output);
    }
    else if (strcmp(op, "j") == 0 || strcmp(op, "jal") == 0)
    {
      sscanf(line, "%s %s", op, rd);

      int num = atoi(rd);
      intconvert_to_binary(num, temp);

      for (int i = 0; i < 26; i++)
        Output[i] = temp[i];
    }
    else if(strcmp(op, "jr") == 0){
      sscanf(line, "%s %s", op, rd);
      register_address(rd,temp, 21, Output);
    }
    // for (int i = 31; i >= 0; --i)
    //   printf("%d", Output[i]);
    for (int i = 0; i < 32; i++) {
      Instructions[instruction_count][i] = Output[i];
    }
    ++instruction_count;
  }

  return instruction_count;
}

/******************************************************************************/
/* Program state - memory spaces, PC, and control */
/******************************************************************************/
BIT PC[32] = {FALSE};
BIT MEM_Instruction[32][32] = {FALSE};
BIT MEM_Data[32][32] = {FALSE};
BIT MEM_Register[32][32] = {FALSE};

BIT RegDst[2] = {FALSE};
BIT Jump = FALSE;
BIT JAL = FALSE;
BIT JR = FALSE;
BIT Branch = FALSE;
BIT MemRead = FALSE;
BIT MemToReg = FALSE;
BIT ALUOp[2] = {FALSE};
BIT MemWrite = FALSE;
BIT ALUSrc = FALSE;
BIT RegWrite = FALSE;
BIT Zero = FALSE;
BIT ALUControl[4] = {FALSE};

void print_instruction()
{
  unsigned pc = binary_to_integer(PC);
  printf("PC: %d\n", pc);
  printf("Instruction: ");
  print_binary(MEM_Instruction[pc]);
  printf("\n");
}

void print_state()
{
  printf("Data: ");
  for (int i = 0; i < 32; ++i)
  {
    printf("%d ", binary_to_integer(MEM_Data[i]));
  }
  printf("\n");

  printf("Register: ");
  for (int i = 0; i < 32; ++i)
  {
    printf("%d ", binary_to_integer(MEM_Register[i]));
  }
  printf("\n");
}

/******************************************************************************/
/* Functions that you will implement */
/******************************************************************************/
void Instruction_Memory(BIT *ReadAddress, BIT *Instruction)
{
  // TODO: Implement instruction memory
  // Input: 32-bit instruction address
  // Output: 32-bit binary instruction
  // Note: Useful to use a 5-to-32 decoder here
  BIT l[32] = {FALSE};
  decoder5(ReadAddress, TRUE, l);
  for (int i = 0; i < 32; i++) {
    multiplexor2_32(l[i], Instruction, MEM_Instruction[i], Instruction);
  }
}

void Control(BIT *OpCode, BIT *funct,
             BIT *RegDst, BIT *Jump, BIT * JAL, BIT* JR, BIT *Branch, BIT *MemRead, BIT *MemToReg,
             BIT *ALUOp, BIT *MemWrite, BIT *ALUSrc, BIT *RegWrite)
{
  // TODO: Set control bits for everything
  // Input: opcode field from the instruction
  // Output: all control lines get set
  // Note: Can use SOP or similar approaches to determine bits

    //1 for jal (000011)
    RegDst[1]   = and_gate6(not_gate(OpCode[5]), not_gate(OpCode[4]), not_gate(OpCode[3]), not_gate(OpCode[2]), OpCode[1], OpCode[0]);

    //1 for R instruction (000000)
    RegDst[0]   = and_gate6(not_gate(OpCode[5]), not_gate(OpCode[4]), not_gate(OpCode[3]), not_gate(OpCode[2]), not_gate(OpCode[1]), not_gate(OpCode[0]));

    //1 for lw (100011), sw (101011), and addi (001000)                   
    *ALUSrc   = or_gate3(and_gate6(OpCode[5], not_gate(OpCode[4]), not_gate(OpCode[3]), not_gate(OpCode[2]), OpCode[1], OpCode[0]),
                        and_gate6(OpCode[5], not_gate(OpCode[4]), OpCode[3], not_gate(OpCode[2]), OpCode[1], OpCode[0]),
                        and_gate6(not_gate(OpCode[5]), not_gate(OpCode[4]), OpCode[3], not_gate(OpCode[2]), not_gate(OpCode[1]), not_gate(OpCode[0])));

    //1 for lw (100011)
    *MemToReg = and_gate6(OpCode[5], not_gate(OpCode[4]), not_gate(OpCode[3]), not_gate(OpCode[2]), OpCode[1], OpCode[0]);                                                                            

    //0 for sw (101011), beq (000100), jump (000010), jr (funct 001000)
    *RegWrite = not_gate(or_gate(or_gate3(and_gate6(OpCode[5], not_gate(OpCode[4]), OpCode[3], not_gate(OpCode[2]), OpCode[1], OpCode[0]),
                        and_gate6(not_gate(OpCode[5]), not_gate(OpCode[4]), not_gate(OpCode[3]), OpCode[2], not_gate(OpCode[1]), not_gate(OpCode[0])),
                        and_gate6(not_gate(OpCode[5]), not_gate(OpCode[4]), not_gate(OpCode[3]), not_gate(OpCode[2]), OpCode[1], not_gate(OpCode[0]))),
                        and_gate6(not_gate(funct[5]), not_gate(funct[4]), funct[3], not_gate(funct[2]), not_gate(funct[1]), not_gate(funct[0]))));

    //1 for lw (100011)
    *MemRead  = and_gate6(OpCode[5], not_gate(OpCode[4]), not_gate(OpCode[3]), not_gate(OpCode[2]), OpCode[1], OpCode[0]);

    //1 for sw (101011)
    *MemWrite = and_gate6(OpCode[5], not_gate(OpCode[4]), OpCode[3], not_gate(OpCode[2]), OpCode[1], OpCode[0]);

    //1 for beq (000100)
    *Branch   = and_gate6(not_gate(OpCode[5]), not_gate(OpCode[4]), not_gate(OpCode[3]), OpCode[2], not_gate(OpCode[1]), not_gate(OpCode[0]));

    //1 for R instruction (000000)
    ALUOp[1] = and_gate6(not_gate(OpCode[5]), not_gate(OpCode[4]), not_gate(OpCode[3]), not_gate(OpCode[2]), not_gate(OpCode[1]), not_gate(OpCode[0]));

    //1 for beq (000100)
    ALUOp[0] = and_gate6(not_gate(OpCode[5]), not_gate(OpCode[4]), not_gate(OpCode[3]), OpCode[2], not_gate(OpCode[1]), not_gate(OpCode[0]));

    //1 for jump (000010) and jal (000011)
    *Jump     = or_gate(and_gate6(not_gate(OpCode[5]), not_gate(OpCode[4]), not_gate(OpCode[3]), not_gate(OpCode[2]), OpCode[1], not_gate(OpCode[0])),
                       and_gate6(not_gate(OpCode[5]), not_gate(OpCode[4]), not_gate(OpCode[3]), not_gate(OpCode[2]), OpCode[1], OpCode[0]));

    //1 for jal (000011)
    *JAL      = and_gate6(not_gate(OpCode[5]), not_gate(OpCode[4]), not_gate(OpCode[3]), not_gate(OpCode[2]), OpCode[1], OpCode[0]);

    //1 for jr (has R instruction OpCode but has funct 001000)
    *JR       = and_gate6(not_gate(funct[5]), not_gate(funct[4]), funct[3], not_gate(funct[2]), not_gate(funct[1]), not_gate(funct[0]));
}

void Read_Register(BIT *ReadRegister1, BIT *ReadRegister2,
                   BIT *ReadData1, BIT *ReadData2)
{
  // TODO: Implement register read functionality
  // Input: two 5-bit register addresses
  // Output: the values of the specified registers in ReadData1 and ReadData2
  // Note: Implementation will be very similar to instruction memory circuit
  BIT l[32] = {FALSE};
  BIT r[32] = {FALSE};
  decoder5(ReadRegister1, TRUE, l);
  decoder5(ReadRegister2, TRUE, r);

  for (int i = 0; i < 32; i++) {
    multiplexor2_32(l[i], ReadData1, MEM_Register[i], ReadData1);
    multiplexor2_32(r[i], ReadData2, MEM_Register[i], ReadData2);
  }
}

void Write_Register(BIT RegWrite, BIT *WriteRegister, BIT *WriteData)
{
  // TODO: Implement register write functionality
  // Input: one 5-bit register address, data to write, and control bit
  // Output: None, but will modify register file
  // Note: Implementation will again be similar to those above
  BIT l[32] = {FALSE};
  decoder5(WriteRegister, RegWrite, l);

  for (int i = 0; i < 32; i++) {
    // write
    multiplexor2_32(and_gate(RegWrite, l[i]), MEM_Register[i], WriteData, MEM_Register[i]);
  }

}

void ALU_Control(BIT *ALUOp, BIT *funct, BIT *ALUControl)
{
  // TODO: Implement ALU Control circuit
  // Input: 2-bit ALUOp from main control circuit, 6-bit funct field from the
  //        binary instruction
  // Output: 4-bit ALUControl for input into the ALU
  // Note: Can use SOP or similar approaches to determine bits

  //always 0, with the given instructions
  ALUControl[3] = FALSE;

  //1 for beq (X1 XXXXXX), sub (1X XX0010), slt (1X XX1010) // this bit represents binvert/carryin
  ALUControl[2] = or_gate3(ALUOp[0], 
                           and_gate6(ALUOp[1], not_gate(funct[3]), not_gate(funct[2]), funct[1], not_gate(funct[0]), TRUE),
                           and_gate6(ALUOp[1], funct[3], not_gate(funct[2]), funct[1], not_gate(funct[0]), TRUE));

  //0 for and (1X XX0100), or (1X XX0101) // op1
  ALUControl[1] = not_gate(or_gate(and_gate6(ALUOp[1], not_gate(funct[3]), funct[2], not_gate(funct[1]), not_gate(funct[0]), TRUE),
                                   and_gate6(ALUOp[1], not_gate(funct[3]), funct[2], not_gate(funct[1]), funct[0], TRUE)));

  //1 for slt (1X XX1010), or (1X XX0101) // op0
  ALUControl[0] = or_gate(and_gate6(ALUOp[1], funct[3], not_gate(funct[2]), funct[1], not_gate(funct[0]), TRUE), 
                          and_gate6(ALUOp[1], not_gate(funct[3]), funct[2], not_gate(funct[1]), funct[0], TRUE));
}

void ALU(BIT *ALUControl, BIT *Input1, BIT *Input2, BIT* Zero, BIT *Result)
{
  // TODO: Implement 32-bit ALU
  // Input: 4-bit ALUControl, two 32-bit inputs
  // Output: 32-bit result, and zero flag big
  // Note: Can re-use prior implementations (but need new circuitry for zero)
  BIT CarryOut = FALSE;
  ALU32(Input1, Input2, ALUControl[2], ALUControl[2], ALUControl[0], ALUControl[1], Zero, Result, &CarryOut);

}


void Data_Memory(BIT MemWrite, BIT MemRead, 
                 BIT *Address, BIT *WriteData, BIT *ReadData)
{
  // TODO: Implement data memory
  // Input: 32-bit address, control flags for read/write, and data to write
  // Output: data read if processing a lw instruction
  // Note: Implementation similar as above
  BIT l[32] = {FALSE};
  decoder5(Address, TRUE, l);

  for (int i = 0; i < 32; i++) {
    // read
    multiplexor2_32(and_gate3(MemRead, not_gate(MemWrite), l[i]), ReadData, MEM_Data[i], ReadData);
    // write
    multiplexor2_32(and_gate3(not_gate(MemRead), MemWrite, l[i]), MEM_Data[i], WriteData, MEM_Data[i]);
  }

}

void Extend_Sign16(BIT *Input, BIT *Output)
{
  // TODO: Implement 16-bit to 32-bit sign extender
  // Copy Input to Output, then extend 16th Input bit to 17-32 bits in Output
  BIT temp0[32] = {FALSE};
  BIT temp1[32] = {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
               TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
               TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
               TRUE};
  for (int i = 0; i < 16; i++) {
      temp0[i] = Input[i];
      temp1[i] = Input[i];
  }

  multiplexor2_32(Input[15], temp0, temp1, Output);
}

void AssignInstructions(BIT* instr, BIT* op, BIT* reg1, BIT* reg2, BIT* write, BIT* ad, BIT* funct, BIT* jumpad) {
  int j = 0;
  for (int i = 0; i < 5; i++) {
    funct[j] = instr[i];
    j++;
  }
  j = 0;
  for (int i = 0; i < 16; i++) {
    ad[j] = instr[i];
    j++;
  }
  j = 0;
  for (int i = 11; i < 16; i++) {
    write[j] = instr[i];
    j++;
  }
  j = 0;
  for (int i = 16; i < 21; i++) {
    reg2[j] = instr[i];
    j++;
  }
  j = 0;
  for (int i = 21; i < 26; i++) {
    reg1[j] = instr[i];
    j++;
  }
  j = 0;
  for (int i = 26; i < 32; i++) {
    op[j] = instr[i];
    j++;
  }
  j = 0;
  for (int i = 0; i < 26; i++) {
    jumpad[j] = instr[i];
    j++;
  }
  for (int i = 26; i < 32; i++) {
    jumpad[i] = jumpad[25];
  }
}

void updateState()
{
  // TODO: Implement the full datapath here
  // Essentially, you'll be figuring out the order in which to process each of
  // the sub-circuits comprising the entire processor circuit. It makes it
  // easier to consider the pipelined version of the process, and handle things
  // in order of the pipeline. The stages and operations are:

  //jump address +1
  BIT nextPC[32] = {FALSE};
  BIT CarryOut;
  ALU32(PC, ONE, 0, 0, 0, 1, &Zero, nextPC, &CarryOut);


  // Fetch - load instruction from instruction memory **************

  BIT instruction[32] = {FALSE};
  Instruction_Memory(PC, instruction);

  // Decode - set control bits and read from the register file **************

  BIT OpCode[6] = {FALSE};
  BIT ReadRegister1[5] = {FALSE};
  BIT ReadRegister2[5] = {FALSE};
  BIT WriteReg[5] = {FALSE};
  BIT Address16[16] = {FALSE};

  //for ALU Control
  BIT Funct[5] = {FALSE};

  //for updating PC
  BIT JumpAd[32] = {FALSE};

  //sign extending the jump address from 26-32 is included
  AssignInstructions(instruction, OpCode, ReadRegister1, ReadRegister2, WriteReg, Address16, Funct, JumpAd);

  //takes in Funct only for JR control bit
  Control(OpCode, Funct, RegDst, &Jump, &JAL, &JR, &Branch, &MemRead, &MemToReg, ALUOp, &MemWrite, &ALUSrc, &RegWrite);

  BIT ReadData1[32] = {FALSE};
  BIT ReadData2[32] = {FALSE};
  Read_Register(ReadRegister1, ReadRegister2, ReadData1, ReadData2);

  //determines the write register for the write back later on
  BIT WriteRegister[5] = {FALSE};
  BIT tmp1[5] = {FALSE};
  multiplexor4_5(RegDst[0], RegDst[1], ReadRegister2, WriteReg, REG_THIRTY_ONE, tmp1, WriteRegister);

  BIT Address32[32] = {FALSE};
  Extend_Sign16(Address16, Address32);
  
  // Execute - process ALU **************

  ALU_Control(ALUOp, Funct, ALUControl);
  BIT ALUResult[32] = {FALSE};
  BIT ALUInput2[32] = {FALSE};
  multiplexor2_32(ALUSrc, ReadData2, Address32, ALUInput2);
  ALU(ALUControl, ReadData1, ALUInput2, &Zero, ALUResult);

  // Memory - read/write data memory **************

  BIT ReadData[32] = {FALSE};
  Data_Memory(MemWrite, MemRead, ALUResult, ReadData2, ReadData);

  // Write Back - write to the register file **************

  BIT WriteData[32] = {FALSE};
  multiplexor2_32(MemToReg, ALUResult, ReadData, WriteData);
  //determine if we want to write back the next instruction
  multiplexor2_32(JAL, WriteData, nextPC, WriteData);

  Write_Register(RegWrite, WriteRegister, WriteData);


  // Update PC - determine the final PC value for the next instruction **************
  
  BIT pc2[32] = {FALSE};  
  BIT pc1[32] = {FALSE};
  BIT tmp = {FALSE};

  //jr
  multiplexor2_32(JR, nextPC, ReadData1, pc1);

  ALU32(nextPC, Address32, 0, 0, 0, 1, &tmp, pc2, &CarryOut);

  //branch
  multiplexor2_32(and_gate(Branch, Zero), pc1, pc2, pc2);

  //jump/jal
  multiplexor2_32(Jump, pc2, JumpAd, PC);
}

/******************************************************************************/
/* Main */
/******************************************************************************/

int main()
{
  setbuf(stdout, NULL);

  // parse instructions into binary format
  int counter = get_instructions(MEM_Instruction);

  // load program and run
  copy_bits(ZERO, PC);
  copy_bits(THIRTY_TWO, MEM_Register[29]);

  while (binary_to_integer(PC) < counter)
  {
    print_instruction();
    updateState();
    print_state();
  }

  return 0;
}
