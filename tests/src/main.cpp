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

#include <stdio.h>
#include "gtest/gtest.h"

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	int ret = RUN_ALL_TESTS();

	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "--waitAnyKey") == 0)
		{
			std::puts("Press any key to exit");
			std::getchar();
			break;
		}
	}

	return ret;
}