#include "stack.h"
//file name for dump error
const char* DUMP_FILE_NAME = "C:\\Users\\levce\\CLionProjects\\Processor\\log\\log.txt";

#ifdef STACK_DEBUG

    int stack_ok(Stack *stack);
    void dump(Stack *stack, const char* error_file_name, const char* error_func_name, int line_num);
    size_t calculate_hash(const char *data, size_t size);
    size_t calculate_struct_hash(Stack *stack);
    size_t calculate_stack_hash(Stack *stack);
    void calculate_hashes(Stack * stack);
    bool check_default_elements(Stack *stack);

#endif

void fill_stack(Stack * stack);

size_t increase_stack(Stack *stack);

void fill_stack(Stack *stack)
{
    size_t offset = sizeof(stack->stack_canary) + sizeof(stack_type) * stack->size;
    for (; offset < sizeof(stack->stack_canary) + sizeof(stack_type) * stack->capacity; offset += sizeof(stack_type))
    {
        *(stack_type *)(stack->data + offset) = DEFAULT_ELEMENT;
    }
}

size_t stack_constructor(Stack *stack)
{
    if (stack == nullptr)
    {
        #ifdef STACK_DEBUG
        dump(stack, location);
        #endif

        assert(0);
    }

    stack->size = 0;

    stack->data = (char *)realloc(stack->data,stack_canary_size +
                                  stack->capacity * sizeof(stack_type) + stack_canary_size);
    if (stack->data == nullptr)
    {
        #ifdef STACK_DEBUG
        stack->states = MEMORY_ERROR;
        dump(stack, location);
        #endif
        assert(0);
    }

    #ifdef STACK_DEBUG
    stack->stack_canary = (unsigned long long)stack->data;
    stack->first_struct_canary  = STRUCT_CANARY;
    stack->second_struct_canary = STRUCT_CANARY;

    *(unsigned long long *)(stack->data)                                                                      = stack->stack_canary;
    *(unsigned long long *)(stack->data + sizeof(stack->stack_canary) + stack->capacity * sizeof(stack_type)) = stack->stack_canary;
    #endif

    fill_stack(stack);

    #ifdef STACK_DEBUG
    calculate_hashes(stack);

    stack->states = CONSTRUCTED;

    ASSERT_OK(stack, location);
    #endif

    return stack_canary_size + stack->capacity * sizeof(stack_type) + stack_canary_size;
}

size_t push_stack(Stack *stack, const stack_type* el)
{
    assert(stack!=nullptr);

    #ifdef STACK_DEBUG
    ASSERT_OK(stack, location);
    #endif

    if (stack->size >= stack->capacity)
    {
        increase_stack(stack);
    }

    *(stack_type *)(stack->data + stack_canary_size + stack->size * sizeof(stack_type)) = *el;

    ++stack->size;

    #ifdef STACK_DEBUG
    calculate_hashes(stack);

    ASSERT_OK(stack, location);
    #endif

    return stack->size;
}

size_t increase_stack(Stack *stack)
{
    #ifdef STACK_DEBUG
    ASSERT_OK(stack, location);
    #endif
    if (stack->capacity == 0)
    {
        stack->capacity = 1;
    }

    size_t new_capacity = stack->capacity * INCREASE_COEFFICIENT;

    char *pointer = (char *) realloc(stack->data, stack_canary_size + new_capacity * sizeof(stack_type) + stack_canary_size);

    if (pointer == nullptr)
    {
        #ifdef STACK_DEBUG
        stack->states = CONSTRUCTED | MEMORY_ERROR;
        dump(stack, location);
        #endif

        assert(0);
    }

    stack->data = pointer;
    stack->capacity = new_capacity;

    *(stack_type *)(stack->data + stack_canary_size + stack->capacity * sizeof(stack_type)) = DEFAULT_ELEMENT;

    #ifdef STACK_DEBUG
    stack->stack_canary = *(unsigned long long *)pointer;

    *(unsigned long long *)(stack->data + sizeof(stack->stack_canary) + new_capacity * sizeof(stack_type)) = stack->stack_canary;
    #endif

    fill_stack(stack);

    #ifdef STACK_DEBUG
    calculate_hashes(stack);

    ASSERT_OK(stack, location);
    #endif

    return new_capacity;
}

stack_type top_stack(Stack *stack)
{
    assert(stack!=nullptr);

    #ifdef STACK_DEBUG
    ASSERT_OK(stack, location);
    #endif

    if (stack->size > 0)
    {
        size_t offset_in_bytes = stack_canary_size + (stack->size - 1) * sizeof(stack_type);

        stack_type last_element = *(stack_type *)(stack->data + offset_in_bytes);

        #ifdef STACK_DEBUG
        calculate_hashes(stack);

        ASSERT_OK(stack, location);
        #endif

        return last_element;
    }
    else
    {
        #ifdef STACK_DEBUG
        stack->states = CONSTRUCTED | COUNT_ELEMENT_ERROR;

        dump(stack, location);
        #endif

        return DEFAULT_ELEMENT;
    }
}

stack_type pop_stack(Stack *stack)
{
    assert(stack!=nullptr);

    #ifdef STACK_DEBUG
    ASSERT_OK(stack, location);
    #endif

    if (stack->size <= 0)
    {
        #ifdef STACK_DEBUG
        stack->states = CONSTRUCTED | COUNT_ELEMENT_ERROR;

        dump(stack, location);
        #endif

        return DEFAULT_ELEMENT;
    }

    else
    {
        size_t offset_in_bytes = stack_canary_size + (stack->size - 1) * sizeof(stack_type);
        stack_type last_element = *(stack_type*)(stack->data + offset_in_bytes);

        *(stack_type *)(stack->data + offset_in_bytes) = DEFAULT_ELEMENT;

        --stack->size;

        #ifdef STACK_DEBUG
        calculate_hashes(stack);

        ASSERT_OK(stack, location);
        #endif

        return last_element;
    }
}


void stack_destructor(Stack *stack)
{
    assert(stack!=nullptr);

    #ifdef STACK_DEBUG
    ASSERT_OK(stack, location);
    #endif

    free(stack->data);

    stack->size = 0;
    stack->capacity = 0;

    #ifdef STACK_DEBUG
    stack->struct_hash = 0;
    stack->stack_hash = 0;
    stack->states = DESTRUCTED;
    #endif

    stack->data = nullptr;
}

#ifdef STACK_DEBUG
void dump(Stack *stack, const char* error_file_name, const char* error_func_name, int line_num)
{
    FILE *file = fopen(DUMP_FILE_NAME, "a");
    unsigned long long state = 0;

    if (stack != nullptr)
    {
        state = stack->states;
    }
    else
    {
        state = STACK_NULLPTR_ERROR;
    }

    int index = 1;
    for (size_t i = 4; i < (1 << COUNT_TYPES_STATES); i <<= 1)
    {
        if ((state & i) != 0)
        {
            fprintf(file, GET_STR_ERROR(states_name[index], error_file_name, error_func_name, line_num, stack->name));
        }

        ++index;
    }
    if (stack != nullptr && stack->data != nullptr)
    {
        fprintf(file, "Information about stack struct\n\n");
                                                                // long %X
        fprintf(file, "excepted first struct canary:  %15lX;      real first struck canary:  %15llX\n", STRUCT_CANARY, stack->first_struct_canary);
        fprintf(file, "excepted second struct canary: %15lX;      real second struck canary: %15llX\n", STRUCT_CANARY, stack->second_struct_canary);
        fprintf(file, "excepted struct hash:          %15lX;       real struct hash:         %15lX\n\n\n", stack->struct_hash, calculate_struct_hash(stack));

        fprintf(file, "Information about stack\n\n");

        fprintf(file, "excepted first stack canary:   %Illu;           real first stack canary:  %Illu\n", stack->stack_canary, *(unsigned long long *)stack->data);
        fprintf(file, "excepted second stack canary:  %Illu;           real second stack canary: %Illu\n", stack->stack_canary,
                *(unsigned long long *)(stack->data + stack_canary_size + sizeof(stack_type) * stack->capacity));
        fprintf(file, "excepted stack hash          %Ilu;           real stack hash:        %Ilu\n", stack->stack_hash, calculate_stack_hash(stack));

        fprintf(file, "stack size:    %Ilu\n", stack->size);
        fprintf(file, "capacity size: %Ilu\n", stack->capacity);
        fprintf(file, "stack states   %Illu\n", stack->states);
        fprintf(file, "Stack elements:\n");

        for (size_t i = 0; i < stack->size; ++i)
        {
            fprintf(file, "%Ilu element: %d\n", i, *(stack_type *)(stack->data + stack_canary_size + i * sizeof(stack_type)));
        }

        for (size_t i = stack->size; i < stack->capacity; ++i)
        {
            fprintf(file, "[%Ilu] element: %d\n", i, *(stack_type *)(stack->data + stack_canary_size + i * sizeof(stack_type)));
        }
        fprintf(file, "-------------------------------------------------------------------------------\n\n\n\n");
    }

    //fclose(file);
    assert(0);
}

int stack_ok(Stack *stack)
{
    int count_errors = 0;

    if (stack == nullptr)
    {
        return STRUCT_NULLPTR_ERROR;
    }

    if ((stack->states & DESTRUCTED) != 0)
    {
        return DESTRUCTED;
    }

    if (!(stack->states & CONSTRUCTED))
    {
        stack->states = NEWBORN;
        return NEWBORN;
    }

    if (stack->data == nullptr)
    {
        stack->states |= STACK_NULLPTR_ERROR;
        return STACK_NULLPTR_ERROR;
    }

    size_t offset_in_bytes = sizeof(stack->stack_canary) + stack->capacity * sizeof(stack_type);

    if (!check_default_elements(stack))
    {
        stack->states |= DEFAULT_ELEMENT_ERROR;
        ++count_errors;
    }

    if ((stack->stack_canary != *(unsigned long long *)(stack->data)) ||
        (stack->stack_canary != *(unsigned long long *)(stack->data + offset_in_bytes)))
    {
        stack->states |= STACK_CANARY_ERROR;
        ++count_errors;
    }

    if (stack->first_struct_canary != STRUCT_CANARY || stack->second_struct_canary != STRUCT_CANARY)
    {
        stack->states |= STRUCT_CANARY_ERROR;
        ++count_errors;
    }

    if (stack->stack_hash != calculate_stack_hash(stack))
    {
        stack->states |= STACK_HASH_ERROR;
        ++count_errors;
    }

    if (stack->struct_hash != calculate_struct_hash(stack))
    {
        stack->states |= STRUCT_HASH_ERROR;
        ++count_errors;
    }

    if (count_errors == 0)
    {
        stack->states = CONSTRUCTED;
    }

    return count_errors;
}

size_t calculate_hash(const char *data, size_t size)
{
    size_t hash = 0;
    for (size_t i = 0; i < size; ++i)
    {
        hash += (size_t)(data[i]) * (1uL << i);
    }

    return hash;
}

size_t calculate_struct_hash(Stack *stack)
{
    size_t old_hash = stack->struct_hash;
    size_t states = stack->states;

    stack->struct_hash = 0;
    stack->states = 0;

    size_t new_hash = calculate_hash((char *)stack, sizeof(*stack));

    stack->struct_hash = old_hash;
    stack->states = states;
    return new_hash;
}

size_t calculate_stack_hash(Stack *stack)
{
    size_t stack_hash = calculate_hash(stack->data, sizeof(stack->data));

    return stack_hash;
}

void calculate_hashes(Stack *stack)
{
    stack->stack_hash = calculate_stack_hash(stack);
    stack->struct_hash = calculate_struct_hash(stack);
}

bool check_default_elements(Stack *stack){
    size_t offset = sizeof(stack->stack_canary) + sizeof(stack_type) * stack->size;
    for (; offset < sizeof(stack->stack_canary) + sizeof(stack_type) * stack->capacity; offset += sizeof(stack_type))
    {
        if (*(stack_type *)(stack->data + offset) != DEFAULT_ELEMENT)
        {
            return false;
        }
    }
    return true;
}
#endif