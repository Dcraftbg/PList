#pragma once
#ifndef PAGE_SIZE
#define PAGE_SIZE 4096
#endif
#include <stddef.h>
#include "list.h"
typedef struct {
    struct list list;
    size_t pages; // 0 means just this page
} PListNode;
static void* plist_node_data_end(PListNode* node) {
    return ((char*)node)+((node->pages+1)*PAGE_SIZE);
}
static void* plist_node_data_start(PListNode* node) {
    return (char*)node;
}

typedef struct {
    struct list list;
} PList;

void* plist_allocate_page(PList* list);
void  plist_dealloc_page(PList* alloc, void* page);

void* plist_allocate_cont_pages(PList* alloc, size_t pages);
void  plist_dealloc_cont_pages(PList* alloc, void* page, size_t count);

void plist_defrag(PList* alloc);

static size_t plist_node_count(PList* list) {
    return list_len(&list->list);
}

