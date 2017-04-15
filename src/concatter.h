#pragma once

#include <string>
#include <unordered_set>
#include <vector>
#include "utf8.h"

namespace kyusu {
    template<class ARRAY>
    class Concatter {
    public:
        Concatter(ARRAY const &hyphens){
            for(std::string const &c: hyphens){
                hyphens_.insert(UTF8::to_u8char(c.c_str(), c.size()));
            }
            blank_ = UTF8::to_u8char(u8" ", 1);
        }

        bool need_concat(UTF8::U8Char c1, UTF8::U8Char c2){
            if(UTF8::is_alnum(c1)){
                bool is_hyphen2 = hyphens_.find(c2) != std::end(hyphens_);
                return UTF8::is_alnum(c2) || is_hyphen2;
            }

            bool is_hyphen1 = hyphens_.find(c1) != std::end(hyphens_);
            if(is_hyphen1) {
                bool is_hyphen2 = hyphens_.find(c2) != std::end(hyphens_);
                bool is_katakana2;
                return UTF8::is_alnum(c2) || is_hyphen2 || UTF8::is_katakana(c2);
            }

            if(UTF8::is_katakana(c1)){
                return hyphens_.find(c2) != std::end(hyphens_);
            }

            return false;
        }

        std::string concat(std::string const &words) {
            auto c = words.c_str();

            auto c1 = c;
            auto length1 = UTF8::get_variable_length(c1);
            if(words.size() <= length1){
                return words;
            }
            auto h1 = UTF8::to_u8char(c1, length1);

            std::vector<char> result(words.size() + 1, 0);
            std::copy(c1, c1 + length1, std::begin(result));
            std::uint64_t last = length1;

            auto c2 = c + length1;
            auto length2 = UTF8::get_variable_length(c2);
            if(words.size() <= length1 + length2){
                return words;
            }
            auto h2 = UTF8::to_u8char(c2, length2);

            auto func = [&](char const *c3, std::uint64_t length3) {
                auto h3 = UTF8::to_u8char(c3, length3);

                if(h2 != blank_ || !need_concat(h1, h3)) {
                    std::copy(c2, c2 + length2, std::begin(result) + last);
                    last += length2;
                    assert(last <= words.size());
                }

                c2 = c3;
                h1 = h2;
                h2 = h3;
                length2 = length3;
            };
            UTF8::iterate_by_character(c + length1 + length2, func);
            std::copy(c2, c2 + length2, std::begin(result) + last);
            last += length2;
            result[last] = '\0';
            return std::string(result.data());
        }

    private:
        std::unordered_set<UTF8::U8Char> hyphens_;
        UTF8::U8Char blank_;
    };
}