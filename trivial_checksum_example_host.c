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

/* Size of the buffer for which we compute the checksum: 64KBytes. */
#define BUFFER_SIZE (1 << 4)

void populate_mram(XDPI interface) {
  uint8_t buffer[BUFFER_SIZE];

  for (int byte_index = 0; byte_index < BUFFER_SIZE; byte_index++) {
    buffer[byte_index] = (uint8_t)byte_index;
  }
  uint8_t *buffers[64];
  struct timeval begin, end;
  for (int i = 0; i < 64; i++)
  {
      buffers[i] = buffer;
  }
  gettimeofday(&begin, NULL);
  SendToPIM(interface, buffers, DPU_MRAM_HEAP_POINTER_NAME, 0, BUFFER_SIZE, 0);
  gettimeofday(&end, NULL);
  printf("SendToPIM: %d ns\n", (end.tv_sec * 1000000 + end.tv_usec) - (begin.tv_sec * 1000000 + begin.tv_usec));
}

int main() {
  struct dpu_set_t set, dpu;
  uint64_t checksum[64];
  uint32_t checksum32;
  XDPI interface;
  struct timeval begin, end;
  interface = newDirectPIMInterface();
  allocate(interface, 1, DPU_BINARY);
  populate_mram(interface);
  Launch(interface, 0);
  uint8_t *buffers[64];

  for (int i = 0; i < 64; i++)
  {
    buffers[i] = checksum+i;
  }
  gettimeofday(&begin, NULL);
  ReceiveFromPIM(interface, buffers, DPU_MRAM_HEAP_POINTER_NAME, BUFFER_SIZE, 8, 0);
  gettimeofday(&end, NULL);
  printf("ReceiveFromPIM: %d ns\n", (end.tv_sec * 1000000 + end.tv_usec) - (begin.tv_sec * 1000000 + begin.tv_usec));
  checksum32 = (uint32_t)(checksum[0]);
  printf("0x%08x\n",checksum32);
  deallocate(interface);
  return 0;
}
