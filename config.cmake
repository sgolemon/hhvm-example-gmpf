HHVM_EXTENSION(gmpf ext_gmpf.cpp)
HHVM_SYSTEMLIB(gmpf ext_gmpf.php)

find_path(GMP_INCLUDE_DIR NAMES gmp.h)
find_library(GMP_LIBRARY NAMES gmp)

if (NOT GMP_INCLUDE_DIR OR NOT GMP_LIBRARY)
  message(FATAL_ERROR "Unable to locate libgmp")
endif()

message(STATUS "GMP header: ${GMP_INCLUDE_DIR}")
message(STATUS "GMP library: ${GMP_LIBRARY}")

include_directories(${GMP_INCLUDE_DIR})
target_link_libraries(gmpf ${GMP_LIBRARY})
