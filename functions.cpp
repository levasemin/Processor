#include "processor.h"

void push_back_label(labels *all_labels, label lab)
{
    assert(all_labels != nullptr);

    if (all_labels->labels_capacity <= all_labels->count)
    {
        if (all_labels->labels_capacity == 0)
        {
            all_labels->labels_capacity += DEFAULT_CAPACITY_LABELS;
        }

        all_labels->labels = (label *)realloc(all_labels->labels, all_labels->labels_capacity * 2 * sizeof(lab));

        assert(all_labels->labels != nullptr);

        all_labels->labels_capacity *= 2;
    }

    *(all_labels->labels + all_labels->count) = lab;

    ++all_labels->count;
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