// Licensed under the MIT License <http://opensource.org/licenses/MIT>
// Author: Andrew Stalin <andrew.stalin@gmail.com>
//
// THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
// IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
// FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
// AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
// LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "babel/encoding.h"
#include "codepages.h"
#include <vector>
#include <stdexcept>
#include <cassert>
#include <errno.h>

using namespace babel;

const size_t BUFFER_SIZE = 100;

const char* Encoding::codepage_to_name(int codepage)
{
	for (auto i = 0u; codepage_alias[i].name != nullptr; ++i)
	{
		if (codepage_alias[i].codepage == codepage)
		{
			return codepage_alias[i].name;
		}
	}

	throw std::runtime_error("unknown codepage");
}

Encoding Encoding::open(const char* from, const char* to)
{
	auto conv = iconv_open(to, from);

	if (conv == reinterpret_cast<iconv_t>(-1))
	{
		if (errno == EINVAL)
		{
			throw std::runtime_error(std::string("the conversion from ") + from + " to " + to + " is not supported by the implementation");
		}
		else
		{
			throw std::runtime_error("unknown error");
		}
	}

	return Encoding(conv);
}

size_t Encoding::encoding_size(const char* input, size_t input_size) const
{
	assert(input != nullptr);
	auto data_ptr = const_cast<char*>(input);
	size_t result{ 0 };

	while (input_size > 0)
	{
		char buffer[BUFFER_SIZE];
		auto buffer_ptr = buffer;
		size_t outbytesleft{ BUFFER_SIZE };
		auto res = iconv(iconv_, &data_ptr, &input_size, &buffer_ptr, &outbytesleft);

		if (res == static_cast<size_t>(-1))
		{
			if (errno == E2BIG)
			{}
			else if (errno == EILSEQ)
			{
				throw std::runtime_error("an invalid multibyte sequence has been encountered in the input");
			}
			else if (errno == EINVAL)
			{
				throw std::runtime_error("an incomplete multibyte sequence has been encountered in the input");
			}
			else
			{
				throw std::runtime_error("unknown error");
			}
		}

		result += BUFFER_SIZE - outbytesleft;
	}

	return result;
}

size_t Encoding::encode(const char* input, size_t input_size, char* output, size_t output_size)
{
	assert(input != nullptr && output != nullptr);
	auto data_ptr = const_cast<char*>(input);
	auto outbytesleft = output_size;
	
	while (input_size > 0)
	{
		auto res = iconv(iconv_, &data_ptr, &input_size, &output, &outbytesleft);

		if (res == static_cast<size_t>(-1))
		{
			if (errno == E2BIG)
			{
				return res;
			}
			else if (errno == EILSEQ)
			{
				throw std::runtime_error("an invalid multibyte sequence has been encountered in the input");
			}
			else if (errno == EINVAL)
			{
				throw std::runtime_error("an incomplete multibyte sequence has been encountered in the input");
			}
			else
			{
				throw std::runtime_error("unknown error");
			}
		}
	}

	return output_size - outbytesleft;
}

std::string babel::encode(const char* from, const char* to, const char* str, size_t size)
{
	auto enc = Encoding::open(from, to);

	std::string result(enc.encoding_size(str, size), 0);
	auto result_ptr = const_cast<char*>(result.data());
	auto encoding_size = enc.encode(str, size, result_ptr, result.size());
	
	if (encoding_size != result.size())
	{
		throw std::runtime_error("string encoding error");
	}

	return result;
}

#ifdef _WIN32

std::wstring babel::string_cast(const char* str, size_t size, const char* codepage)
{
	assert(str != nullptr);

	if (size == 0)
	{
		return std::wstring();
	}

	auto enc = babel::Encoding::open(codepage, "UTF-16LE");

	std::wstring result(enc.encoding_size(str, size) / 2, 0);
	auto result_ptr = reinterpret_cast<char*>(const_cast<wchar_t*>(result.data()));
	auto encoding_size = enc.encode(str, size, result_ptr, result.size() * 2);

	if (encoding_size == std::numeric_limits<size_t>::max())
	{
		throw std::runtime_error("insufficient buffer size");
	}

	return result;
}

std::string babel::string_cast(const wchar_t* str, size_t size, const char* codepage)
{
	assert(str != nullptr);

	if (size == 0)
	{
		return std::string();
	}

	auto enc = babel::Encoding::open("UTF-16LE", codepage);

	std::string result(enc.encoding_size(reinterpret_cast<const char*>(str), size * 2), 0);
	auto result_ptr = const_cast<char*>(result.data());
	auto encoding_size = enc.encode(reinterpret_cast<const char*>(str), size * 2, result_ptr, result.size());

	if (encoding_size == std::numeric_limits<size_t>::max())
	{
		throw std::runtime_error("insufficient buffer size");
	}

	return result;
}

std::wstring babel::string_cast(const std::string& str, const char* codepage)
{
	return string_cast(str.data(), str.size(), codepage);
}

std::string babel::string_cast(const std::wstring& str, const char* codepage)
{
	return string_cast(str.data(),str.size(), codepage);
}

std::wstring babel::string_cast(const std::string& str, uint32_t codepage)
{
	return string_cast(str.data(), str.size(), babel::Encoding::codepage_to_name(codepage));
}

std::string babel::string_cast(const std::wstring& str, uint32_t codepage)
{
	return string_cast(str.data(), str.size(), babel::Encoding::codepage_to_name(codepage));
}

std::wstring babel::string_cast(const char* str, size_t size, uint32_t codepage)
{
	return string_cast(str, size, babel::Encoding::codepage_to_name(codepage));
}

std::string babel::string_cast(const wchar_t* str, size_t size, uint32_t codepage)
{
	return string_cast(str, size, babel::Encoding::codepage_to_name(codepage));
}
#endif