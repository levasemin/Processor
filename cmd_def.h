#ifdef PROCESSOR
#define CUR_FLAG        *(Flag*)(void *)(my_cpu.code + my_cpu.ip)
#define CUR_BYTE        *(my_cpu.code + my_cpu.ip)

#define CUR_INT         *(int *)(void *)(my_cpu.code + my_cpu.ip)
#define CUR_INT_NEXT    CUR_INT;                                                                                        \
                        my_cpu.ip += sizeof(int);

#define REG_VALUE       my_cpu.registers[(int)CUR_BYTE]

#define REG_INDEX_NEXT  my_cpu.registers[(int)CUR_BYTE] / my_cpu.registers[REG_PRESISION];                              \
                        ++my_cpu.ip;
#endif

#ifdef DISASSEMBLER
#define CUR_FLAG        *(Flag*)(void *)(my_disassembler.code + my_disassembler.ip)
#define CUR_BYTE        *(my_disassembler.code + my_disassembler.ip)
#define CUR_INT         *(int *)(void *)(my_disassembler.code + my_disassembler.ip)

#define CUR_INT_NEXT    CUR_INT; \
                        my_disassembler.ip += sizeof(int);
#endif

#define GET_EL1_EL2                                                                                                     \
cpu_type el1 = pop_stack(&my_cpu.stack);                                                                                \
cpu_type el2 = pop_stack(&my_cpu.stack);                                                                                \
push_stack(&my_cpu.stack, &el2);                                                                                        \
push_stack(&my_cpu.stack, &el1);

#define PRINTF_ELEMENT                                          \
    if (el < 0)                                                 \
    {                                                           \
        el *= -1;                                               \
        printf("-");                                            \
    }                                                           \
                                                                \
printf("%d.", el / my_cpu.registers[REG_PRESISION]);            \
printf("%d\n", el % my_cpu.registers[REG_PRESISION]);

DEF_CMD(POP, CMD_POP, 1,
{
    if ((CUR_FLAG).CONST_FLAG == 0 && (CUR_FLAG).REG_FLAG == 0 && (CUR_FLAG).MEM_FLAG == 0)
    {
        ++my_cpu.ip;
        pop_stack(&my_cpu.stack);
    }

    else if ((CUR_FLAG).REG_FLAG && (CUR_FLAG).MEM_FLAG)
    {
        ++my_cpu.ip;

        int reg_index = REG_INDEX_NEXT

        int number_index = CUR_INT_NEXT;

        my_cpu.ram.memmory[reg_index + number_index] = pop_stack(&my_cpu.stack);
    }

    else if ((CUR_FLAG).REG_FLAG)
    {
        ++my_cpu.ip;
        my_cpu.registers[(int)CUR_BYTE] = pop_stack(&my_cpu.stack);
        ++my_cpu.ip;
    }

    else if ((CUR_FLAG).MEM_FLAG)
    {
        ++my_cpu.ip;
        my_cpu.ram.memmory[CUR_INT] = pop_stack(&my_cpu.stack);
        my_cpu.ip += sizeof(int);
    }

},
{
    if ((CUR_FLAG).CONST_FLAG == 0 && (CUR_FLAG).REG_FLAG == 0 && (CUR_FLAG).MEM_FLAG == 0)
    {
        if (j == 1)
        {
            fprintf(dis_file, "pop\n");
        }

        ++my_disassembler.ip;
    }

    else if ((CUR_FLAG).REG_FLAG && (CUR_FLAG).MEM_FLAG)
    {
        ++my_disassembler.ip;
        int cur_byte = CUR_BYTE;

        ++my_disassembler.ip;

        int number_index = CUR_INT_NEXT
                
        if (j == 1)
        {
            fprintf(dis_file, "pop [%cx + %d]", cur_byte + 'a', number_index);
        }
    }

    else if ((CUR_FLAG).REG_FLAG)
    {
        ++my_disassembler.ip;

        if (j == 1)
        {
            fprintf(dis_file, "pop %cx\n", CUR_BYTE + 'a');
        }

        ++my_disassembler.ip;
    }

    else if ((CUR_FLAG).MEM_FLAG)
    {
        ++my_disassembler.ip;

        if (j == 1)
        {
            fprintf(dis_file, "pop [%d]\n", CUR_INT);
        }

        my_disassembler.ip += sizeof(int);
    }
})

DEF_CMD(PUSH, CMD_PUSH, 1,
{
    if ((CUR_FLAG).CONST_FLAG)
    {
        ++my_cpu.ip;
        cpu_type el = CUR_INT * my_cpu.registers[REG_PRESISION];
        push_stack(&my_cpu.stack, &el);
        my_cpu.ip += sizeof(int);
    }

    else if ((CUR_FLAG).REG_FLAG && (CUR_FLAG).MEM_FLAG)
    {
        ++my_cpu.ip;

        int reg_index = REG_INDEX_NEXT

        int number_index = CUR_INT_NEXT

        cpu_type el = my_cpu.ram.memmory[reg_index + number_index];
        push_stack(&my_cpu.stack , &el);
    }

    else if ((CUR_FLAG).REG_FLAG)
    {
        ++my_cpu.ip;
        cpu_type el = my_cpu.registers[(int)CUR_BYTE];
        push_stack(&my_cpu.stack, &el);
        ++my_cpu.ip;
    }

    else if ((CUR_FLAG).MEM_FLAG)
    {
        ++my_cpu.ip;
        cpu_type el = my_cpu.ram.memmory[CUR_INT];
        push_stack(&my_cpu.stack, &el);
        my_cpu.ip += sizeof(int);
    }
},
{
    if ((CUR_FLAG).CONST_FLAG)
    {
        ++my_disassembler.ip;

        if (j == 1)
        {
            fprintf(dis_file, "push %d\n", CUR_INT);
        }

        my_disassembler.ip += sizeof(int);
    }

    else if ((CUR_FLAG).REG_FLAG && (CUR_FLAG).MEM_FLAG)
    {
        ++my_disassembler.ip;

        int cur_byte = CUR_BYTE;
        ++my_disassembler.ip;

        int number_index = CUR_INT_NEXT

        if (j == 1)
        {
            fprintf(dis_file, "push [%cx + %d]\n", cur_byte + 'a', number_index);
        }
    }
    else if ((CUR_FLAG).REG_FLAG)
    {
        ++my_disassembler.ip;

        if (j == 1)
        {
            fprintf(dis_file, "push %cx\n", CUR_BYTE + 'a');
        }

        ++my_disassembler.ip;
    }
    else if ((CUR_FLAG).MEM_FLAG)
    {
        ++my_disassembler.ip;

        if (j == 1)
        {
            fprintf(dis_file, "push %cx\n", CUR_INT);
        }

        my_disassembler.ip += sizeof(int);
    }
})

DEF_CMD(IN, CMD_IN, 1,
{
    cpu_type el = 0;

    scanf("%d", &el);
    
    el *= my_cpu.registers[REG_PRESISION];

    if ((CUR_FLAG).CONST_FLAG == 0 && (CUR_FLAG).REG_FLAG == 0 && (CUR_FLAG).MEM_FLAG == 0)
    {
        ++my_cpu.ip;
        push_stack(&my_cpu.stack, &el);
    }

    else if ((CUR_FLAG).REG_FLAG && (CUR_FLAG).MEM_FLAG)
    {
        ++my_cpu.ip;

        int reg_index = REG_INDEX_NEXT

        int number_index = CUR_INT_NEXT

        my_cpu.ram.memmory[reg_index + number_index] = el;
    }

    else if ((CUR_FLAG).REG_FLAG)
    {
        ++my_cpu.ip;

        if (CUR_BYTE == REG_PRESISION)
        {
            el = (int)pow(10, el/my_cpu.registers[REG_PRESISION]);
        }
        my_cpu.registers[(int)CUR_BYTE] = el;
        ++my_cpu.ip;
    }

    else if ((CUR_FLAG).MEM_FLAG)
    {
        ++my_cpu.ip;
        my_cpu.ram.memmory[CUR_INT] = el;
        my_cpu.ip += sizeof(int);
    }
},
{
    if ((CUR_FLAG).CONST_FLAG == 0 && (CUR_FLAG).REG_FLAG == 0 && (CUR_FLAG).MEM_FLAG == 0)
    {
        ++my_disassembler.ip;

        if (j == 1)
        {
            fprintf(dis_file, "in\n");
        }
    }

    else if ((CUR_FLAG).REG_FLAG && (CUR_FLAG).MEM_FLAG)
    {
        ++my_disassembler.ip;

        int cur_byte = CUR_BYTE;
        ++my_disassembler.ip;

        int number_index = CUR_INT_NEXT

        if (j == 1)
        {
            fprintf(dis_file, "in [%cx + %d]\n", cur_byte + 'a', number_index);
        }

    }

    else if ((CUR_FLAG).REG_FLAG)
    {
        ++my_disassembler.ip;

        if (j == 1)
        {
            fprintf(dis_file, "in %cx\n", CUR_BYTE + 'a');
        }

        ++my_disassembler.ip;
    }

    else if ((CUR_FLAG).MEM_FLAG)
    {
        ++my_disassembler.ip;

        if (j == 1)
        {
            fprintf(dis_file, "in [%d]\n", CUR_INT);
        }

        my_disassembler.ip += sizeof(int);
    }
})

DEF_CMD(ADD, CMD_ADD, 0,
{
    cpu_type a = pop_stack(&my_cpu.stack);
    cpu_type b = pop_stack(&my_cpu.stack);
    cpu_type c = a + b;

    push_stack(&my_cpu.stack, &c);

    ++my_cpu.ip;
},
{
    if (j == 1)
    {
        fprintf(dis_file, "add\n");
    }

    ++my_disassembler.ip;
})

DEF_CMD(MUL, CMD_MUL, 0,
{
    cpu_type a = pop_stack(&my_cpu.stack);
    cpu_type b = pop_stack(&my_cpu.stack);
    cpu_type c = a * b / my_cpu.registers[REG_PRESISION];
    push_stack(&my_cpu.stack, &c);

    ++my_cpu.ip;
},
{
    if (j == 1)
    {
        fprintf(dis_file, "mul\n");
    }

    ++my_disassembler.ip;
})

DEF_CMD(SUB, CMD_SUB, 0,
{
    cpu_type a = pop_stack(&my_cpu.stack);
    cpu_type b = pop_stack(&my_cpu.stack);
    cpu_type c = b - a;

    push_stack(&my_cpu.stack, &c);

    ++my_cpu.ip;
},
{
    if (j == 1)
    {
        fprintf(dis_file, "sub\n");
    }

    ++my_disassembler.ip;
})

DEF_CMD(DIV, CMD_DIV, 0,
{
    cpu_type a = pop_stack(&my_cpu.stack);
    cpu_type b = pop_stack(&my_cpu.stack);
    cpu_type c = b * my_cpu.registers[REG_PRESISION] / a;

    push_stack(&my_cpu.stack, &c);

    ++my_cpu.ip;
},
{
    if (j == 1)
    {
        fprintf(dis_file, "div\n");
    }

    ++my_disassembler.ip;
})

DEF_CMD(SQRT, CMD_SQRT, 0,
{
    cpu_type a = (int)sqrt(pop_stack(&my_cpu.stack) * my_cpu.registers[REG_PRESISION]);

    push_stack(&my_cpu.stack, &a);

    ++my_cpu.ip;
},
{
    if (j == 1)
    {
        fprintf(dis_file, "sqrt\n");
    }

    ++my_disassembler.ip;
})

DEF_CMD(RET, CMD_RET, 0,
{
     my_cpu.ip = (size_t)pop_stack(&my_cpu.back_call);
},
{
    if (j == 1)
    {
        fprintf(dis_file, "ret\n");
    }

    ++my_disassembler.ip;
})


DEF_CMD(OUT, CMD_OUT, 1,
{
    if ((CUR_FLAG).CONST_FLAG == 0 && (CUR_FLAG).REG_FLAG == 0 && (CUR_FLAG).MEM_FLAG == 0)
    {
        cpu_type el = top_stack(&my_cpu.stack);
        PRINTF_ELEMENT
        ++my_cpu.ip;
    }

    else if ((CUR_FLAG).REG_FLAG && (CUR_FLAG).MEM_FLAG)
    {
        ++my_cpu.ip;

        int reg_index = REG_INDEX_NEXT

        int number_index = CUR_INT_NEXT

        cpu_type el = my_cpu.ram.memmory[reg_index + number_index];

        PRINTF_ELEMENT
    }

    else if ((CUR_FLAG).REG_FLAG)
    {
        ++my_cpu.ip;

        cpu_type el = my_cpu.registers[(int)CUR_BYTE];

        ++my_cpu.ip;

        PRINTF_ELEMENT
    }

    else if ((CUR_FLAG).MEM_FLAG)
    {
        ++my_cpu.ip;

        cpu_type el = my_cpu.ram.memmory[CUR_INT];

        PRINTF_ELEMENT

        my_cpu.ip += sizeof(int);
    }
    },
    {
    if ((CUR_FLAG).CONST_FLAG == 0 && (CUR_FLAG).REG_FLAG == 0 && (CUR_FLAG).MEM_FLAG == 0)
    {
        ++my_disassembler.ip;

        if (j == 1)
        {
            fprintf(dis_file, "out\n");
        }
    }

    else if ((CUR_FLAG).REG_FLAG && (CUR_FLAG).MEM_FLAG)
    {
        ++my_disassembler.ip;

        int cur_byte = CUR_BYTE;
        ++my_disassembler.ip;

        int number_index = CUR_INT_NEXT

        if (j == 1)
        {
            fprintf(dis_file, "out [%cx + %d]\n", cur_byte + 'a', number_index);
        }
    }

    else if ((CUR_FLAG).REG_FLAG)
    {
        ++my_disassembler.ip;

        if (j == 1)
        {
            fprintf(dis_file, "out %cx\n", CUR_BYTE + 'a');
        }

        ++my_disassembler.ip;
    }

    else if ((CUR_FLAG).MEM_FLAG)
    {
        ++my_disassembler.ip;

        if (j == 1)
        {
            fprintf(dis_file, "out [%d]\n", CUR_INT);
        }

        my_disassembler.ip += sizeof(int);
    }
})

DEF_CMD(LOG, CMD_LOG, 0,
{
    dump_log_file(&my_cpu, log_file);
    ++my_cpu.ip;
},
{
    if (j == 1)
    {
        fprintf(dis_file, "log\n");
    }

    ++my_disassembler.ip;
})

    DEF_CMD(HLT, CMD_HLT, 0,
{
    my_cpu.state = STOP_HLT;
},
{
    if (j == 1)
    {
        fprintf(dis_file, "hlt\n");
    }
    ++my_disassembler.ip;
})

DEF_CMD(JMP, CMD_JMP, 1,
{
    ++my_cpu.ip;
    my_cpu.ip = (size_t)CUR_INT;
},
{
    ++my_disassembler.ip;

    if (j == 0)
    {
        generate_push_label(&my_disassembler);
    }

    else
    {
        char operation[5] = "jmp ";
        fprintf_func_label_name(dis_file, operation, &my_disassembler, (size_t)CUR_INT);
    }

    my_disassembler.ip += sizeof(int);
})

DEF_CMD(JA, CMD_JA, 1,
{
    ++my_cpu.ip;

    GET_EL1_EL2

    if (el1 > el2)
    {
        my_cpu.ip = (size_t)CUR_INT;
    }

    else
    {
        my_cpu.ip += sizeof(int);
    }
},
{
    ++my_disassembler.ip;

    if (j == 0)
    {
        generate_push_label(&my_disassembler);
    }

    else
    {
        char operation[5] = "ja ";
        fprintf_func_label_name(dis_file, operation, &my_disassembler, (size_t)CUR_INT);
    }

    my_disassembler.ip += sizeof(int);
})

DEF_CMD(JAE, CMD_JAE, 1,
{
    ++my_cpu.ip;

    GET_EL1_EL2

    if (el1 >= el2)
    {
        my_cpu.ip = (size_t)*(cpu_type *)(void *)(my_cpu.code + my_cpu.ip);
    }

    else
    {
        my_cpu.ip += sizeof(int);
    }
},
{
    ++my_disassembler.ip;

    if (j == 0)
    {
        generate_push_label(&my_disassembler);
    }

    else
    {
        char operation[5] = "jae ";
        fprintf_func_label_name(dis_file, operation, &my_disassembler, (size_t)CUR_INT);
    }

    my_disassembler.ip += sizeof(int);
})

DEF_CMD(JB, CMD_JB, 1,
{
    ++my_cpu.ip;

    GET_EL1_EL2

    if (el1 < el2)
    {
        my_cpu.ip = (size_t)*(cpu_type *)(void *)(my_cpu.code +  my_cpu.ip);
    }

    else
    {
        my_cpu.ip += sizeof(int);
    }
},
{
    ++my_disassembler.ip;

    if (j == 0)
    {
        generate_push_label(&my_disassembler);
    }

    else
    {
        char operation[5] = "jb ";
        fprintf_func_label_name(dis_file, operation, &my_disassembler, (size_t)CUR_INT);
    }

    my_disassembler.ip += sizeof(int);
})

DEF_CMD(JBE, CMD_JBE, 1,
{
    ++my_cpu.ip;

    GET_EL1_EL2

    if (el1 <= el2)
    {
        my_cpu.ip = (size_t)*(cpu_type *)(my_cpu.code + my_cpu.ip);
    }

    else
    {
        my_cpu.ip += sizeof(int);
    }
},
{
    ++my_disassembler.ip;

    if (j == 0)
    {
        generate_push_label(&my_disassembler);
    }

    else
    {
        char operation[5] = "jbe ";
        fprintf_func_label_name(dis_file, operation, &my_disassembler, (size_t)CUR_INT);
    }

    my_disassembler.ip += sizeof(int);
})

DEF_CMD(JE, CMD_JE, 1,
{
    ++my_cpu.ip;

    GET_EL1_EL2

    if (el1 == el2)
    {
        my_cpu.ip = (size_t)*(cpu_type *)(my_cpu.code +  my_cpu.ip);
    }

    else
    {
        my_cpu.ip += sizeof(int);
    }
},
{
    ++my_disassembler.ip;

    if (j == 0)
    {
        generate_push_label(&my_disassembler);
    }

    else
    {
        char operation[5] = "je ";
        fprintf_func_label_name(dis_file, operation, &my_disassembler, (size_t)CUR_INT);
    }

    my_disassembler.ip += sizeof(int);
})

DEF_CMD(JNE, CMD_JNE, 1,
{
    ++my_cpu.ip;

    GET_EL1_EL2

    if (el1 != el2)
    {
        my_cpu.ip = (size_t)*(cpu_type *)(void *)(my_cpu.code + my_cpu.ip);
    }

    else
    {
        my_cpu.ip += sizeof(int);
    }
},
{
    ++my_disassembler.ip;

    if (j == 0)
    {
        generate_push_label(&my_disassembler);
    }

    else
    {
        char operation[5] = "jne ";
        fprintf_func_label_name(dis_file, operation, &my_disassembler, (size_t)CUR_INT);
    }

    my_disassembler.ip += sizeof(int);
})

DEF_CMD(CALL, CMD_CALL, 1,
{
    ++my_cpu.ip;
    int ip_jmp = (int)my_cpu.ip;

    my_cpu.ip += sizeof(int);
    cpu_type el = (cpu_type)my_cpu.ip;
    push_stack(&my_cpu.back_call, &(el));

    my_cpu.ip = (size_t)*(cpu_type *)(void *)(my_cpu.code + ip_jmp);
},
{
    ++my_disassembler.ip;

    if (j == 0)
    {
        generate_push_label(&my_disassembler);
    }

    else
    {
        char operation[5] = "call";
        fprintf_func_label_name(dis_file, operation, &my_disassembler, (size_t)CUR_INT);
    }

    my_disassembler.ip += sizeof(int);
})

DEF_CMD(NOP, CMD_NOP, 0,
{
},
{
})

DEF_CMD(DB, CMD_DB, 1,
{
    ++my_cpu.ip;

    struct str
    {
        char text [MAX_SIZE_LINE];
    };

    printf("%s\n", (*(str *)(my_cpu.code + my_cpu.ip)).text);
    my_cpu.ip += MAX_SIZE_LINE;
},
{
    ++my_disassembler.ip;

    struct str
    {
        char text [MAX_SIZE_LINE];
    };

    fprintf(dis_file, "db %s", (*(str *)(my_disassembler.code+ my_disassembler.ip)).text);
    my_disassembler.ip += MAX_SIZE_LINE;
})