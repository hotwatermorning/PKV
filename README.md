# PKV generator and validator

This project provides a key generator and a validator of PKV (Partial Key Verification).

## How to use.

This project can be used as a header only library.

You need to define these macros to use the validator function.

- HWM_PKV_SUBKEY0_PARAM_A
- HWM_PKV_SUBKEY0_PARAM_B
- HWM_PKV_SUBKEY0_PARAM_C
- HWM_PKV_SUBKEY1_PARAM_A
- HWM_PKV_SUBKEY1_PARAM_B
- HWM_PKV_SUBKEY1_PARAM_C
- HWM_PKV_SUBKEY2_PARAM_A
- HWM_PKV_SUBKEY2_PARAM_B
- HWM_PKV_SUBKEY2_PARAM_C
- HWM_PKV_SUBKEY3_PARAM_A
- HWM_PKV_SUBKEY3_PARAM_B
- HWM_PKV_SUBKEY3_PARAM_C
- HWM_PKV_CHECKSUM_MAGIC_NUMBER_LEFT
- HWM_PKV_CHECKSUM_MAGIC_NUMBER_RIGHT
- HWM_PKV_USE_SUBKEY0
- HWM_PKV_USE_SUBKEY1
- HWM_PKV_USE_SUBKEY2
- HWM_PKV_USE_SUBKEY3

If you use CMake to configure your project, it is convenient to use setup_pkv_defines() command in [PVK.cmake.sample](PKV.cmake.sample).

To use the generator function, this macro should also be defined with the value 1.

- HWM_PKV_ENABLE_KEY_GENERATOR=1

Once these macros are defined, include [PKV.h](./include/PKV/PKV.h) in a source file of your project.

```cpp
#include <PKV/PKV.h>
```

And invoke the validator and the generator function like this.

```cpp
std::vector<std::string> keys;

// generate keys
for(int i = 0; i < 100; ++i) {
    keys.push_back(hwm::PKV::generateKey(i));
}

// validate all keys
for(auto const & key: keys) {
    if(hwm::PKV::isValidKey(key)) {
        std::cout << "Valid key." << std::endl;
    } else {
        std::cout << "Invalid key." << std::endl;
    }
}
```

## Build example apps

```cpp
cd /path/to/PKV
cmake -B build -G Xcode .
cmake --build build

# run generator
./build/example/generator/Debug/pkv-generator

# run validator
./build/example/validator/Debug/pkv-validator
```
