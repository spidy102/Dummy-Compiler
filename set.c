/*
Group - 29

Shaurya Marwah - 2019B3A70459P
Hari Sankar - 2019B3A70564P
Ruchir Kumbhare - 2019B5A70650P
Ashwin Murali - 2019B2A70957P
Dilip Venkatesh - 2020A7PS1203P

*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "set.h"

void add_in_set(ull *set, int num)
{
    
    ull shifted = (ull)1 << num;
    *set = *set | shifted; // 0 indexed set, if 1 at a particular bit, it indicates that element is present in the set
}

void remove_from_set(ull *set, int x)
{
    ull temp = (ull)1 << x;
    *set = *set ^ temp; // xor operation to make the bit at xth position 0
}

bool contains_in_set(ull *set, int x)
{
    
    ull temp = (ull)1 << x;
    if ((*set & temp) >0)
        return true;
    return false;
}

void print_set_elements(ull* set) {
    int i = 0;
    ull temp = *set;
    printf("set contains\n");
    while (temp!=0) {
        if (temp%2==1) {
            printf("%d\n", i);
        }
        temp/=2;
        i++;
    }
}

void union_two_sets(ull *res, ull *first, ull *second)
{
    *res = *first | *second;
}

void intersect_two_sets(ull *res, ull *first, ull *second)
{
    *res = *first & *second;
}
