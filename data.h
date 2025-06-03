#ifndef data_H
#define data_H

struct Date
{
    int day;
    int month;
    int year;
};

struct expenseEntry
{
    int entryId;
    struct Date entryDate;
    char *entryType;
    char *entrySubtype;
    char *entryDescription;
    double entryAmount;
};

struct record
{
    struct expenseEntry *data;
    int record_size;
};

struct record storeData(char filename[]);


#endif