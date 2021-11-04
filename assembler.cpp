#include "processor.h"

#define command_const(operation) {1, 0, 0, operation}
#define command_reg(operation) {0, 1, 0, operation}
#define command_reg_mem(operation) {0, 1, 1, operation}
#define command_mem(operation) {0, 0, 1, operation}
#define command_none(operation) {0, 0, 0, operation}

#define push_back_flag_value(type, value)                                                                               \
    if (my_assembler.code_capacity <= my_assembler.ip + sizeof(Flag) + sizeof(value))                                   \
    {                                                                                                                   \
        if (my_assembler.code_capacity == 0)                                                                            \
        {                                                                                                               \
            my_assembler.code_capacity += DEFAULT_CAPACITY_CODE;                                                        \
        }                                                                                                               \
                                                                                                                        \
        my_assembler.code = (char *)realloc(my_assembler.code, my_assembler.code_capacity * 2);                         \
                                                                                                                        \
        assert(my_assembler.code != nullptr);                                                                           \
                                                                                                                        \
        my_assembler.code_capacity *= 2;                                                                                \
    }                                                                                                                   \
                                                                                                                        \
    if (flag.CONST_FLAG != 0 || flag.REG_FLAG != 0 || flag.MEM_FLAG != 0)                                               \
    {                                                                                                                   \
        *(type *)(my_assembler.code + my_assembler.ip) = value;                                                         \
        my_assembler.ip += sizeof(value);                                                                               \
    }                                                                                                                   \


#define prepeare_register(Cmd)                                                                                          \
    char register_arg = 0;                                                                                              \
                                                                                                                        \
    count_of_scan = sscanf(data->start, "%s %c%c", cmd, &register_arg, &type);                                          \
    if (register_arg < 'a' || register_arg > 'z' || count_of_scan != 3)                                                 \
    {                                                                                                                   \
        my_assembler.state = STOP_UNDECLARED_ASSEMBLER;                                                                 \
        break;                                                                                                          \
    }                                                                                                                   \
    else                                                                                                                \
    {                                                                                                                   \
        cmd_reg(Cmd)                                                                                                    \
    }


#define cmd_reg(Cmd)                                                                                                    \
    Flag flag = command_reg(CMD_##Cmd);                                                                                 \
    *(Flag *)(my_assembler.code + (my_assembler.ip) ++) = flag;                                                         \
    push_back_flag_value(char, (char)(register_arg - 'a'));


#define cmd_reg_mem(Cmd)                                                                                                \
    if (register_arg < 'a' || register_arg > 'z')                                                                       \
    {                                                                                                                   \
        my_assembler.state = STOP_UNDECLARED_ASSEMBLER;                                                                 \
        break;                                                                                                          \
    }                                                                                                                   \
                                                                                                                        \
    Flag flag = command_reg_mem(CMD_##Cmd);                                                                             \
    *(Flag *)(my_assembler.code + (my_assembler.ip) ++) = flag;                                                         \
    push_back_flag_value(char, char(register_arg - 'a'))                                                                \
    push_back_flag_value(int, memory_arg);                                                                              \


#define prepeare_ram(Cmd)                                                                                               \
    int memory_arg = 0;                                                                                                 \
    char register_arg = 0;                                                                                              \
    count_of_scan = sscanf(data->start, "%s [%c%c + %d]", cmd, &register_arg, &type, &memory_arg);                      \
                                                                                                                        \
    if (count_of_scan == 4)                                                                                             \
    {                                                                                                                   \
        cmd_reg_mem(Cmd)                                                                                                \
    }                                                                                                                   \
                                                                                                                        \
    else                                                                                                                \
    {                                                                                                                   \
        count_of_scan = sscanf(data->start, "%s [%d + %c%c]", cmd, &memory_arg, &register_arg, &type);                  \
        if (count_of_scan == 4)                                                                                         \
        {                                                                                                               \
            cmd_reg_mem(Cmd)                                                                                            \
        }                                                                                                               \
                                                                                                                        \
        else                                                                                                            \
        {                                                                                                               \
            count_of_scan = sscanf(data->start, "%s [%d]", cmd, &memory_arg);                                           \
                                                                                                                        \
            if (count_of_scan == 2)                                                                                     \
            {                                                                                                           \
                cmd_ram(Cmd)                                                                                            \
            }                                                                                                           \
        }                                                                                                               \
    }                                                                                                                   \



#define cmd_ram(Cmd)                                                                                                    \
Flag flag = command_mem(CMD_##Cmd);                                                                                     \
*(Flag *)(my_assembler.code + (my_assembler.ip) ++) = flag;                                                             \
push_back_flag_value(int, memory_arg);                                                                                  \


#define prepeare_const(Cmd)\
    cmd_const(Cmd)


#define cmd_const(Cmd)                                                                                                  \
if (strcmp(cmd, "POP") != 0 && strcmp(cmd, "IN") != 0 && strcmp(cmd, "OUT") != 0)                                       \
{                                                                                                                       \
    Flag flag = command_const(CMD_##Cmd);                                                                               \
    *(Flag *)(my_assembler.code + (my_assembler.ip) ++) = flag;                                                         \
    push_back_flag_value(int, arg);                                                                                     \
}                                                                                                                       \
                                                                                                                        \
else                                                                                                                    \
{                                                                                                                       \
    Flag flag = command_none(CMD_##Cmd);                                                                                \
    *(Flag *)(my_assembler.code + (my_assembler.ip) ++) = flag;                                                         \
    push_back_flag_value(char, -1);                                                                                     \
}                                                                                                                       \

#define prepeare_label(Cmd)                                                                                             \
char func_label_arg[MAX_LABEL_ARG];                                                                                     \
count_of_scan = sscanf(data->start, "%s %s", cmd, func_label_arg);                                                      \
                                                                                                                        \
if (count_of_scan != 2)                                                                                                 \
{                                                                                                                       \
    my_assembler.state = STOP_UNDECLARED_ASSEMBLER;                                                                     \
    break;                                                                                                              \
}                                                                                                                       \
                                                                                                                        \
else                                                                                                                    \
{                                                                                                                       \
    cmd_label(Cmd)                                                                                                      \
}


#define cmd_label(Cmd)                                                                                                  \
if (j == 0)                                                                                                             \
    {                                                                                                                   \
        int value = 0;                                                                                                  \
        Flag flag = command_const(CMD_##Cmd);                                                                           \
        *(Flag *)(my_assembler.code + (my_assembler.ip) ++) = flag;                                                     \
        push_back_flag_value(int, value);                                                                               \
    }                                                                                                                   \
                                                                                                                        \
else                                                                                                                    \
{                                                                                                                       \
    for (size_t l = 0; l < my_assembler.all_labels.count; l ++)                                                         \
        {                                                                                                               \
            if (strcmp(my_assembler.all_labels.labels[l].name, func_label_arg) == 0)                                    \
            {                                                                                                           \
                Flag flag = command_const(CMD_##Cmd);                                                                   \
                *(Flag *)(my_assembler.code + (my_assembler.ip) ++) = flag;                                             \
                push_back_flag_value(int, (int)my_assembler.all_labels.labels[l].ip);                                   \
                break;                                                                                                  \
            }                                                                                                           \
        }                                                                                                               \
}

#define DEF_CMD(Cmd, num, args, ...)                                                                                    \
if (strcmp(cmd, #Cmd) == 0 && num <= SIMPLE_COMMANDS)                                                                   \
    {                                                                                                                   \
        {                                                                                                               \
            if (args > 0)                                                                                               \
            {                                                                                                           \
                if (*(data->end - 1) == 'x')                                                                            \
                {                                                                                                       \
                    prepeare_register(Cmd)                                                                              \
                }                                                                                                       \
                                                                                                                        \
                else if (*(data->end - 1) == ']')                                                                       \
                {                                                                                                       \
                    prepeare_ram(Cmd)                                                                                   \
                }                                                                                                       \
                                                                                                                        \
                else                                                                                                    \
                {                                                                                                       \
                    prepeare_const(Cmd)                                                                                 \
                }                                                                                                       \
            }                                                                                                           \
                                                                                                                        \
            else                                                                                                        \
                {                                                                                                       \
                    Flag flag = command_none(CMD_##Cmd);                                                                \
                    *(Flag *)(my_assembler.code + (my_assembler.ip) ++) = flag;                                         \
                    push_back_flag_value(char, -1);                                                                     \
                }                                                                                                       \
        }                                                                                                               \
    }                                                                                                                   \
else if (strcmp(cmd, #Cmd) == 0)                                                                                        \
    {                                                                                                                   \
        prepeare_label(Cmd)                                                                                             \
    }                                                                                                                   \
    else                                                                                                                \



void Initialize_my_assembler(assembler *my_assembler, int state, Verification ver, size_t default_capacity_code,
                                                                                size_t default_capacity_labels);

size_t get_prepeare_strings(FILE *input, strings **data);

int read_command(strings *data, char *cmd, int *arg);

void add_label(assembler *my_assembler, strings *data, char *label_arg);


size_t get_prepeare_strings(FILE *input, strings **data)
{
    assert(input != nullptr);
    assert(data  != nullptr);

    char *string = nullptr;

    read_file(input, &string);

    assert(string != nullptr);

    size_t count_strings = get_count_string(string);

    *data = get_strings_data(string, count_strings);

    assert(*data != nullptr);

    return count_strings;
}


void Initialize_assembler(assembler *my_assembler, int state, Verification ver, size_t default_capacity_code,
                                                                                size_t default_capacity_labels)
{

    my_assembler->state = state;

    my_assembler->all_labels.labels_capacity = default_capacity_labels;

    my_assembler->all_labels = {0, default_capacity_labels, (label *) calloc(default_capacity_labels, sizeof(label))};

    assert(my_assembler->all_labels.labels != nullptr);

    my_assembler->code_capacity = sizeof(Verification) + default_capacity_code;
    my_assembler->code = (char *)calloc(my_assembler->code_capacity, sizeof(char));

    assert(my_assembler->code  != nullptr);
    assert(my_assembler->all_labels.labels != nullptr);

    *(Verification *)(my_assembler->code + my_assembler->ip) = ver;
}


int read_command(strings *data, char *cmd, int *arg)
{
    if (data->start == data->end || *data->start == '#')
    {
        return 0;
    }

    int count_of_scan = sscanf(data->start, "%s %d", cmd, arg);

    if (strchr(cmd, '#') != nullptr)
        *(strchr(cmd, '#')) = '\0';

    for (int i = 0; cmd[i] != '\0'; ++i)
    {
        cmd[i] = (char)toupper(cmd[i]);
    }
    return count_of_scan;
}


void add_label(assembler *my_assembler, strings *data, char *label_arg)
{
    sscanf(data->start, "%s", label_arg);
    bool label_was = false;

    for (size_t k = 0; k < my_assembler->all_labels.count; ++k)
    {
        if (strcmp(my_assembler->all_labels.labels[k].name, label_arg) == 0)
        {
            my_assembler->all_labels.labels[k].ip = my_assembler->ip;
            label_was = true;
        }
    }

    if (!label_was)
    {
        label lab = {my_assembler->ip, {}};
        sscanf(data->start, "%s", lab.name);
        push_back_label(&my_assembler->all_labels, lab);
    }
}

void assemble(const char *input_file_name, const char *output_file_name, const char *log_file_name)
{
    assert(input_file_name  != nullptr);
    assert(output_file_name != nullptr);
    assert(log_file_name    != nullptr);

    FILE *input = fopen(input_file_name, "r");
    FILE *output = fopen(output_file_name, "wb");
    FILE *log_file = fopen(log_file_name, "a");

    assert(input    != nullptr);
    assert(output   != nullptr);
    assert(log_file != nullptr);

    assemble(input, output, log_file);
}

void assemble(FILE *input, FILE *output, FILE *log_file)
{
    assert(input    != nullptr);
    assert(output   != nullptr);
    assert(log_file != nullptr);

    Verification ver = {SIGNATURE, VERSION};

    assembler my_assembler = {};
    Initialize_assembler(&my_assembler, RUNNING_ASSEMBLER, ver, DEFAULT_CAPACITY_CODE, DEFAULT_CAPACITY_LABELS);

    strings *data = nullptr;

    size_t count_strings = get_prepeare_strings(input, &data);

    assert(data != nullptr);

    int  arg  = 0;
    char type = 0;

    char label_arg[MAX_LABEL_ARG] = "";
    
    for (int j = 0; j < 2; ++j)
    {
        my_assembler.ip = sizeof(Verification);
        for (size_t i = 0; i < count_strings; ++i)
        {
            char cmd[MAX_SIZE_LINE] = "";

            int count_of_scan = read_command(data, cmd, &arg);
            if ( count_of_scan == 0)
            {
                data += 1;
                continue;
            }
            #include "cmd_def.h"
            {
                if (strchr(data->start, ':'))
                {
                    add_label(&my_assembler, data, label_arg);
                }

                else
                {
                    my_assembler.state = STOP_UNDECLARED_ASSEMBLER;
                }
            }

            if (my_assembler.state != RUNNING_ASSEMBLER)
            {
                fprintf(log_file, "line: %Iu \n", i + 1);
                dump_log_file(&my_assembler, log_file);

                assert(0);
            }

            data += 1;
        }

        data -= count_strings;
    }

    fwrite(my_assembler.code, sizeof(char), my_assembler.ip, output);

    dump_log_file(&my_assembler, log_file);
}

#undef DEF_CMD