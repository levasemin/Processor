#include "processor.h"

const int REG_PRESISION = 'p'-'a';
const int DEFAULT_PRESISION =  10;

#define PROCESSOR

#define DEF_CMD(Cmd, num, args, code_proc, code_dis) \
    case CMD_##Cmd:                                  \
    code_proc                                        \
    break;


void Initialize_cpu_processor(cpu *my_cpu, int state, FILE *output);



void Initialize_cpu_processor(cpu *my_cpu, int state, FILE *output)
{
    size_t count_elements = read_file(output, &my_cpu->code);

    my_cpu->state = state;
    my_cpu->code = my_cpu->code;
    my_cpu->code_capacity = count_elements;

    stack_constructor(&my_cpu->stack);

    stack_constructor(&my_cpu->back_call);

    my_cpu->registers[REG_PRESISION] = DEFAULT_PRESISION;
}


void processor(const char *output_file_name, const char *log_file_name)
{
    assert(output_file_name != nullptr);
    assert(log_file_name != nullptr);

    FILE *output = fopen(output_file_name, "rb");
    FILE *log_file = fopen(log_file_name, "a");

    assert(output != nullptr);
    assert(log_file != nullptr);

    processor(output, log_file);
}


void processor(FILE *output, FILE *log_file)
{
    assert(output != nullptr);
    assert(log_file != nullptr);

    cpu my_cpu = {};

    Initialize_cpu_processor(&my_cpu, RUNNING_PROCESSOR, output);

    Verification ver_target = {SIGNATURE, VERSION};
    Verification ver = {SIGNATURE, VERSION};

    if (check_verification(&(my_cpu.state), ver_target, ver))
    {
        dump_log_file(&my_cpu, log_file);
        assert (0);
    }

    my_cpu.ip += sizeof(Verification);

    while (my_cpu.ip < my_cpu.code_capacity && my_cpu.state == RUNNING_PROCESSOR)
    {
        switch(((Flag *)(my_cpu.code + my_cpu.ip))->OPERATION)
        {
            #include "cmd_def.h"
            default:
                printf("Error undeclared");

                my_cpu.state = STOP_UNDECLARED_PROCESSOR;
                dump_log_file(&my_cpu, log_file);

                assert(0);
                break;
        }
    }

    dump_log_file(&my_cpu, log_file);
}

#undef DEF_CMD
#undef PROCESSOR
