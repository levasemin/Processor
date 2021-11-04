#include "processor.h"

const char* input_file_name  = "C:\\Users\\levce\\CLionProjects\\ded\\examples\\rec.txt";
const char* output_file_name = "C:\\Users\\levce\\CLionProjects\\ded\\examples\\out_commands.txt";
const char* log_file_name    = "C:\\Users\\levce\\CLionProjects\\ded\\log\\log.txt";
const char* dis_file_name    = "C:\\Users\\levce\\CLionProjects\\ded\\examples\\disas_commands.txt";

int main() {
    //assemble(input_file_name, output_file_name, log_file_name);
    processor(output_file_name, log_file_name);
    //disassemble(output_file_name, dis_file_name, log_file_name);
}