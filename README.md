[![Build Status](https://travis-ci.org/andrewstalin/babel.svg?branch=master)](https://travis-ci.org/andrewstalin/babel)
[![Build Status](https://ci.appveyor.com/api/projects/status/8xmsyb7gj7nsurii/branch/master?svg=true)](https://ci.appveyor.com/project/andrewstalin/babel)

Babel is the library for encoding/decoding strings. Used for convert iconv and implementation using Win32 API by Yukihiro Nakadaira.

# Example

Using Encoding type

```c++
const std::string str_utf16_be = "...";
auto enc = Encoding::open("UTF-16BE", "UTF-8");
std::string result(enc.encoding_size(str_utf16_be.c_str(), str_utf16_be.size()), 0);
enc.encode(str_utf16_be.c_str(), str_utf16_be.size()), const_cast<char*>(result.data()), result.size());
```

Using encode function

```c++
const std::string str_utf16_be = "...";
auto str = babel::encode("UTF-16BE", "UTF-8", str_utf16_be.c_str(), str_utf16_be.size());
```

Using for convert single-byte to multi-byte and vise versa (for Windows only).

```c++
// convert UTF-8 string to wide string
std::wstring str = babel::string_cast(u8"Привет, мир!", 65001);

// convert wide string to WINDOWS-1251 string
std::string str = babel::string_cast(L"Привет, мир!", "WINDOWS-1251");
```
