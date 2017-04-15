#pragma once

#include <string>
#include <cassert>

namespace kyusu {
    class UTF8 {
    public:
        typedef std::uint32_t U8Char;

        static U8Char to_u8char(const char *c, std::uint64_t length) {
            std::uint32_t value = 0;
            for(std::uint64_t i = 0; i < length; ++i){
                value <<= 8;
                value += c[i];
            }
            return value;
        }

        static bool is_ascii(U8Char c){
            return c < 128;
        }

        template<typename Callback>
        static void iterate_by_character(const char *c, Callback const &callback) {
            std::uint64_t i = 0;
            while(c[i] != '\0'){
                std::uint64_t length = get_variable_length(c + i);
                callback(c + i, length);
                ++i;
            }
        }

        static uint64_t get_variable_length(const char *c) {
            // suppose that the character encoding is UTF-8.
            uint32_t x = (uint32_t)c[0];
            x <<= 24;
            uint64_t length = __builtin_clz(~x);
            if(length == 0){
                ++length;
            }

            // maximum 4 bytes
            assert(length < 4);
            return length;
        }
    };
}