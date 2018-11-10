// Example program
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>


template<typename T>
void sort(std::vector<T>& v) {
    bool changed = true;
    while (changed) {
        changed = false;
        for (unsigned int i = 0; i < v.size() - 1; ++i) {
            if (v[i] < v[i+1]) {
                std::swap(v[i], v[i+1]);
                changed = true;
            }
        }
    }    
}


int main() {
    if (0) {
        std::vector<int> v(8);
        
        v[0] = 1;
        v[1] = 4;
        v[2] = 5;
        v[3] = 7;
        v[4] = 2;
        v[5] = 3;
        v[6] = 6;
        v[7] = 8;
        
        sort(v);
        
        for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
            if (0 == *it % 2) {
                std::cout << *it << " ";
            }
        }
    }
	
	if (1) {
		std::vector<int> v { 1, 4, 5, 7, 2, 3, 6, 8 };
		std::sort(v.begin(), v.end(), [](int a, int b) { return a >= b; });
		std::for_each(v.begin(), v.end(), [](int i) { if (0 == i%2) std::cout << i << " "; });
		//std::copy_if(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "), [](int i) { return 0 == i%2; });
	}
}
