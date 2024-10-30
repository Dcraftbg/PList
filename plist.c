// Licensed under MIT:  
// Copyright 2024 Dcraftbg 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#include "plist.h"

void* plist_allocate_page(PList* list) {
    if(list_empty(&list->list)) return NULL;
    PListNode* node = (PListNode*)list->list.next;
    void* result = (void*)node;
    if(node->pages) {
        PListNode* new_node = (PListNode*)(((char*)node)+PAGE_SIZE);
        new_node->pages = node->pages-1;
        list_init(&new_node->list);
        list_append(&new_node->list, &node->list);
    }
    list_remove(&node->list);
    return result;
}
void plist_dealloc_page(PList* alloc, void* page) {
    plist_dealloc_cont_pages(alloc, page, 1);
}

void* plist_allocate_cont_pages(PList* alloc, size_t pages) {
    if(pages == 0) return NULL;
    if(pages == 1) return plist_allocate_page(alloc);
    pages--;
    for(struct list* list = alloc->list.next; list != &alloc->list; list=list->next) {
        PListNode* node = (PListNode*)list;
        void* result = node;
        if(node->pages > pages) {
            PListNode* new_node = (PListNode*)(((char*)node)+PAGE_SIZE*pages);
            new_node->pages = node->pages-pages;
            list_init(&new_node->list);
            list_append(&new_node->list, list);
            list_remove(list);
            return result;
        } else if (node->pages == pages) {
            list_remove(list);
            return result;
        }
    }
    return NULL;
}
void plist_dealloc_cont_pages(PList* alloc, void* page, size_t count) {
    if(count == 0) return;
    PListNode* node = (PListNode*)page;
    list_init(&node->list);
    node->pages = count-1;
    list_append(&node->list, alloc->list);
}

void plist_defrag(PList* alloc) {
    list_foreach(defrag_list, &alloc->list) {
        PListNode* defrag = (PListNode*)defrag_list;
        struct list* check_list = alloc->list.next;
        while(check_list != &alloc->list) {
            PListNode* check = (PListNode*)check_list;
            struct list* next = check_list->next;
            if((PListNode*)plist_node_data_end(defrag)==check) {
                list_remove(&check->list);
                defrag->pages += check->pages+1;
            }
            check_list = next;
        }
    }
}
