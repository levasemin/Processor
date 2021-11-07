#include "processor.h"

size_t get_file_size(FILE *file)
{
    assert(file != nullptr);

    fseek(file, 0L, SEEK_END);

    size_t file_size = (size_t)ftell(file);

    fseek(file, 0L, SEEK_SET);

    return file_size;
}

size_t read_file(FILE *file, char **string)
{
    assert(file != nullptr);

    size_t file_size = get_file_size(file);

    *string = (char *)calloc((file_size + 1), sizeof(char));
    assert(*string != nullptr);

    size_t count_elements = fread(*string, sizeof(char), file_size, file);

    (*string)[count_elements] = '\n';

    return file_size;
}

strings * get_strings_data(char *string, size_t count_string)
{
    assert(string != nullptr);
    assert(count_string > 0);

    strings *data = (strings *) calloc(count_string, sizeof(strings));

    assert(data!= nullptr);

    size_t i = 0;

    for (i = 0; i < count_string; ++i)
    {
        data[i].start = string;

        data[i].end = strchr(string, '\0');

        string = data[i].end + 1;
    }

    data[i-1].end = string;
    return data;
}

size_t get_count_string(char *string)
{
    assert(string != nullptr);

    size_t count_string = 0;

    while ((string = strchr(string, '\n')) != nullptr)
    {
        *string = '\0';
        ++string;
        ++count_string;
    }

    return count_string;
}