编译：
./build.sh


优化后SDK的checksum运行程序，symbol使用DPU_MRAM_HEAP_POINTER_NAME：
sudo ./trivial_checksum_example_host_direct_app

优化后SDK的checksum运行程序，symbol使用变量名：
sudo ./trivial_checksum_example_host_direct_app

修改NR_DPUS就可以设置rank数量