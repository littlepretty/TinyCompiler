//
//  symtab.c
//  TinyCompiler
//
//  Created by Littlepretty Yan on 2/7/14.
//  Copyright (c) 2014 littlepretty. All rights reserved.
//

#include <stdio.h>
#include "symtab.h"
#include <string.h>
#include <stdlib.h>


#define SIZE 211    //!< size of hash table
#define SHIFT 4

static int hash(char *key)
{
    int temp = 0;
    int i = 0;
    while (key[i] != '\0') {
        temp = ((temp<<SHIFT) + key[i]) % SIZE;
        ++i;
    }
    return temp;
}

typedef struct LineListNode{
    int line_no;
    struct LineListNode* next;
    
} *LineListNode;

typedef struct BucketListNode{
    char *name;
    int mem_loc;
    
    LineListNode lines;
    
    struct BucketListNode* next;
} *BucketListNode;

static BucketListNode hashTable[SIZE];


void st_insert(char *name, int lineno, int loc)
{
    if (name) {
        int hashValue = hash(name);
        BucketListNode bucketListNode = hashTable[hashValue]; //!< point to bucket's header
        
        //!< find the right position in bucket list or null(need to alloc later)
        while (bucketListNode != NULL && strcmp(name, bucketListNode->name) != 0) {
            bucketListNode = bucketListNode->next;
        }
        
        if (bucketListNode == NULL) { //!< bucket or symbol not exist: insert a node as header of bucket
            bucketListNode = malloc(sizeof(struct BucketListNode));
            if (bucketListNode == NULL) {
                fprintf(stderr, "Out of Memory Error When Allocate BucketList");
            }
            bucketListNode->name = name;
            bucketListNode->mem_loc = loc;
            bucketListNode->lines = malloc(sizeof(struct LineListNode));
            bucketListNode->lines->line_no = lineno;
            
            bucketListNode->lines->next = NULL;
            bucketListNode->next = hashTable[hashValue];
            hashTable[hashValue] = bucketListNode;
            
        } else {    //!< symbol already in bucket list, insert a lineno node
            LineListNode lineNode = bucketListNode->lines;
            while (lineNode->next != NULL) {
                lineNode = lineNode->next;
            }
            lineNode->next = malloc(sizeof(struct LineListNode));
            lineNode->next->line_no = lineno;
            lineNode->next->next = NULL;
        }
        
    }
    
}

int st_lookup(char *name)
{
    int hashValue = hash(name);
    BucketListNode node = hashTable[hashValue];
    while (node != NULL && strcmp(name, node->name) != 0) {
        node = node->next;
    }
    if (node == NULL) {
        return -1;
    }else {
        return node->mem_loc;
    }
}

void printSymTab(FILE *listing)
{
    fprintf(listing, "Variable Name     Location     Line Numbers\n");
    fprintf(listing, "-------------     --------     ------------\n");
    
    for (int i =0 ; i < SIZE; i++) {
        
        if (hashTable[i] != NULL) {
            
            BucketListNode node = hashTable[i];
            while (node != NULL) {
                
                fprintf(listing, "%-14s     ", node->name);
                fprintf(listing, "%-8d     ", node->mem_loc);
                LineListNode lineNode = node->lines;
                while (lineNode != NULL) {
                    fprintf(listing, "%4d", lineNode->line_no);
                    lineNode = lineNode->next;
                }
                fprintf(listing, "\n");
                node = node->next;
            }
            
        }
        
    }
    
}