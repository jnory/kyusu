#pragma once

#include <unordered_set>
#include <string>
#include <cassert>

#include "./utf8.h"

namespace kyusu {
    template <class ARRAY>
    class Decider {
    public:
        Decider(ARRAY const &kuten, ARRAY const &kakko_hiraku, ARRAY const &kakko_tojiru): stack_(0) {
            for(std::string const &c: kuten){
                kuten_.insert(UTF8::to_u8char(c.c_str(), c.size()));
            }
            for(std::string const &c: kakko_hiraku){
                kakko_tojiru_.insert(UTF8::to_u8char(c.c_str(), c.size()));
            }
            for(std::string const &c: kakko_tojiru){
                kakko_tojiru_.insert(UTF8::to_u8char(c.c_str(), c.size()));
            }
            hash_blank_ = UTF8::to_u8char(u8" ", 1);
        }

        bool is_eof(UTF8::U8Char h1, UTF8::U8Char h2) {
            bool is_kakko_hiraku = kakko_hiraku_.find(h1) != std::end(kakko_hiraku_);
            if(is_kakko_hiraku){
                ++stack_;
                return false;
            }
            bool is_kakko_tojiru = kakko_tojiru_.find(h1) != std::end(kakko_tojiru_);
            if(is_kakko_tojiru){
                stack_ = (uint64_t) std::max<int>(((int)stack_) - 1, 0);
                return false;
            }

            bool is_kuten = kuten_.find(h1) != std::end(kuten_);
            if(stack_ == 0 && is_kuten){
                if(UTF8::is_ascii(h1)){
                    return h2 == hash_blank_;
                }else{
                    return true;
                }
            }else{
                return false;
            }
        }

    private:
        std::unordered_set<std::uint32_t> kuten_;
        std::unordered_set<std::uint32_t> kakko_hiraku_;
        std::unordered_set<std::uint32_t> kakko_tojiru_;
        std::uint64_t stack_;
        std::uint32_t hash_blank_;
    };

    template<class D, typename Callback>
    class Splitter {
    public:
        Splitter(D &decider, Callback const &callback): decider_(decider), callback_(callback){
        }

        void split(std::string const &data){
            auto c = data.c_str();

            auto c1 = data.c_str();
            auto length1 = UTF8::get_variable_length(c);
            auto h1 = UTF8::to_u8char(c1, length1);

            std::uint64_t last = 0;

            auto func = [&](char const *c2, std::uint64_t length2) {
                auto h2 = UTF8::to_u8char(c2, length2);
                bool is_eof = decider_.is_eof(h1, h2);

                if(is_eof){
                    callback_(data.substr(last, (std::uint64_t)std::distance(c + last, c2)));
                    last = (std::uint64_t) std::distance(c, c2);
                }

                c1 = c2;
                h1 = h2;
            };
            UTF8::iterate_by_character(data.c_str() + length1, func);
            callback_(data.substr(last));
        }


    private:
        D &decider_;
        Callback const &callback_;
    };
}