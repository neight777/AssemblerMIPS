/**
 * @file consts.c
 * @brief Constants used throughout codebase.
 * @author Cameron Hannay
 */
#include "include.h"

// TODO: Add in more documentation/comments since Old Jim forgot

const uint32_t UNDEFINED;

const uint32_t MAX_5_BIT = 31;
const uint32_t MAX_6_BIT = 63;
const uint32_t MAX_16_BIT = 65535;
const uint32_t MAX_26_BIT = 67108863;

const char* registers[] = {
    "$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
    "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
    "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
    "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"
};

const struct instruction_definition instruction_definitions[] = {
    // Three "Golden Standard" Instructions
    // The ADD instruction shows what each part of the instruction defintion means
    {
        "ADD",                  // MIPS instruction name
        R_TYPE,                 // Type of instruction (R, I, or J)
        "000000",               // Opcode
        "100000",               // Funct Code
        { RD, RS, RT, EMPTY }   // Registers
    },                          // This would be read as: add $rd, $rs, $rt

    {
        "ADDI",
        I_TYPE,
        "001000",
        NULL,
        { RT, RS, IMM, EMPTY }
    },

    {
        "BNE",
        I_TYPE,
        "000101",
        NULL,
        { RS, RT, IMM, EMPTY }
    },

    //TODO: Fix the bugs in the other instructions to restore functionality

    {
        "AND",
        R_TYPE,                 //Changed from I-type to R-type 
        "000000",               //R-type so opcode changed to 000000
        "100100",               //funct code is correct
        { RD, RS, RT, EMPTY }
    },

    {
        "ANDI",
        I_TYPE,                 //changed to I-type from R-type
        "001100",
        NULL,
        { RT, RS, IMM, EMPTY } //changed to RT, RS, IMM, EMPTY
    },

    {
        "BEQ",
        I_TYPE,                 //changed to I-type from R-type
        "000100",               //opcode changed to 000100 to match correct MIPS BEQ
        NULL,                   //funct code changed to null since I-type instructions don't use one
        { RS, RT, IMM, EMPTY }
    },

    {
        "DIV",
        R_TYPE,
        "000000",
        "011010",
        { RS, RT, EMPTY, EMPTY }//added Rt since it was EMPTY before
    },

    {
        "LUI",
        I_TYPE,
        "001111",
        NULL,
        { RT, IMM, EMPTY, EMPTY } //First is RT and IMM is second
    },

    {
        "LW",
        I_TYPE,                 //Changed to I-type from R-type
        "100011",               
        NULL,
        { RT, IMM, RS, EMPTY }  //Changed to match syntax RT, IMM, RS
    },

    {
        "MFHI",                     
        R_TYPE,
        "000000",
        "010000",                   //changed from 010010 to 010000
        { RD, EMPTY, EMPTY, EMPTY } //Replaced RS with RD for correct syntax
    },

    {
        "MFLO",
        R_TYPE,
        "000000",                   
        "010010",                   //changed to correct funct code of 010010
        { RD, EMPTY, EMPTY, EMPTY } //Replaced RS with RD for correct syntax
    },

    {
        "OR",                   
        R_TYPE,
        "000000",               
        "100101",             //changed to correct MIPS funct code for OR
        { RD, RS, RT, EMPTY }
    },

    {
        "SLT",
        R_TYPE,
        "000000",
        "101010",             //changed from 101011 to 101010
        { RD, RS, RT, EMPTY } 
    },

    {
        "SUB",
        R_TYPE,
        "000000",               //changed opcode to 000000 for R-type
        "100010",               //changed funct code to 100010 to match funct code
        { RD, RS, RT, EMPTY }
    },

    {
        "SW",
        I_TYPE,
        "101011",             //change opcode to be correct
        NULL,
        { RT, IMM, RS, EMPTY }//change from RS IMM RT to RT IMM RS to match syntax
    },
    
    // TODO: Implement MULT, ORI, and SLTI instructions once bugs are fixed

    {
        "MULT",         //MIPS Instruction name
        R_TYPE,         //MULT is an R-type Instruction
        "000000",       //R-type opcode
        "011000",       //Mult Funct code
        { RS, RT, EMPTY, EMPTY } //Match MULT syntax
    },

    {
        "ORI",          //MIPS instruction name
        I_TYPE,         //ORI is an I-type instruction
        "001101",       //ORI opcode 
        NULL,           //funct code blank for I-type
        { RT, RS, IMM, EMPTY } //match ORI syntax  
    },

    {
        "SLTI",         //MIPS instruction name
        I_TYPE,         //SLTI is an I-type instruction
        "001010",       //SLTI opcode
        NULL,           //funct code blank for I-type
        { RT, RS, IMM, EMPTY } //match SLTI syntax
    }
};