#include "bench.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <numeric>

using namespace std;

class Solution {
public:
};

int main()
{
    try
    {
        Bench bench("in.txt", "out.txt", "expected.txt");

        Solution s;

        bench.start();
        {
            //************* CALL SOLUTION METHOD HERE *************//

            //************* CALL SOLUTION METHOD HERE *************//
        }

        bench.stop();
        bench.compareResults();
    }
    catch(const std::exception& e_)
    {
        std::cerr << e_.what() << '\n';
    }
    return 0;
}
