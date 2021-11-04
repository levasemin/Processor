#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "commands.h"
#include "stack.h"

#define VERSION "1.02"
#define SIGNATURE "DED!"

typedef stack_type cpu_type;

const int MAX_LABEL_ARG = 20;
const int MAX_SIZE_LINE = 30;
const int DEFAULT_CAPACITY_CODE = 10;
const int DEFAULT_CAPACITY_LABELS = 10;

const int REGISTER_COUNT = 26;
const int RAM_SIZE  = 1024;

enum cpu_states
{
    RUNNING_ASSEMBLER            = 0,
    RUNNING_PROCESSOR            = 1,
    RUNNING_DISASSEMBLER         = 2,
    STOP_HLT                     = 3,
    STOP_INVALID_SIGNATURE       = 4,
    STOP_INVALID_VERSION         = 5,
    STOP_UNDECLARED_ASSEMBLER    = 6,
    STOP_UNDECLARED_PROCESSOR    = 7,
    STOP_UNDECLARED_DISASSEMBLER = 8
};

//names of states stack
const char * const cpu_states_name[] =
{
        "running assembler",
        "running processor",
        "running disassembler",
        "was stopped by hlt",
        "invalid signature",
        "invalid version",
        "found undeclared command (assembler)",
        "found undeclared command (processor)",
        "found undeclared command (disassembler)"
};


struct strings
{
    char *start;
    char *end;
};

struct ram
{
    size_t count;
    int memmory[RAM_SIZE];
};

struct label
{
    size_t ip;
    char name[MAX_LABEL_ARG];
};

struct labels
{
    size_t count;
    size_t labels_capacity;
    label *labels;
};

struct assembler
{
    size_t ip;

    size_t code_capacity;
    char *code;

    labels all_labels;

    int state;
};

struct cpu
{
    size_t ip;

    size_t code_capacity;
    char *code;

    Stack stack;
    ram ram;
    cpu_type registers[REGISTER_COUNT];

    Stack back_call;

    int state;
};

void assemble(const char *input_file_name, const char *output_file_name, const char *log_file_name);

void assemble(FILE *input, FILE *output, FILE*log_file);


void processor(const char *output_file_name, const char *log_file_name);

void processor(FILE *output, FILE *log_file);


void disassemble(const char *output_file_name, const char *dis_file_name, const char *log_file_name);

void disassemble(FILE *output, FILE *log_file, FILE *disas_file);


void Initialize_assembler(assembler *my_assembler, int state, Verification ver, size_t default_capacity_code, size_t default_capacity_labels);

int check_verification(int *state, Verification ver_target, Verification ver);


void push_back_label(labels *all_labels, label lab);


strings *get_strings_data(char *string, size_t count_string);

size_t get_file_size(FILE *file);

size_t read_file(FILE *file, char **string);

size_t get_count_string(char *string);


void dump_log_file(assembler *my_assembler, FILE *file);

void dump_log_file(cpu *my_cpu, FILE *file);
