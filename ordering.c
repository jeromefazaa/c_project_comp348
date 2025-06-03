// ordering.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "data.h"
#include "ordering.h"
#include "main.h"
#include "budget.h"

struct record duplicateRecord(const struct record *orig);

void sortMenu(struct record records)
{
    int choice;
    printf("Sort Menu\n"
           "1. Sort by ID\n"
           "2. Sort by Date\n"
           "3. Sort by Amount\n"
           "4. Sort by Description\n"
           "Choice: ");
    scanf("%d", &choice);
    struct record copy = duplicateRecord(&records);

    switch (choice)
    {
    case 1:
        qsort(copy.data, copy.record_size, sizeof(struct expenseEntry), compareById);
        break;
    case 2:
        qsort(copy.data, copy.record_size, sizeof(struct expenseEntry), compareByDate);
        break;
    case 3:
        qsort(copy.data, copy.record_size, sizeof(struct expenseEntry), compareByAmount);
        break;
    case 4:
        qsort(copy.data, copy.record_size, sizeof(struct expenseEntry), compareByDescription);
        break;
    default:
        printf("Invalid choice\n");
        return;
    }
    // printf("calling Displaying all entries")
    displayAllEntries(copy);
}

int compareByDate(const void *x_void, const void *y_void)
{
    struct expenseEntry *x = (struct expenseEntry *)x_void;
    struct expenseEntry *y = (struct expenseEntry *)y_void;
    if (x->entryDate.year == y->entryDate.year && x->entryDate.month == y->entryDate.month)
        return x->entryDate.day - y->entryDate.day;
    else if (x->entryDate.year == y->entryDate.year)
        return x->entryDate.month - y->entryDate.month;
    else
        return x->entryDate.year - x->entryDate.year;
}

int compareByAmount(const void *x_void, const void *y_void)
{
    struct expenseEntry *x = (struct expenseEntry *)x_void;
    struct expenseEntry *y = (struct expenseEntry *)y_void;
    return x->entryAmount - y->entryAmount;
}

int compareByDescription(const void *x_void, const void *y_void)
{
    struct expenseEntry *x = (struct expenseEntry *)x_void;
    struct expenseEntry *y = (struct expenseEntry *)y_void;
    return strcmp(x->entryDescription, y->entryDescription);
}

int compareById(const void *x_void, const void *y_void)
{
    struct expenseEntry *x = (struct expenseEntry *)x_void;
    struct expenseEntry *y = (struct expenseEntry *)y_void;
    return x->entryId - y->entryId;
}

struct record duplicateRecord(const struct record *orig)
{
    struct record copy;
    copy.record_size = orig->record_size;
    // allocate a new array of expenseEntry
    copy.data = malloc(copy.record_size * sizeof *copy.data);
    if (!copy.data)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    // copy all entries
    memcpy(copy.data,
           orig->data,
           copy.record_size * sizeof *copy.data);
    return copy;
}
