#include <iostream>
#include <cstdio>

#define M 2

typedef struct btree_nodes
{
    int k[2*M-1];
    struct btree_nodes *p[2*M];
    int num;
    bool is_leaf;
    struct btree_nodes *prev;
    struct btree_nodes *next;
} btree_node;