#include "custom_vector.h"

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>


void allocStringVector(size_t size, StringVector* vec_)
{
    vec_->data = 0;
    vec_->size = 0;
    vec_->is_sortedAscending = false;
}

void appendStr(StringVector* vec, string str_)
{
    // Allocate (or reallocate) memory for the new string pointer
    string** newData = (string**)realloc(vec->data, (vec->size + 1) * sizeof(string));
    if (newData == NULL) {
        perror("Failed to allocate memory for new string");
        return -1;
    }
    vec->data = newData;
    
    // Allocate memory for the new string
    vec->data[vec->size] = malloc(sizeof(string));
    if (vec->data[vec->size] == NULL) 
    {
        perror("Failed to allocate memory for string data");
        return -1;
    }
    
    // Copy the string into the newly allocated memory
    strncpy(vec->data[vec->size]->data, str_.data, str_.size);
    vec->data[vec->size]->size = str_.size;

    vec->size++;
    
    return 0;
}

void eraseStr(StringVector* vec, string str_)
{
    // Find the string in the vector
    size_t foundIndex = -1;
    size_t i;
    for ( i = 0; i < vec->size; i++) 
    {
        if (strcmp(vec->data[i], str_.data) == 0) 
        {
            foundIndex = i;
            break;
        }
    }

    // If the string was found, remove it
    if (foundIndex != -1)
    {
        // Free the memory for the string
        free(vec->data[foundIndex]);

        size_t i = 0;
        // Shift remaining elements to fill the gap
        for (i = foundIndex; i < vec->size - 1; i++)
        {
            vec->data[i] = vec->data[i + 1];
        }

        // Update size and reallocate memory
        vec->size--;
        vec->data = (char**)realloc(vec->data, vec->size * sizeof(char*));
    }
}

void uninitStringVector(StringVector* vec_)
{
    int i;
    for (i = 0; i < vec_->size; i++)
    {
        free(vec_->data[i]);
    }

    vec_->size = 0;
}

void sortAscending(StringVector* const vector)
{
    int i;
    for (i = 0; i < vector->size - 1; i++)
    {
        int k = 0;
        for (k = i + 1; k < vector->size; k++)
        {
            int dif_letter_ind = 0;

            // while both beginnigs are equal or the strings didnt end yet
            while (vector->data[i]->data[dif_letter_ind] == vector->data[k]->data[dif_letter_ind] &&
                (dif_letter_ind <= vector->data[i]->size && dif_letter_ind <= vector->data[k]->size))
            {
                dif_letter_ind++;
            }

            if (dif_letter_ind >= vector->data[i]->size || dif_letter_ind >= vector->data[k]->size)
            {
                break;
            }

            if (vector->data[i]->data[dif_letter_ind] > vector->data[k]->data[dif_letter_ind])
            {
                string* temp = vector->data[k];
                vector->data[k] = vector->data[i];
                vector->data[i] = temp;
            }
        }
    }

    vector->is_sortedAscending = true;

}

bool isInSortedAscVector(const StringVector* const blacklist, const string* const name_)
{
    int low = 0;
    int high = blacklist->size - 1;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        //printf("%s\n", blacklist->data[mid]->data);
        int res = strncmp(blacklist->data[mid]->data, name_->data, name_->size);

        if (res == 0)
        {
            printf("The domain name %s was found among blacklist\n", name_->data);
            return true;
        }
        else if (res < 0) {
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }
    return false;
}