#pragma once

#include <cassert>
#include <cstdint>
#include <regex>
#include <string>
#include <sstream>
#include <iomanip>

namespace hwm::PKV {

namespace detail {

inline
std::string replaceAll(std::string_view src, std::string_view pattern, std::string_view toReplace)
{
    std::string::size_type pos = 0;
    auto const npat = pattern.size();

    if(npat == 0) {
        assert("pattern should not be empty." && false);
        return std::string(src.begin(), src.end());
    }

    std::string ret;

    for( ; ; ) {
        auto found = src.find(pattern, pos);
        if(found == src.npos) {
            ret += src.substr(pos);
            return ret;
        }

        ret.append(src.begin() + pos, src.begin() + found);
        ret.append(toReplace.begin(), toReplace.end());
        pos = found + npat;
    }
}

inline
std::string toUpper(std::string_view str)
{
    std::string tmp;
    for(auto c: str) {
        if('a' <= c && c <= 'z') {
            c = 'A' + (c - 'a');
        }

        tmp.push_back(c);
    }

    return tmp;
}

inline
std::uint32_t fromHexString(std::string_view str)
{
    std::string tmp(str.begin(), str.end());
    return static_cast<std::uint32_t>(std::stoul(tmp, nullptr, 16));
}

//! Format a number as a hex string with zero padding to fill the width.
inline
std::string toHexString(std::uint32_t num, int width)
{
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(width) << std::hex << num;
    return toUpper(ss.str());
}

//! Test key format.
inline
bool isKeyFormatValid(std::string_view key)
{
    if(key.length() != 24) {
        return false;
    }

    std::regex pattern("^[0-9A-F]{4}-[0-9A-F]{4}-[0-9A-F]{4}-[0-9A-F]{4}-[0-9A-F]{4}$");

    if(std::regex_match(key.begin(), key.end(), pattern) == false) {
        return false;
    }

    return true;
}

//! Get a checksum number from a serial key without a checksum part.
inline
std::uint32_t getChecksumForSerial(std::string_view serial)
{
    // Use magic numbers
    std::uint32_t left = HWM_PKV_CHECKSUM_MAGIC_NUMBER_LEFT;
    std::uint32_t right = HWM_PKV_CHECKSUM_MAGIC_NUMBER_RIGHT;

    for(auto c: serial) {
        if(c == '-') { continue; }

        right += static_cast<unsigned char>(c);
        if (right > 0x000000ff)
        {
            right -= 0x000000ff;
        }

        left += right;

        if (left > 0x000000ff)
        {
            left -= 0x000000ff;
        }
    }

    return (left << 8) + right;
}


//! Make sure the serial key has the specified checksum.
inline
bool isSerialChecksumValid(std::string_view serial, std::uint32_t checksum)
{
    return getChecksumForSerial(serial) == checksum;
}

//! Get a subkey number from the specified seed.
inline
std::uint32_t getSubkeyFromSeed(std::uint32_t seed, std::uint32_t paramA, std::uint32_t paramB, std::uint32_t paramC)
{
    const std::uint32_t subkey_range_max_param_a = 25;
    const std::uint32_t subkey_range_max_param_b = 3;

    paramA = paramA % subkey_range_max_param_a;
    paramB = paramB % subkey_range_max_param_b;

    std::uint32_t subkey;
    if (paramA % 2 == 0)
    {
        subkey = (((seed >> paramA) & 0x000000ff) ^ ((seed >> paramB) | paramC)) & 0x000000ff;
    }
    else
    {
        subkey = (((seed >> paramA) & 0x000000ff) ^ ((seed >> paramB) & paramC)) & 0x000000ff;
    }

    return subkey;
}

}

//==============================================================================
// Test if the key is valid.
inline
bool isValidKey(std::string_view key)
{
    auto tmp = detail::toUpper(key);
    key = tmp;

    if (!detail::isKeyFormatValid(key))
    {
        return false;
    }

    const auto hyphenRemoved = detail::replaceAll(key, "-", "");
    const auto serial = hyphenRemoved.substr(0, 16);
    const auto checksum = detail::fromHexString(hyphenRemoved.substr(16, 4));

    if (!detail::isSerialChecksumValid(serial, checksum))
    {
        return false;
    }

    const auto seed = detail::fromHexString(serial.substr(0, 8));

#if HWM_PKV_USE_SUBKEY0
    // Verify 0th subkey
    {
        auto const expected = detail::getSubkeyFromSeed(seed, HWM_PKV_SUBKEY0_PARAM_A, HWM_PKV_SUBKEY0_PARAM_B, HWM_PKV_SUBKEY0_PARAM_C);
        auto const actual = detail::fromHexString(serial.substr(8, 2));
        if (expected != actual)
        {
            return false;
        }
    }
#endif

#if HWM_PKV_USE_SUBKEY1
    // Verify 1st subkey
    {
        auto const expected = detail::getSubkeyFromSeed(seed, HWM_PKV_SUBKEY1_PARAM_A, HWM_PKV_SUBKEY1_PARAM_B, HWM_PKV_SUBKEY1_PARAM_C);
        auto const actual = detail::fromHexString(serial.substr(10, 2));
        if (expected != actual)
        {
            return false;
        }
    }
#endif

#if HWM_PKV_USE_SUBKEY2
    // Verify 2nd subkey
    {
        auto const expected = detail::getSubkeyFromSeed(seed, HWM_PKV_SUBKEY2_PARAM_A, HWM_PKV_SUBKEY2_PARAM_B, HWM_PKV_SUBKEY2_PARAM_C);
        auto const actual = detail::serial.substr(12, 2);
        if (expected != actual)
        {
            return false;
        }
    }
#endif

#if HWM_PKV_USE_SUBKEY3
    // Verify 3rd subkey
    {
        auto const expected = detail::getSubkeyFromSeed(seed, HWM_PKV_SUBKEY3_PARAM_A, HWM_PKV_SUBKEY3_PARAM_B, HWM_PKV_SUBKEY3_PARAM_C);
        auto const actual = detail::serial.substr(14, 2);
        if (expected != actual)
        {
            return false;
        }
    }
#endif

    return true;
}

#if HWM_PKV_ENABLE_KEY_GENERATOR

//==============================================================================
// Generate a (legitimate) license key
inline
std::string generateKey(std::uint32_t seed)
{
    std::string serial = detail::toHexString(seed >> 16, 4) + "-" + detail::toHexString(seed & 0xFFFF, 4) + "-";
    serial += detail::toHexString(detail::getSubkeyFromSeed(seed, HWM_PKV_SUBKEY0_PARAM_A, HWM_PKV_SUBKEY0_PARAM_B, HWM_PKV_SUBKEY0_PARAM_C), 2);
    serial += detail::toHexString(detail::getSubkeyFromSeed(seed, HWM_PKV_SUBKEY1_PARAM_A, HWM_PKV_SUBKEY1_PARAM_B, HWM_PKV_SUBKEY1_PARAM_C), 2);
    serial += "-";
    serial += detail::toHexString(detail::getSubkeyFromSeed(seed, HWM_PKV_SUBKEY2_PARAM_A, HWM_PKV_SUBKEY2_PARAM_B, HWM_PKV_SUBKEY2_PARAM_C), 2);
    serial += detail::toHexString(detail::getSubkeyFromSeed(seed, HWM_PKV_SUBKEY3_PARAM_A, HWM_PKV_SUBKEY3_PARAM_B, HWM_PKV_SUBKEY3_PARAM_C), 2);
    serial += "-";

    std::uint32_t const checkSum = detail::getChecksumForSerial(detail::replaceAll(serial, "-", ""));
    assert(checkSum <= 0xFFFF);
    serial += detail::toHexString((checkSum >> 8) & 0xFF, 2) + detail::toHexString(checkSum & 0xFF, 2);

    return serial;
}
#endif

} // namespace hwm::PKV

