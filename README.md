# PKV generator and validator

This project provides a key generator and a key validator of [PKV (Partial Key Verification)](http://www.brandonstaggs.com/2007/07/26/implementing-a-partial-serial-number-verification-system-in-delphi/).

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

Windows:
```cpp
cd C:¥path¥to¥develo¥PKV
cmake -B build -G "Visual Studio 16 2019" .
cmake --build build

# run generator
.¥build¥example¥generator¥Debug¥pkv-generator

# run validator
.¥build¥example¥generator¥Debug¥pkv-validator
```

macOS:
```cpp
cd /path/to/develop/PKV
cmake -B build -G Xcode .
cmake --build build

# run generator
./build/example/generator/Debug/pkv-generator

# run validator
./build/example/validator/Debug/pkv-validator
```

## Acknowledgement

This project is based on [juce_pkv_example](https://github.com/COx2/juce_pkv_example).
