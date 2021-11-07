#include "processor.h"

#define DISASSEMBLER

#define DEF_CMD(Cmd, num, args, code_proc, code_dis) \
    case CMD_##Cmd:                                  \
    code_dis                                         \
    break;

#define DUMP_CONSOLE(operation)                       \
    getchar();                                        \
    fprintf(stderr, "%d \n", operation);              \
    dump_log_file(&my_disassembler, stderr);


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

void Initialize_disassembler(assembler *my_disassembler, int state, FILE *byte_file)
{
    size_t count_elements = read_file(byte_file, &my_disassembler->code);

    my_disassembler->state = state;
    my_disassembler->code = my_disassembler->code;
    my_disassembler->code_capacity = count_elements;
}

void disassemble(const char *byte_file_name, const char *dis_file_name)
{
    assert(byte_file_name  != nullptr);
    assert(dis_file_name   != nullptr);
    assert(log_file_name   != nullptr);

    FILE *byte_file = fopen(byte_file_name, "rb");
    FILE *log_file  = fopen(log_file_name, "ab");
    FILE *dis_file  = fopen(dis_file_name, "wb");

    assert(byte_file != nullptr);
    assert(log_file  != nullptr);
    assert(dis_file  != nullptr);

    disassemble(byte_file, dis_file, log_file);
}

void disassemble(FILE *byte_file, FILE *dis_file, FILE *log_file)
{
    assert(byte_file != nullptr);
    assert(log_file  != nullptr);
    assert(dis_file  != nullptr);

    assembler my_disassembler = {};

    Initialize_disassembler(&my_disassembler, RUNNING_DISASSEMBLER, byte_file);

    Verification ver_target = {SIGNATURE, VERSION};
    Verification ver = *(Verification *)my_disassembler.code;
    

    if (check_verification(&(my_disassembler.state), ver_target, ver))
    {
        dump_log_file(&my_disassembler, log_file);
        assert (0);
    }

    for (int j = 0; j < 2; ++j)
    {
        my_disassembler.ip = sizeof(Verification);

        while (my_disassembler.ip < my_disassembler.code_capacity)
        {
            //DUMP_CONSOLE(((Flag *)(my_disassembler.code + my_disassembler.ip))->OPERATION)
            
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
}


static const char* default_dis_file_name    = "examples/disas_commands.txt";

static const char* default_byte_file_name   = "examples/out_commands.txt";

int main(int argc, char *argv[]) {
    
    const char *dis_file_name = default_dis_file_name;

    const char *byte_file_name = default_byte_file_name;

    if (argc >= 2)
    {
        byte_file_name = argv[1];
    }

    if (argc >= 3)
    {
        dis_file_name = argv[2];
    }

    disassemble(byte_file_name, dis_file_name);
}

#undef DEF_CMD
#undef DISASSEMBLER