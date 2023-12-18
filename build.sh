dpu-upmem-dpurte-clang trivial_checksum_example.c -o trivial_checksum_example_dpu_binary
gcc -O3 --std=c99 -o trivial_checksum_example_host_direct_app -I. ./upmemInterface/upmem_direct_c_direct_opt.o  trivial_checksum_example_host.c -g `dpu-pkg-config --cflags --libs dpu` -lstdc++ -lpthread
gcc -O3 --std=c99 -o trivial_checksum_example_host_upmem_app -I. ./upmemInterface/upmem_direct_c_upmem.o  trivial_checksum_example_host.c -g `dpu-pkg-config --cflags --libs dpu` -lstdc++ -lpthread

dpu-upmem-dpurte-clang trivial_checksum_example1.c -o trivial_checksum_example_dpu_v1_binary
gcc -O3 --std=c99 -o trivial_checksum_example_host_direct_v1_app -I. ./upmemInterface/upmem_direct_c_direct_opt.o  trivial_checksum_example_host1.c -g `dpu-pkg-config --cflags --libs dpu` -lstdc++ -lpthread
gcc -O3 --std=c99 -o trivial_checksum_example_host_upmem_v1_app -I. ./upmemInterface/upmem_direct_c_upmem.o  trivial_checksum_example_host1.c -g `dpu-pkg-config --cflags --libs dpu` -lstdc++ -lpthread
