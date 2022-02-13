#include "bench.hpp"

#include <iostream>

int main()
{
    try
    {
        Bench bench("in.txt", "out.txt", "expected.txt");
        bench.start();
        {
            //************* CODE HERE *************//

            std::string test_input;
            int number;
            bench >> test_input >> number;
            bench << test_input << " " << number * 40;

            //************* CODE HERE *************//
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
