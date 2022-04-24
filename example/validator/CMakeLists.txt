project(pkv-validator-project)

include(../PKV.cmake)

set(TARGET_NAME pkv-validator)

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME} PRIVATE main.cpp)

setup_pkv_macros(${TARGET_NAME})

target_compile_options(${TARGET_NAME}
    PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8 /permissive->
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Werror=return-type>
)
