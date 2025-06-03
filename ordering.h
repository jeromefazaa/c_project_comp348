#ifndef ordering_H
#define ordering_H

void sortMenu(struct record records);
int compareByDate(const void *x_void, const void *y_void);
int compareByAmount(const void *x_void, const void *y_void);
int compareByDescription(const void *x_void, const void *y_void);
int compareById(const void *x_void, const void *y_void);
struct record duplicateRecord(const struct record *orig);




#endif