#include "plist.h"
#include <stddef.h>
#include <stdio.h>
#define REG_PAGES 20
char reg1[REG_PAGES*PAGE_SIZE] __attribute__((aligned(4096)));
char reg2[REG_PAGES*PAGE_SIZE] __attribute__((aligned(4096)));
char reg3[REG_PAGES*PAGE_SIZE] __attribute__((aligned(4096)));

typedef struct {
    void* address;
    size_t size;
} BootMemoryNode;
BootMemoryNode nodes[] = {
    { .address=reg1, .size=sizeof(reg1) },
    { .address=reg2, .size=sizeof(reg2) },
    { .address=reg3, .size=sizeof(reg3) },
};
#define nodes_count (sizeof(nodes)/sizeof(*nodes))

static size_t init_boot_memory(PList* alloc) {
    list_init(&alloc->list);
    size_t total_pages=0;
    for(size_t i = 0; i < nodes_count; ++i) {
        PListNode* node = (PListNode*)nodes[i].address;
        list_init(&node->list);
        total_pages+=nodes[i].size/PAGE_SIZE;
        node->pages = (nodes[i].size/PAGE_SIZE)-1;
        list_append(&node->list, &alloc->list);
    }
    return total_pages;
}
static void defrag_test_1() {
    PList alloc;
    init_boot_memory(&alloc);
    printf("Before defrag: %zu nodes\n", plist_node_count(&alloc));
    plist_defrag(&alloc);
    printf("After defrag: %zu nodes\n", plist_node_count(&alloc));
    void* addr;
    size_t pages;
    pages = 60;
    addr = plist_allocate_cont_pages(&alloc, pages);
    printf("Allocating %zu continious pages: %p\n", pages, addr);
}
static void defrag_test_2() {
    PList alloc;
    size_t total_pages = init_boot_memory(&alloc);
    for(size_t i = 0; i < total_pages; ++i) {
        void* addr = plist_allocate_page(&alloc);
        printf("%zu> %p\n", i, addr);
        plist_dealloc_page(&alloc, addr);
    }
    printf("Before defrag: %zu nodes\n", plist_node_count(&alloc));
    plist_defrag(&alloc);
    printf("After defrag: %zu nodes\n", plist_node_count(&alloc));
}
// #pragma GCC diagnostic pop
int main(void) {
    defrag_test_1();
    defrag_test_2();
}
