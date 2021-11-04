#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <stdint.h>

//for getting stack name
#define GET_NAME(var) #var

#define STACK_DEBUG

#ifdef STACK_DEBUG

const size_t STRUCT_CANARY = 0xDED32DED32DED339;

//for getting location of stack
#define location  __FILE__, __func__, __LINE__

//count of all states
const int COUNT_TYPES_STATES = 12;

//states for stack
enum States
{
    CONSTRUCTED           = 1,
    NEWBORN               = 1 << 2,
    DESTRUCTED            = 1 << 3,
    STRUCT_HASH_ERROR     = 1 << 4,
    STACK_HASH_ERROR      = 1 << 5,
    STRUCT_CANARY_ERROR   = 1 << 6,
    STACK_CANARY_ERROR    = 1 << 7,
    STRUCT_NULLPTR_ERROR  = 1 << 8,
    STACK_NULLPTR_ERROR   = 1 << 9,
    MEMORY_ERROR          = 1 << 10,
    COUNT_ELEMENT_ERROR   = 1 << 11,
    DEFAULT_ELEMENT_ERROR = 1 << 12,
};

//names of states stack
const char * const states_name[] = {
        "constructed",
        "no constructed yet",
        "destructed :(",
        "struct hash has changed",
        "stack hash has changed",
        "struct canary has changed",
        "stack canary has changed",
        "pointer to struct is nullptr",
        "pointer to stack is nullptr",
        "free memory is not enough",
        "stack is empty",
        "default elements are out of place"
};

// super assert for Stack
#define ASSERT_OK(obj, loc)                                                                             \
    if (stack_ok(obj) != 0)                                                                             \
    {                                                                                                   \
        dump(obj, loc);                                                                                 \
    }

//first string in dump file, change if you need
#define GET_STR_ERROR(error_name, error_file_name, error_func_name, line_num, stack_name)               \
                     "error: %s; error file: %s; error function: %s; error line: %d; stack name: %s\n", \
                      error_name, error_file_name, error_func_name,  line_num,       stack_name

#endif

//change to fill your free places in stack by this element
#define DEFAULT_ELEMENT 0

//change to your value for increasing stack to his size * 2
const int INCREASE_COEFFICIENT = 2;

//type of stack elements
typedef int stack_type;

//size of stack_canary
const size_t stack_canary_size = sizeof(unsigned long long);

//stack struct, you can initialization your stack as {}, but if you want to see his name in dumpfile, you should
//my_stack.name = GET_NAME(my_stack);
struct Stack
{
    #ifdef STACK_DEBUG
    unsigned long long first_struct_canary;

    unsigned long long stack_canary;
    #endif

    size_t size;
    size_t capacity;

    #ifdef STACK_DEBUG
    char *name;
    size_t struct_hash;
    size_t stack_hash;
    #endif

    char *data;

    #ifdef STACK_DEBUG
    unsigned long long states;
    unsigned long long second_struct_canary;
    #endif
};

/*!
 * function for constructing your stack, you should do it before using stack
 * if you do not do it, you will have a error "not constructed yet" and state NEWBORN
 * @param stack pointer to your stack
 * @return size of stack in char
 */
size_t stack_constructor(Stack *stack);

/*!
 * function for pushing in stack
 * @param stack pointer to your stack
 * @param el pointer to your element
 * @return new count of elements
 */
size_t push_stack(Stack *stack, const stack_type* el);

/*!
 * function for get top element
 * @param stack pointer to your stack
 * @return last element
 */
stack_type top_stack(Stack *stack);

/*!
 * function for remove last element
 * @param stack pointer to your stack
 * @return last element
 */
stack_type pop_stack(Stack *stack);

/*!
 * function for destructing your stack, after this, you won't be able to do anything with stack before initialization stack
 * @param stack pointer to your stack
 */
void stack_destructor(Stack *stack);

