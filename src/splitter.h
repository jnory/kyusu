#pragma once

#include <unordered_set>
#include <string>
#include <cassert>

namespace kyusu {
    template <class ARRAY>
    class Decider {
    public:
        Decider(ARRAY const &kuten, ARRAY const &kakko_hiraku, ARRAY const &kakko_tojiru): stack_(0) {
            kuten_.insert(std::begin(kuten), std::end(kuten));
            kakko_hiraku_.insert(std::begin(kakko_hiraku), std::end(kakko_hiraku));
            kakko_tojiru_.insert(std::begin(kakko_tojiru), std::end(kakko_tojiru));
        }

        bool is_eof(std::string const &c1, std::string const &c2) {
            bool is_kakko_hiraku = kakko_hiraku_.find(c1) != std::end(kakko_hiraku_);
            if(is_kakko_hiraku){
                ++stack_;
                return false;
            }
            bool is_kakko_tojiru = kakko_tojiru_.find(c1) != std::end(kakko_tojiru_);
            if(is_kakko_tojiru){
                stack_ = (uint64_t) std::max<int>(((int)stack_) - 1, 0);
                return false;
            }

            bool is_kuten = kuten_.find(c1) != std::end(kuten_);
            if(stack_ == 0 && is_kuten){
                if(c1.size() == 1){
                    return c2 == " ";
                }else{
                    return true;
                }
            }else{
                return false;
            }
        }

    private:
        std::unordered_set<std::string> kuten_;
        std::unordered_set<std::string> kakko_hiraku_;
        std::unordered_set<std::string> kakko_tojiru_;
        std::uint64_t stack_;
    };

    template<class D, typename Callback>
    class Splitter {
    public:
        Splitter(D &decider, Callback const &callback): decider_(decider), callback_(callback){
        }

        void split(std::string const &data){
            const char *c_str = data.c_str();
            std::string mc1 = get_utf8_character(c_str);
            std::uint64_t i = 0;
            std::uint64_t last = 0;
            while(i + 1 < data.size()){
                std::string mc2 = get_utf8_character(c_str + i + mc1.size());

                bool is_eof = decider_.is_eof(mc1, mc2);

                i += mc1.size();
                mc1 = mc2;

                if(is_eof){
                    callback_(data.substr(last, i - last));
                    last = i;
                }
            }
            callback_(data.substr(last));
        }


    private:
        std::string get_utf8_character(const char *c) {
            uint64_t length = get_variable_length_utf8(c);
            char mc[5];
            std::copy(c,  c + length, mc);
            mc[length] = '\0';
            return std::string(mc);
        }

        uint64_t get_variable_length_utf8(const char *c) {
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

        D &decider_;
        Callback const &callback_;
    };
}