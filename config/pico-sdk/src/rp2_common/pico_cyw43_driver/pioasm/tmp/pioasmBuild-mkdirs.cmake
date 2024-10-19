# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/helene/.pico-sdk/sdk/2.0.0/tools/pioasm"
  "/home/helene/pico-projects/LED-matrix-i2c/pioasm"
  "/home/helene/pico-projects/LED-matrix-i2c/pioasm-install"
  "/home/helene/pico-projects/LED-matrix-i2c/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/tmp"
  "/home/helene/pico-projects/LED-matrix-i2c/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp"
  "/home/helene/pico-projects/LED-matrix-i2c/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src"
  "/home/helene/pico-projects/LED-matrix-i2c/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/helene/pico-projects/LED-matrix-i2c/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/helene/pico-projects/LED-matrix-i2c/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
