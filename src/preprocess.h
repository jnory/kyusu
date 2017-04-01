#pragma once

#include <cstdint>
#include <string>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/trim.hpp>
#include "config.h"

namespace kyusu {
    class LengthFilter {
    public:
        LengthFilter(uint64_t min, uint64_t max): min_(min), max_(max){
        }

        bool operator()(std::string const &line){
            return !(line.size() < min_ || line.size() > max_);
        }

    private:
        uint64_t min_;
        uint64_t max_;
    };

    class SpaceReplacer {
    public:
        SpaceReplacer(Config const &config): config_(config){}

        std::string operator()(std::string const &line){
            std::string tmp = line;
            for(auto space: config_.spaces()){
                boost::algorithm::replace_all(tmp, space, " ");
            }
            return tmp;
        }

    private:
        Config const & config_;
    };

    class StripReplacer {
    public:
        StripReplacer(){}

        std::string operator()(std::string const &line){
            return boost::algorithm::trim_copy(line);
        }
    };
}

