// this file will capture input and sotre it
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "main.h"
#include "data.h"
#include "budget.h"
#include "ordering.h"

// this will open a file and return a pointer to the file
static FILE *openFileToRead(char filename[])
{
    printf("Opening file\n");
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file\n");
        exit(1);
    }
    else
    {
        printf("Success, returning file %s\n", filename);
        return file;
    }
};

// this functions will open the file, by calling the helper function, read the input, parse it, and store it as a struct, it will finally return an array of structs
struct record storeData(char filename[])
{
    // first open the file;
    printf("in storeData attmepting to open file\n");
    FILE *file = openFileToRead(filename);
    if (file)
        printf("File is opened\n");
    // data points to an array of structs, notably the first item
    int cap = 16, count = 0;
    struct expenseEntry *data = malloc(cap * sizeof(struct expenseEntry));
    if (!data)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    char typeBuf[100 * sizeof(char)], subtypeBuf[100 * sizeof(char)], descBuf[100 * sizeof(char)];
    int id, y, m, d;
    double amt;
    while (fscanf(file,
                  "%d|%d-%d-%d|%99[^|]|%99[^|]|%99[^|]|%lf",
                  &id, &y, &m, &d,
                  typeBuf, subtypeBuf, descBuf,
                  &amt) == 8)
    {

        // if count reaches cap, we realloc to bigger arr
        if (count == cap)
        {
            // grow by 50%
            cap *= 1.5;
            data = realloc(data, cap * sizeof(struct expenseEntry));
            if (!data)
            {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
        }
        // Fill element #count:
        data[count].entryId = id;
        data[count].entryDate.day = d;
        data[count].entryDate.month = m;
        data[count].entryDate.year = y;
        data[count].entryAmount = amt;
        // strdup() each string so memory is properly owned:
        data[count].entryType = strdup(typeBuf);
        data[count].entrySubtype = strdup(subtypeBuf);
        data[count].entryDescription = strdup(descBuf);
        count++;
    }
    // finally after we captured all the input, reallocate the memory to fit the exact count, so we don't overextend and then finalize the return struct and return it
    data = realloc(data, count * sizeof(struct expenseEntry));
    if (!data)
    {
        perror("realloc");
        exit(EXIT_FAILURE);
    }
    struct record returnRecord = {data, count};
    return returnRecord;
};
