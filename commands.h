#include <stdio.h>

struct Verification
{
    char signature[5];
    char version[5];
};

struct Flag
{
    unsigned char CONST_FLAG : 1;
    unsigned char REG_FLAG   : 1;
    unsigned char MEM_FLAG   : 1;
    unsigned char OPERATION  : 5;
};

enum Command
{
    CMD_NOP = 0,
    CMD_HLT,
    CMD_POP,
    CMD_PUSH,
    CMD_ADD,
    CMD_MUL,
    CMD_SUB,
    CMD_DIV,
    CMD_SQRT,
    CMD_OUT,
    CMD_IN,
    CMD_RET,
    CMD_LOG,
    CMD_DB,

    SIMPLE_COMMANDS,

    CMD_JMP,
    CMD_JA,
    CMD_JB,
    CMD_JE,
    CMD_JAE,
    CMD_JBE,
    CMD_JNE,
    CMD_CALL,
};