#include "processor.h"

#define command_const(operation) {1, 0, 0, operation}
#define command_reg(operation) {0, 1, 0, operation}
#define command_reg_mem(operation) {0, 1, 1, operation}
#define command_mem(operation) {0, 0, 1, operation}
#define command_none(operation) {0, 0, 0, operation}
#define command_string(operation) {1, 1, 1, operation}

#define DUMP_CONSOLE(cmd)                             \
    getchar();                                        \
    fprintf(stderr, "line: %Iu \n", i + 1);           \
    fprintf(stderr, "%s \n", cmd);                    \
    dump_log_file(&my_assembler, stderr);

#define push_back_flag_value(type, value)                                                                               \
    if (my_assembler->code_capacity <= my_assembler->ip + sizeof(Flag) + sizeof(value))                                 \
    {                                                                                                                   \
        if (my_assembler->code_capacity == 0)                                                                           \
        {                                                                                                               \
            my_assembler->code_capacity += DEFAULT_CAPACITY_CODE;                                                       \
        }                                                                                                               \
                                                                                                                        \
        my_assembler->code_capacity = my_assembler->ip + sizeof(Flag) + sizeof(value);                                  \
                                                                                                                        \
        my_assembler->code = (char *)realloc(my_assembler->code, my_assembler->code_capacity * 2);                      \
                                                                                                                        \
        assert(my_assembler->code != nullptr);                                                                          \
                                                                                                                        \
        my_assembler->code_capacity *= 2;                                                                               \
    }                                                                                                                   \
                                                                                                                        \
    if (flag.CONST_FLAG != 0 || flag.REG_FLAG != 0 || flag.MEM_FLAG != 0)                                               \
    {                                                                                                                   \
        *(type *)(void *)(my_assembler->code + my_assembler->ip) = value;                                               \
        my_assembler->ip += sizeof(value);                                                                              \
    }                                                                                                                   \


void prepeare_register(assembler *my_assembler, strings *data, char *cmd, Flag flag);

void prepeare_ram(assembler *my_assembler, strings *data, char *cmd, Flag flag);

void prepeare_const(assembler *my_assembler, strings *data, char *cmd, Flag flag);

void prepeare_none(assembler *my_assembler, Flag flag);

void prepeare_label(assembler *my_assembler, strings *data, char *cmd, Flag flag, int j);

void prepeare_string(assembler *my_assembler, strings *data, char *cmd, Flag flag);



void prepeare_register(assembler *my_assembler, strings *data, char *cmd, Flag flag)
{
    char register_arg = 0;
    char type = 0;
    int count_of_scan = sscanf(data->start, "%s %c%c", cmd, &register_arg, &type);

    *(Flag *)(my_assembler->code + (my_assembler->ip) ++) = flag;

    if (register_arg < 'a' || register_arg > 'z' || count_of_scan != 3)
    {
        my_assembler->state = STOP_UNDECLARED_ASSEMBLER;
        return;
    }

    else
    {
        push_back_flag_value(char, (char)(register_arg - 'a'));
    }
}


void prepeare_ram(assembler *my_assembler, strings *data, char *cmd, Flag flag)
{
    int memory_arg = 0;
    char register_arg = 0;
    char type = 0;
    int count_of_scan = sscanf(data->start, "%s [%c%c + %d]", cmd, &register_arg, &type, &memory_arg);

    if (count_of_scan == 4)
    {
        if (register_arg < 'a' || register_arg > 'z')
        {
            my_assembler->state = STOP_UNDECLARED_ASSEMBLER;
            return;
        }

        flag = command_reg_mem(flag.OPERATION);
        *(Flag *)(my_assembler->code + (my_assembler->ip) ++) = flag;

        push_back_flag_value(char, char(register_arg - 'a'));
        push_back_flag_value(int, memory_arg);
    }

    else
    {
        count_of_scan = sscanf(data->start, "%s [%d + %c%c]", cmd, &memory_arg, &register_arg, &type);

        if (count_of_scan == 4)
        {
            if (register_arg < 'a' || register_arg > 'z')
            {
                my_assembler->state = STOP_UNDECLARED_ASSEMBLER;
                return;
            }

            flag = command_reg_mem(flag.OPERATION);
            *(Flag *)(my_assembler->code + (my_assembler->ip) ++) = flag;

            push_back_flag_value(char, char(register_arg - 'a'));
            push_back_flag_value(int, memory_arg);
        }

        else
        {
            count_of_scan = sscanf(data->start, "%s [%d]", cmd, &memory_arg);

            if (count_of_scan == 2)
            {
                flag = command_mem(flag.OPERATION);
                *(Flag *)(my_assembler->code + (my_assembler->ip) ++) = flag;

                push_back_flag_value(int, memory_arg);
            }
        }
    }
}

void prepeare_const(assembler *my_assembler, strings *data, char *cmd, Flag flag)
{
    int arg = 0;

    if ((strcmp(cmd, "DB")) == 0)
    {
        flag = command_string(flag.OPERATION);                                                                  

        struct str
        {
            char text [MAX_SIZE_LINE];
        };

        str text = {};                                                                                                  

        sscanf(data->start, "%s %s", cmd, text.text);                                                            

        *(Flag *)(my_assembler->code + (my_assembler->ip) ++) = flag;        

        push_back_flag_value(str, text);
    }

    else
    {
        sscanf(data->start, "%s %d", cmd, &arg);

        if (strcmp(cmd, "pop") != 0 && strcmp(cmd, "in") != 0 && strcmp(cmd, "out") != 0)
        {
            flag = command_const(flag.OPERATION);
            *(Flag *)(my_assembler->code + (my_assembler->ip) ++) = flag;

            push_back_flag_value(int, arg);
        }

        else
        {
            flag = command_none(flag.OPERATION);
            *(Flag *)(my_assembler->code + (my_assembler->ip) ++) = flag;

            push_back_flag_value(char, -1);
        }
    }
}


 void prepeare_none(assembler *my_assembler, Flag flag)
{
     *(Flag *)(my_assembler->code + (my_assembler->ip) ++) = flag;
     push_back_flag_value(char, -1);
}

void prepeare_label(assembler *my_assembler, strings *data, char *cmd, Flag flag, int j)
 {
    char func_label_arg[MAX_LABEL_ARG];
    int count_of_scan = sscanf(data->start, "%s %s", cmd, func_label_arg);

    if (count_of_scan != 2)
    {
        my_assembler->state = STOP_UNDECLARED_ASSEMBLER;
        return;
    }

    else
    {
        if (j == 0)
        {
            int value = 0;

            flag = command_const(flag.OPERATION);
            *(Flag *)(my_assembler->code + (my_assembler->ip) ++) = flag;

            push_back_flag_value(int, value);
        }

        else
        {
            for (size_t l = 0; l < my_assembler->all_labels.count; l ++)
                {
                    if (strcmp(my_assembler->all_labels.labels[l].name, func_label_arg) == 0)
                    {
                        flag = command_const(flag.OPERATION);
                        *(Flag *)(my_assembler->code + (my_assembler->ip) ++) = flag;

                        push_back_flag_value(int, (int)my_assembler->all_labels.labels[l].ip);
                        break;
                    }
                }
        }
    }
}

#define DEF_CMD(Cmd, num, args, ...)                                                                                    \
if (strcmp(cmd, #Cmd) == 0 && num <= SIMPLE_COMMANDS)                                                                   \
    {                                                                                                                   \
        {                                                                                                               \
            if (args > 0)                                                                                               \
            {                                                                                                           \
                if (*(data->end - 2) == 'x')                                                                            \
                {                                                                                                       \
                    Flag flag = command_reg(CMD_##Cmd);                                                                 \
                    prepeare_register(&my_assembler, data, cmd, flag);                                                  \
                }                                                                                                       \
                                                                                                                        \
                else if (*(data->end - 2) == ']')                                                                       \
                {                                                                                                       \
                    Flag flag = command_mem(CMD_##Cmd);                                                                 \
                                                                                                                        \
                    prepeare_ram(&my_assembler, data, cmd, flag);                                                       \
                }                                                                                                       \
                                                                                                                        \
                else                                                                                                    \
                {                                                                                                       \
                    Flag flag = command_const(CMD_##Cmd);                                                               \
                                                                                                                        \
                    prepeare_const(&my_assembler, data, cmd, flag);                                                     \
                }                                                                                                       \
            }                                                                                                           \
                                                                                                                        \
            else                                                                                                        \
            {                                                                                                           \
                Flag flag = command_none(CMD_##Cmd);                                                                    \
                                                                                                                        \
                prepeare_none(&my_assembler, flag);                                                                     \
            }                                                                                                           \
        }                                                                                                               \
    }                                                                                                                   \
else if (strcmp(cmd, #Cmd) == 0)                                                                                        \
    {                                                                                                                   \
        Flag flag = command_const(CMD_##Cmd);                                                                           \
                                                                                                                        \
        prepeare_label(&my_assembler, data, cmd, flag, j);                                                              \
    }                                                                                                                   \
    else                                                                                                                \



void Initialize_my_assembler(assembler *my_assembler, int state, Verification ver, size_t default_capacity_code,
                                                                                size_t default_capacity_labels);

size_t get_prepeare_strings(FILE *input, strings **data);

int read_command(strings *data, char *cmd);

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


int read_command(strings *data, char *cmd)
{
    if (*data->start == '#')
    {
        return -1;
    }
    int arg = 0;
    int count_of_scan = sscanf(data->start, "%s %d", cmd, &arg);

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

void assemble(const char *input_file_name, const char *output_file_name)
{
    assert(input_file_name  != nullptr);
    assert(output_file_name != nullptr);

    FILE *input    = fopen(input_file_name, "rb");
    FILE *output   = fopen(output_file_name, "wb");
    FILE *log_file = fopen(log_file_name, "ab");

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
                    dump_log_file(&my_assembler, log_file);

    size_t count_strings = get_prepeare_strings(input, &data);

    assert(data != nullptr);

    char label_arg[MAX_LABEL_ARG] = "";
    
    for (int j = 0; j < 2; ++j)
    {
        my_assembler.ip = sizeof(Verification);
        for (size_t i = 0; i < count_strings; ++i)
        {
            
            char cmd[MAX_SIZE_LINE] = "";

            int count_of_scan = read_command(data, cmd);
            
            //DUMP_CONSOLE(cmd);
            
            if ( count_of_scan == -1)
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
                fprintf(log_file, "line: %Ilu \n", i + 1);
                dump_log_file(&my_assembler, log_file);

                assert(0);
            }

            data += 1;
        }

        data -= count_strings;
    }

    fwrite(my_assembler.code, sizeof(char), my_assembler.ip, output);
    
    fclose(input);
    fclose(output);
}

static const char* default_input_file_name    = "examples/square.txt";

static const char* default_output_file_name   = "examples/out_commands.txt";

int main(int argc, char *argv[]) {
    
    const char *input_file_name = default_input_file_name;

    const char *output_file_name = default_output_file_name;

    if (argc >= 2)
    {
        input_file_name = argv[1];
    }

    if (argc >= 3)
    {
        output_file_name = argv[2];
    }

    assemble(input_file_name, output_file_name);
}

#undef DEF_CMD