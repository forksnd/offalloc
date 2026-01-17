// Offset allocator test
// https://github.com/aarni57/offalloc

#include "offalloc.h"

#include <stdio.h>
#include <stdlib.h>

static void do_something(uint32_t offset) {
    printf("do_something: offset %u\n", offset);
}

int main()
{
    oa_allocator_t allocator = { 0 };
    oa_create(&allocator, 65536, 1024);

    oa_allocation_t a;
    if (!oa_allocate(&allocator, 1337, &a))
        do_something(a.offset);

    oa_allocation_t b;
    if (!oa_allocate(&allocator, 123, &b))
        do_something(b.offset);

    oa_allocation_t c;
    if (!oa_allocate(&allocator, 574574, &c))
        do_something(c.offset);

    oa_storage_report_t r;
    oa_storage_report(&allocator, &r);

    printf("Total free space: %u, Largest free region: %u\n", r.total_free_space, r.largest_free_region);

    // Test scenario: space runs out
    oa_allocation_t d;
    printf("Attempting to allocate 60000 units (should fail initially)...\n");
    if (!oa_allocate(&allocator, 60000, &d)) {
        do_something(d.offset);
        printf("Allocation of 60000 units succeeded unexpectedly.\n");
    } else {
        printf("Allocation of 60000 units failed as expected (OA_NO_SPACE).\n");
    }

    // Free some space
    printf("Freeing allocation 'a' (1337 units)...\n");
    oa_free(&allocator, &a);
    oa_storage_report(&allocator, &r);
    printf("Total free space after freeing 'a': %u, Largest free region: %u\n", r.total_free_space, r.largest_free_region);

    // Attempt allocation again
    printf("Attempting to allocate 60000 units again (should succeed now)...\n");
    if (!oa_allocate(&allocator, 60000, &d)) {
        do_something(d.offset);
        printf("Allocation of 60000 units succeeded as expected.\n");
    } else {
        printf("Allocation of 60000 units failed unexpectedly.\n");
    }

    oa_free(&allocator, &d);
    oa_free(&allocator, &c);
    oa_free(&allocator, &b);

    oa_destroy(&allocator);

    return EXIT_SUCCESS;
}
