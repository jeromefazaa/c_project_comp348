#ifndef budget_H
#define budget_H

void displayAllEntries(struct record records);
void expenseDistribution(struct record records);
bool compareStrings(char *str1, char *str2);
void addEntry(struct record *records);
void modifyEntry(struct record *records);
void filterByMonth(const struct record *records);
void freeRecords(struct record *records);




#endif