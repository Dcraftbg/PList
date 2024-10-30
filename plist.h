// Licensed under MIT:  
// Copyright 2024 Dcraftbg 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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

