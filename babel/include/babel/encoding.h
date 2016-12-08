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

#ifndef _BABEL_ENCODING_H_
#define _BABEL_ENCODING_H_

#include <string>

#ifdef _WIN32
#include "babel/iconv.h"
#else
#include <iconv.h>
#endif

namespace babel
{
	class Encoding
	{
	private:
		iconv_t iconv_;

	public:
		Encoding(Encoding&& val)
			: iconv_(val.iconv_)
		{
			val.iconv_ = nullptr;
		}

		Encoding(const Encoding&) = delete;
		Encoding& operator=(const Encoding&) = delete;

		~Encoding()
		{
			if (iconv_ != nullptr)
			{
				iconv_close(iconv_);
				iconv_ = nullptr;
			}
		}

		static const char* codepage_to_name(int codepage);

		static Encoding open(const char* from, const char* to);

		size_t encoding_size(const char* input, size_t input_size) const;
		size_t encode(const char* input, size_t input_size, char* output, size_t output_size);

	private:
		Encoding(iconv_t conv)
			: iconv_(conv)
		{}
	};

	std::string encode(const char* from, const char* to, const char* str, size_t size);

#ifdef _WIN32
	std::wstring string_cast(const std::string& str, uint32_t codepage);
	std::string string_cast(const std::wstring& str, uint32_t codepage);

	std::wstring string_cast(const char* str, size_t size, uint32_t codepage);
	std::string string_cast(const wchar_t* str, size_t size, uint32_t codepage);

	std::wstring string_cast(const std::string& str, const char* codepage);
	std::string string_cast(const std::wstring& str, const char* codepage);

	std::wstring string_cast(const char* str, size_t size, const char* codepage);
	std::string string_cast(const wchar_t* str, size_t size, const char* codepage);
#endif
}

#endif