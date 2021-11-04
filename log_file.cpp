#include "processor.h"

void dump_log_file(cpu *my_cpu, FILE *file)
{
    assert(file != nullptr);

    Verification ver = *(Verification *)my_cpu->code;

    fprintf(file, "state: %s \n", cpu_states_name[my_cpu->state]);
    fprintf(file, "expected signature: %s real signature %s \n", SIGNATURE, ver.signature);
    fprintf(file, "expected version: %s real version %s \n", VERSION, ver.version);
    fprintf(file, "ip: %Iu \n", my_cpu->ip);

    for (size_t i = 0; i < my_cpu->code_capacity; i++)
    {
        fprintf(file, "%02x ", (unsigned char)my_cpu->code[i]);
    }

    fprintf(file, "\n");

    for (size_t i = 0; i < my_cpu->ip; i++)
    {
        fprintf(file, "   ");
    }

    fprintf(file, "^");
    
    fprintf(file, "\n\n");

    fprintf(file, "stack:\n");

    for (size_t i = 0; i < my_cpu->stack.size; i++)
    {
        fprintf(file, "[%Iu] %d\n", my_cpu->stack.size, pop_stack(&my_cpu->stack));
    }

    fprintf(file, "\n\n");

    fprintf(file, "stack: \n");

    for (int i = 0; i < REGISTER_COUNT; i ++)
    {
        fprintf(file, "%cx: %d\n", 'a' + i, my_cpu->registers[i]);
    }

    fprintf(file, "\n");
    fprintf(file, "ram memory: \n");

    for (int i = 0; i < RAM_SIZE; i ++)
    {
        if (my_cpu->ram.memmory[i] != 0)
            fprintf(file, "[%d]: %d\n", i, my_cpu->ram.memmory[i]);
    }

    fprintf(file, "\n");
    fprintf(file, "-------------------------------------------------------------------------------------------\n\n\n");

}

void dump_log_file(assembler *my_assembler, FILE *file)
{
    assert(file != nullptr);

    Verification ver = *(Verification *)my_assembler->code;

    fprintf(file, "state: %s \n", cpu_states_name[my_assembler->state]);
    fprintf(file, "expected signature: %s real signature %s \n", SIGNATURE, ver.signature);
    fprintf(file, "expected version: %s real version %s \n", VERSION, ver.version);
    fprintf(file, "ip: %Iu \n", my_assembler->ip);

    for (size_t i = 0; i < my_assembler->code_capacity; i++)
    {
        fprintf(file, "%02x ", (unsigned char)my_assembler->code[i]);
    }

    fprintf(file, "\n");

    for (size_t i = 0; i < my_assembler->ip; i++)
    {
        fprintf(file, "   ");
    }

    fprintf(file, "^");

    fprintf(file, "\n\n");

    fprintf(file, "stack:\n");


    fprintf(file, "\n\n");

    fprintf(file, "stack: \n");

    fprintf(file, "\n");
    fprintf(file, "-------------------------------------------------------------------------------------------\n\n\n");

}

