#ifndef DEBUG_HH
#define DEBUG_HH

#include <iostream>
#include <string_view>

struct Debug {
    bool enabled{false};
    const char* prefix{ " [DEBUG] >> " };

    template<typename T>
    Debug& operator<<(T const &value) {
        if (enabled) {
            std::cout << prefix << value;
            std::cout.flush();         // force flush
        }
        return *this;
    }

    Debug& operator<<(std::ostream& (*manip)(std::ostream&)) {
        if (enabled) {
            std::cout << manip;
            std::cout.flush();
        }
        return *this;
    }

    void setPrefix(std::string_view newPrefix) {
        prefix = newPrefix.data();
    }
};

// We'll define the one global instance in Debug.cc
extern Debug debug;

#endif // DEBUG_HH
