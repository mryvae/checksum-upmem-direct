/* Communication with a DPU via the MRAM. */
/* Populate the MRAM with a collection of bytes and request the DPU to */
/* compute the checksum. */

#include <dpu.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>
#include "upmemInterface/upmem_direct_c.h"
#ifndef DPU_BINARY
#define DPU_BINARY "trivial_checksum_example_dpu_binary"
#endif

#define NR_DPUS 128

/* Size of the buffer for which we compute the checksum: 64KBytes. */
#define BUFFER_SIZE (1 << 4)

void populate_mram(struct dpu_set_t set)
{
    uint8_t buffer[BUFFER_SIZE];

    for (int byte_index = 0; byte_index < BUFFER_SIZE; byte_index++)
    {
        buffer[byte_index] = (uint8_t)byte_index;
    }
    uint8_t *buffers[NR_DPUS];
    struct timeval begin, end;
    for (int i = 0; i < NR_DPUS; i++)
    {
        buffers[i] = buffer;
    }
    struct dpu_set_t dpu;
    uint32_t each_dpu;
    DPU_FOREACH(set, dpu, each_dpu)
    {
        DPU_ASSERT(dpu_prepare_xfer(dpu, buffer));
    }
    gettimeofday(&begin, NULL);
    DPU_ASSERT(dpu_push_xfer(set, DPU_XFER_TO_DPU, DPU_MRAM_HEAP_POINTER_NAME, 0,
                             BUFFER_SIZE, DPU_XFER_DEFAULT));
    gettimeofday(&end, NULL);
    printf("SendToPIM: %d us\n", (end.tv_sec * 1000000 + end.tv_usec) - (begin.tv_sec * 1000000 + begin.tv_usec));
}

int main()
{
    struct dpu_set_t set, dpu;
    uint64_t checksum[NR_DPUS];
    uint32_t checksum32;
    XDPI interface;
    struct timeval begin, end;
    DPU_ASSERT(dpu_alloc(NR_DPUS, NULL, &set));
    DPU_ASSERT(dpu_load(set, DPU_BINARY, NULL));
    populate_mram(set);
    DPU_ASSERT(dpu_launch(set, DPU_SYNCHRONOUS));
    uint8_t *buffers[NR_DPUS];

    gettimeofday(&begin, NULL);
    uint32_t each_dpu;
    DPU_FOREACH(set, dpu, each_dpu)
    {
        DPU_ASSERT(dpu_prepare_xfer(dpu, checksum + each_dpu));
    }
    DPU_ASSERT(dpu_push_xfer(set, DPU_XFER_FROM_DPU, DPU_MRAM_HEAP_POINTER_NAME, BUFFER_SIZE, 8, DPU_XFER_DEFAULT));
    gettimeofday(&end, NULL);
    printf("ReceiveFromPIM: %d us\n", (end.tv_sec * 1000000 + end.tv_usec) - (begin.tv_sec * 1000000 + begin.tv_usec));
    checksum32 = (uint32_t)(checksum[0]);
    printf("0x%08x\n", checksum32);
    return 0;
}
