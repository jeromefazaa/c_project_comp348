#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include "main.h"
#include "data.h"
#include "budget.h"
#include "ordering.h"

void displayAllEntries(struct record records)
{
    struct expenseEntry *data = records.data;
    printf(
        "Finances Summary\n"
        "================\n\n"
        "ID\t\tDate\t\t\tType\t\t\tCategory\t\tDescription\t\tAmount\n"
        "-------------------------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < records.record_size; i++)
    {
        printf("%-5d\t\t%-4d-%-1d-%-1d\t\t%-10s\t\t%-10s\t\t%-10s\t\t%-8.2lf\n", data[i].entryId, data[i].entryDate.year, data[i].entryDate.month, data[i].entryDate.day,
               data[i].entryType, data[i].entrySubtype, data[i].entryDescription, data[i].entryAmount);
    };
};

void expenseDistribution(struct record records)
{
    struct expenseEntry *data = records.data;
    int totalExpenses = 0;
    int totalIncome = 0;
    int totalNeeds = 0;
    int totalWants = 0;
    for (int i = 0; i < records.record_size; i++)
    {
        if (compareStrings((data[i].entryType), "income"))
        {
            totalIncome += data[i].entryAmount;
        };
        if (compareStrings((data[i].entryType), "expense"))
        {
            totalExpenses += data[i].entryAmount;
            if (compareStrings((data[i].entrySubtype), "wants"))
                totalWants += data[i].entryAmount;
            else if (compareStrings((data[i].entrySubtype), "needs"))
                totalNeeds += data[i].entryAmount;
        };
    }
    double needsPercentageIncome = totalNeeds / (double)totalIncome * 100;
    double needsPercentageExpenses = totalNeeds / (double)totalExpenses * 100;
    double wantsPercentageIncome = totalWants / (double)totalIncome * 100;
    double wantsPercentageExpenses = totalWants / (double)totalExpenses * 100;
    int netBalance = totalIncome - totalExpenses;

    printf("======Expense Distribution Report======\n");
    printf("Total Income: $%d\nTotal Expenses: $%d\nNeeds: $%d (%0.2lf%% of expenses, %0.2lf%% of income)\n"
           "Wants: $%d (%0.2lf%% of expenses, %0.2lf%% of income)\nNet Balance: $%d\n\n",
           totalIncome, totalExpenses, totalNeeds, needsPercentageExpenses, needsPercentageIncome,
           totalWants, wantsPercentageExpenses, wantsPercentageIncome, netBalance);
};

bool compareStrings(char *str1, char *str2)
{
    if (strlen(str1) != strlen(str2))
        return false;
    for (int i = 0; i < strlen(str1); i++)
    {
        if (tolower(str1[i]) != tolower(str2[i]))
            return false;
    }
    return true;
}

void addEntry(struct record *records)
{
    struct Date entryDate;
    char choice;
    char typeBuf[100], subtypeBuf[100], descBuf[100];
    double entryAmount;

    // 1) Date
    printf("\nUse today's date? (y/n): ");
    scanf(" %c", &choice);

    if (choice == 'y' || choice == 'Y')
    {
        time_t now = time(NULL);
        struct tm *lt = localtime(&now);
        entryDate.year = lt->tm_year + 1900;
        entryDate.month = lt->tm_mon + 1;
        entryDate.day = lt->tm_mday;
    }
    else
    {
        printf("Enter date (YYYY-MM-DD): ");
        scanf("%d-%d-%d",
              &entryDate.year,
              &entryDate.month,
              &entryDate.day);
    }

    // 2) Type & Category
    printf("Type (income/expense): ");
    scanf(" %99s", typeBuf);
    printf("Category: ");
    scanf(" %99s", subtypeBuf);

    // 3) Description (may contain spaces)
    printf("Description: ");
    getchar(); // consume leftover newline
    fgets(descBuf, sizeof(descBuf), stdin);
    descBuf[strcspn(descBuf, "\n")] = 0; // strip newline

    // 4) Amount
    printf("Amount: $");
    scanf("%lf", &entryAmount);

    // 5) Grow the array by one
    int newSize = records->record_size + 1;
    records->data = realloc(records->data,
                            newSize * sizeof *records->data);
    if (!records->data)
    {
        perror("realloc");
        exit(EXIT_FAILURE);
    }

    // 6) Compute next ID (max existing + 1)
    int maxId = 0;
    for (int i = 0; i < records->record_size; i++)
    {
        if (records->data[i].entryId > maxId)
            maxId = records->data[i].entryId;
    }
    int newId = maxId + 1;

    // 7) Populate the new entry
    struct expenseEntry *e = &records->data[records->record_size];
    e->entryId = newId;
    e->entryDate = entryDate;
    e->entryAmount = entryAmount;
    e->entryType = strdup(typeBuf);
    e->entrySubtype = strdup(subtypeBuf);
    e->entryDescription = strdup(descBuf);

    // 8) Update the count and confirm
    records->record_size = newSize;
}

void modifyEntry(struct record *records)
{
    // 0) Show all entries so the user can see IDs
    displayAllEntries(*records);

    // 1) Prompt for the ID to modify
    int id;
    printf("Enter ID of entry to modify: ");
    scanf("%d", &id);

    // 2) Find the entry
    struct expenseEntry *e = NULL;
    for (int i = 0; i < records->record_size; i++)
    {
        if (records->data[i].entryId == id)
        {
            e = &records->data[i];
            break;
        }
    }
    if (!e)
    {
        printf("Entry ID %d not found.\n", id);
        return;
    }

    // 3) Show current details
    printf("\nCurrent Details:\n"
           "ID: %d\n"
           "Date: %04d-%02d-%02d\n"
           "Type: %s\n"
           "Category: %s\n"
           "Description: %s\n"
           "Amount: $%.2lf\n\n",
           e->entryId,
           e->entryDate.year, e->entryDate.month, e->entryDate.day,
           e->entryType,
           e->entrySubtype,
           e->entryDescription,
           e->entryAmount);

    // 4) Choose field to modify
    printf("What would you like to modify?\n"
           "1. Date\n"
           "2. Amount\n"
           "Choice: ");
    int choice;
    scanf("%d", &choice);

    if (choice == 1)
    {
        printf("Enter new date (YYYY-MM-DD): ");
        scanf("%d-%d-%d",
              &e->entryDate.year,
              &e->entryDate.month,
              &e->entryDate.day);
    }
    else if (choice == 2)
    {
        double newAmt;
        printf("Enter new amount: $");
        scanf("%lf", &newAmt);
        if (newAmt < 0)
        {
            printf("Amount cannot be negative.\n");
            return;
        }
        e->entryAmount = newAmt;
    }
    else
    {
        printf("Invalid choice.\n");
        return;
    }

    printf("Entry updated successfully.\n");
}

void filterByMonth(const struct record *records)
{
    int year, month;
    printf("Enter year (YYYY): ");
    scanf("%d", &year);
    printf("Enter month (1-12): ");
    scanf("%d", &month);

    printf("\nEntries for %04d-%02d:\n", year, month);
   
    int count = 0;
    struct record copy = duplicateRecord(records);
    int j =0;

    for (int i = 0; i < records->record_size; i++)
    {
        struct expenseEntry *e = &records->data[i];

        if (e->entryDate.year == year && e->entryDate.month == month)
        {
            count++;
            copy.data[j] = *e;
            j++;
        }
    }
    copy.record_size = count;
    struct expenseEntry *pCopyData = copy.data;
    pCopyData = realloc(pCopyData,copy.record_size*sizeof(struct expenseEntry));
    displayAllEntries(copy);
}

// Utility: free all heap memory to avoid leaks
void freeRecords(struct record *records)
{
    for (int i = 0; i < records->record_size; i++)
    {
        free(records->data[i].entryType);
        free(records->data[i].entrySubtype);
        free(records->data[i].entryDescription);
    }
    free(records->data);
}
