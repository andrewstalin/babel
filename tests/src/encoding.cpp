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
#include "gtest/gtest.h"
#include <string>
#include <sstream>
#include <errno.h>

std::vector<char> HexStringToCharArray(const std::string& hex)
{
	std::istringstream stream(hex);
	std::vector<char> bytes;
	bytes.reserve(hex.size() / 2);

	for (auto i = 0u; i < hex.length(); i += 2) 
	{
		std::string byte = hex.substr(i, 2);
		bytes.push_back(static_cast<char>(strtol(byte.c_str(), nullptr, 16)));
	}

	return bytes;
}

void ConvertTest(const char* from, const char* to, const std::vector<char>& expected, const std::vector<char>& data)
{
	auto enc = babel::Encoding::open(from, to);
	auto actual_size = enc.encoding_size(data.data(), data.size());
	std::vector<char> actual(actual_size);
	auto encoding_size = enc.encode(data.data(), data.size(), const_cast<char*>(actual.data()), actual_size);

	ASSERT_TRUE(encoding_size > 0);
	ASSERT_EQ(expected, actual);
}

#ifdef _WIN32
TEST(encoding_case, to_wide_string_cast_test)
{
	auto actual = babel::string_cast(u8"Привет, world!", 65001);
	ASSERT_EQ(std::wstring(L"Привет, world!"), actual);
}

TEST(encoding_case, from_wide_string_cast_test)
{
	auto actual = babel::string_cast(L"Привет, world!", 65001);
	ASSERT_EQ(u8"Привет, world!", actual);
}
#endif

TEST(encoding_case, win1251_to_utf16le_test)
{
	ConvertTest("WINDOWS-1251", "UTF-16LE", HexStringToCharArray("1f04400438043204350442042c00200077006f0072006c0064002100"), HexStringToCharArray("cff0e8e2e5f22c20776f726c6421"));
}

TEST(encoding_case, utf8_to_utf16le_test)
{
	ConvertTest("UTF-8", "UTF-16LE", HexStringToCharArray("1f04400438043204350442042c00200077006f0072006c0064002100"), HexStringToCharArray("d09fd180d0b8d0b2d0b5d1822c20776f726c6421"));
}

TEST(encoding_case, utf16le_to_utf8_test)
{
	ConvertTest("UTF-16LE", "UTF-8", HexStringToCharArray("d09fd180d0b8d0b2d0b5d1822c20776f726c6421"), HexStringToCharArray("1f04400438043204350442042c00200077006f0072006c0064002100"));
}