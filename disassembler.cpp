#include "processor.h"

#define DISASSEMBLER

#define DEF_CMD(Cmd, num, args, code_proc, code_dis) \
    case CMD_##Cmd:                                  \
    code_dis                                         \
    break;

void fprintf_func_label_name(FILE*file, char operation[5], assembler *my_disassembler, size_t necessary_ip);

void generate_push_label(assembler *my_disassembler);

int check_verification(Verification ver_target, assembler *my_assembler, FILE *log_file);

void Initialize_disassembler(assembler *my_disassembler, int state, FILE *output);



void generate_push_label(assembler *my_disassembler)
{
    assert(my_disassembler != nullptr);

    char name = (char) my_disassembler->all_labels.count;

    label lab = {(size_t)*(int *)(my_disassembler->code + my_disassembler->ip), {}};
    lab.name[0] = name;

    push_back_label(&my_disassembler->all_labels, lab);
}

void fprintf_func_label_name(FILE*file, char operation[5], assembler *my_disassembler, size_t necessary_ip)
{
    assert(file != nullptr);

    for (size_t i = 0; i < my_disassembler->all_labels.count; ++i)
    {
        if (necessary_ip == my_disassembler->all_labels.labels[i].ip)
        {
            fprintf(file, "%s%d:\n", operation, my_disassembler->all_labels.labels[i].name[0] + 1);
            break;
        }
    }
}

void disassemble(const char *output_file_name, const char *dis_file_name, const char *log_file_name)
{
    assert(output_file_name != nullptr);
    assert(dis_file_name    != nullptr);
    assert(log_file_name    != nullptr);

    FILE *output = fopen(output_file_name, "rb");
    FILE *log_file = fopen(log_file_name, "a");
    FILE *dis_file = fopen(dis_file_name, "w");

    assert(output != nullptr);
    assert(log_file != nullptr);
    assert(dis_file != nullptr);

    disassemble(output, log_file, dis_file);
}

int check_verification(int *state, Verification ver_target, Verification ver)
{
    if (strcmp(ver.signature, ver_target.signature))
    {
        *state = STOP_INVALID_SIGNATURE;
        return 1;
    }

    if (strcmp(ver.version, ver_target.version))
    {
        *state = STOP_INVALID_VERSION;
        return 1;
    }
    return 0;
}

void Initialize_disassembler(assembler *my_disassembler, int state, FILE *output)
{
    size_t count_elements = read_file(output, &my_disassembler->code);

    my_disassembler->state = state;
    my_disassembler->code = my_disassembler->code;
    my_disassembler->code_capacity = count_elements;
}

void disassemble(FILE *output, FILE *log_file, FILE *dis_file)
{
    assert(output != nullptr);
    assert(log_file != nullptr);
    assert(dis_file != nullptr);

    assembler my_disassembler = {};

    Initialize_disassembler(&my_disassembler, RUNNING_DISASSEMBLER, output);

    Verification ver_target = {SIGNATURE, VERSION};
    Verification ver = *(Verification *)my_disassembler.code;
    
    if (check_verification(&(my_disassembler.state),ver_target, ver))
    {
        dump_log_file(&my_disassembler, log_file);
        assert (0);
    }

    for (int j = 0; j < 2; ++j)
    {
        my_disassembler.ip = sizeof(Verification);

        while (my_disassembler.ip < my_disassembler.code_capacity)
        {
            if (my_disassembler.all_labels.count > 0 && j == 1)
            {
                char operation[5] = "";
                fprintf_func_label_name(dis_file, operation, &my_disassembler, my_disassembler.ip);
            }

            switch(((Flag *)(my_disassembler.code + my_disassembler.ip))->OPERATION)
            {
                #include "cmd_def.h"

                default:
                    my_disassembler.state = STOP_UNDECLARED_DISASSEMBLER;

                    printf("Error undeclared");

                    dump_log_file(&my_disassembler, log_file);

                    assert(0);
                    break;
            }
        }
    }

    dump_log_file(&my_disassembler, log_file);
}

#undef DEF_CMD
#undef DISASSEMBLER