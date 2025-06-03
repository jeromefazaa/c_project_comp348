#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "main.h"
#include "data.h"
#include "budget.h"
#include "ordering.h"

int main(int argc, char *argv[])
{

    struct record records = storeData(argv[1]); // load all entries :contentReference[oaicite:0]{index=0}
    bool runProgram = true;
    system("clear");

    while (runProgram)
    {
        printf(
            "Budget Tracking System\n"
            "===============\n"
            "1. Display all entries\n"
            "2. Expense Distribution\n"
            "3. Sort Entries\n"
            "4. Add income/expense entry\n"
            "5. Modify entry\n"
            "6. Filter by month\n"
            "7. Exit\n"
            "Choice: ");

        int choice;
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            system("clear");
            displayAllEntries(records);
            break;

        case 2:
            system("clear");
            expenseDistribution(records);
            break;

        case 3:
            system("clear");
            sortMenu(records);
            break;

        case 4:
            system("clear");
            addEntry(&records);
            break;

        case 5:
            system("clear");
            modifyEntry(&records);
            break;

        case 6:
            system("clear");
            filterByMonth(&records);
            break;

        case 7:
            system("clear");
            freeRecords(&records);
            printf("Goodbye and thanks for using our budget tracker app\n");
            runProgram = false;
            break;

        default:
            // unrecognized choice: just loop back
            break;
        }
    }

    return 0;
}
