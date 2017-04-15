#pragma once

#include <string>
#include <cassert>

namespace kyusu {

    class UTF8 {
    public:
        typedef std::uint32_t U8Char;
        static constexpr const U8Char ASCII_ZERO = 48;
        static constexpr const U8Char ASCII_NINE = 57;
        static constexpr const U8Char ASCII_A = 65;
        static constexpr const U8Char ASCII_Z = 90;
        static constexpr const U8Char ASCII_a = 97;
        static constexpr const U8Char ASCII_z = 122;

        static constexpr const U8Char KATAKANA_A = 0xE382A1;
        static constexpr const U8Char KATAKANA_VO = 0xE383BA;

        static bool is_ascii(U8Char c){
            return c < 128;
        }

        static bool is_num(U8Char c){
            return ASCII_ZERO <= c && c <= ASCII_NINE;
        }

        static bool is_upper_alphabet(U8Char c){
            return ASCII_A <= c && c <= ASCII_Z;
        }

        static bool is_lower_alphabet(U8Char c){
            return ASCII_a <= c && c <= ASCII_z;
        }

        static bool is_alphabet(U8Char c){
            return is_upper_alphabet(c) || is_lower_alphabet(c);
        }

        static bool is_alnum(U8Char c){
            return is_num(c) || is_alphabet(c);
        }

        static bool is_katakana(U8Char c){
            return KATAKANA_A <= c && c <= KATAKANA_VO;
        }

        static U8Char to_u8char(char const *c, std::uint64_t length) {
            unsigned char const *uc = reinterpret_cast<unsigned char const *>(c);
            std::uint32_t value = 0;
            for(std::uint64_t i = 0; i < length; ++i){
                value <<= 8;
                value += uc[i];
            }
            return value;
        }

        template<typename Callback>
        static void iterate_by_character(char const *c, Callback const &callback) {
            std::uint64_t i = 0;
            while(c[i] != '\0'){
                std::uint64_t length = get_variable_length(c + i);
                callback(c + i, length);
                i += length;
            }
        }

        static uint64_t get_variable_length(const char *c) {
            // suppose that the character encoding is UTF-8.
            uint32_t x = (uint32_t) *reinterpret_cast<unsigned char const *>(c);
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