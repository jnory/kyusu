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

        bool filter(std::string const &line) const {
            return !(line.size() < min_ || line.size() > max_);
        }

    private:
        uint64_t const min_;
        uint64_t const max_;
    };

    class SpaceReplacer {
    public:
        SpaceReplacer(Config const &config): config_(config){}

        std::string replace(std::string const &line) const {
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

        std::string replace(std::string const &line) const {
            return boost::algorithm::trim_copy(line);
        }
    };
}

