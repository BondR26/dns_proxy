# CMakeList.txt : CMake project for dns_proxy, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE PROJECT_SOURCE RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	./*)
# Add source to this project's executable.
add_executable (dns_proxy "dns_proxy.c" "dns_proxy.h" ${PROJECT_SOURCE})

target_include_directories(dns_proxy PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR}/
)

# TODO: Add tests and install targets if needed.
# Define the output directory for the binary
set(OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../bin)

# Ensure the output directory exists
file(MAKE_DIRECTORY ${OUTPUT_DIRECTORY})

# Custom command to copy the target to the bin directory
add_custom_command(TARGET dns_proxy POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:dns_proxy> ${OUTPUT_DIRECTORY}
    COMMENT "Copying dns_proxy to ${OUTPUT_DIRECTORY}"
)