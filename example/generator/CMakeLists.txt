project(pkv-generator-project)

include(../PKV.cmake)

set(TARGET_NAME pkv-generator)

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME} PRIVATE main.cpp)

setup_pkv_macros(${TARGET_NAME})

target_compile_definitions(${TARGET_NAME} PRIVATE HWM_PKV_ENABLE_KEY_GENERATOR=1)

target_compile_options(${TARGET_NAME}
    PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8 /permissive->
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Werror=return-type>
)

