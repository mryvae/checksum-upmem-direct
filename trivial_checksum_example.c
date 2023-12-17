#include <mram.h>
#include <stdbool.h>
#include <stdint.h>

#define CACHE_SIZE 256
#define BUFFER_SIZE (1 << 4)

__host uint32_t checksum;
__mram_ptr uint8_t *buffer = (__mram_ptr uint8_t *)(DPU_MRAM_HEAP_POINTER);
__mram_ptr uint64_t *checksum64 = (__mram_ptr uint64_t *)(DPU_MRAM_HEAP_POINTER + BUFFER_SIZE);

int main() {
  __dma_aligned uint8_t local_cache[CACHE_SIZE];
  checksum = 0;

  for (unsigned int bytes_read = 0; bytes_read < BUFFER_SIZE;) {
    mram_read(&buffer[bytes_read], local_cache, CACHE_SIZE);

    for (unsigned int byte_index = 0; (byte_index < CACHE_SIZE) && (bytes_read < BUFFER_SIZE); byte_index++, bytes_read++) {
      checksum += (uint32_t)local_cache[byte_index];
    }
  }
  *checksum64 = checksum;
  return checksum;
}
