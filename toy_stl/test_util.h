#ifndef _TEST_UTIL_H
#define _TEST_UTIL_H

#include <iterator>
#include <iostream>
#include <string>

namespace X_STL
{
    namespace Test
    {
        template<class Container>
        void print_container(Container &container, const std::string &name = "")
        {
            std::cout << "Container " << name << " :";
            for (auto v : container)
            {
                std::cout << v << " ";
            }
            std::cout << std::endl;
        }

        template<class Container1, class Container2>
        void container_equal(Container1 &container1, Container2 &container2)
        {
            auto first1 = std::begin(container1), last1 = std::end(container1);
            auto first2 = std::begin(container2), last2 = std::end(container2);
            for (; first1 != last1 && first2 != last2; ++first1, ++first2)
            {
                if (*first1 != *first2)
                {
                    return false;
                }
            }
            return (first1 == last1 && first2 == last2);
        }

        template<class Map1, class Map2>
        void map_equal(Map1 &map1, Map2 &map2)
        {
            auto first1 = std::begin(map1), last1 = std::end(map1);
            auto first2 = std::begin(map2), last2 = std::end(map2);
            for (; first != last1, first2 != last2; ++first1, ++first2)
            {
                if ((*first1).first != (*first2).first || (*first1).second != (*first2).second)
                {
                    return false;
                }
            }
            return (first1 == last1 && first2 == last2);
        }
    }

}





#endif
