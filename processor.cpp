#include "processor.h"

const int REG_PRESISION = 'p'-'a';
const int DEFAULT_PRESISION =  10;

#define PROCESSOR

#define DEF_CMD(Cmd, num, args, code_proc, code_dis) \
    case CMD_##Cmd:                                  \
    code_proc\
    break;

#define DUMP_CONSOLE(operation)                       \
    getchar();                                        \
    fprintf(stderr, "%d \n", operation);              \
    dump_log_file(&my_processor, stderr);


void Initialize_cpu_processor(cpu *my_cpu, int state, FILE *byte_file);



void Initialize_cpu_processor(cpu *my_cpu, int state, FILE *byte_file)
{
    size_t count_elements = read_file(byte_file, &my_cpu->code);

    my_cpu->state = state;
    my_cpu->code = my_cpu->code;
    my_cpu->code_capacity = count_elements;

    stack_constructor(&my_cpu->stack);

    stack_constructor(&my_cpu->back_call);

    my_cpu->registers[REG_PRESISION] = DEFAULT_PRESISION;
}


void processor(const char *byte_file_name)
{
    assert(byte_file_name != nullptr);
    assert(log_file_name  != nullptr);

    FILE *byte_file = fopen(byte_file_name, "rb");
    FILE *log_file  = fopen(log_file_name, "a");

    assert(byte_file != nullptr);
    assert(log_file != nullptr);

    processor(byte_file, log_file);
}


void processor(FILE *byte_file, FILE *log_file)
{     
    assert(byte_file != nullptr);
    assert(log_file != nullptr);

    cpu my_cpu = {};

    Initialize_cpu_processor(&my_cpu, RUNNING_PROCESSOR, byte_file);

    Verification ver_target = {SIGNATURE, VERSION};
    Verification ver = {SIGNATURE, VERSION};
            dump_log_file(&my_cpu, log_file);
    
    if (check_verification(&(my_cpu.state), ver_target, ver))
    {
        dump_log_file(&my_cpu, log_file);
        assert (0);
    }

    my_cpu.ip += sizeof(Verification);

    while (my_cpu.ip < my_cpu.code_capacity && my_cpu.state == RUNNING_PROCESSOR)
    {
        //DUMP_CONSOLE(((Flag *)(my_cpu.code + my_cpu.ip))->OPERATION)

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
}



static const char* default_byte_file_name = "examples/byte_rec.bin";

int main(int argc, char *argv[]) 
{
    
    const char *byte_file_name = default_byte_file_name;

    if (argc >= 2)
    {
        byte_file_name = argv[1];
    }

    processor(byte_file_name);
}
#undef DEF_CMD
#undef PROCESSOR
