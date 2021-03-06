#include <reverse.hpp>

#include <iostream>
#include <vector>
#include <array>
#include <string>

int main () {
    {
        std::vector<int> a{1,2,3,4,5,6,7};
        std::vector<std::string> b{"hey","how","are","you","doing"};
        std::cout << std::endl << "Reverse range test" << std::endl << std::endl;
        for (auto i : iter::reverse(a)) {
            std::cout << i << std::endl;
        }
        std::cout<<std::endl;
        //modify a
        for (auto & i : iter::reverse(a)) {
            std::cout << i << std::endl;
            i = 9;
        }
        std::cout<<std::endl;
        for (auto i : iter::reverse(a)) {
            std::cout << i << std::endl;
        }
        std::cout<<std::endl;
        //try strings
        for (auto s : iter::reverse(b)) {
            std::cout << s << std::endl;
        }
    }
}
