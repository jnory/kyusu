#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <boost/property_tree/json_parser.hpp>

namespace kyusu {
    class Config {
    public:
        Config(std::string const &path) : path_(path){
            boost::property_tree::ptree json;
            boost::property_tree::read_json(path, json);

            for(auto const &item: json.get_child("space")){
                spaces_.push_back(item.second.get<std::string>(""));
            }
            for(auto const &item: json.get_child("hyphen")){
                hyphens_.push_back(item.second.get<std::string>(""));
            }
            for(auto const &item: json.get_child("kakko_hiraku")){
                kakko_hirakus_.push_back(item.second.get<std::string>(""));
            }
            for(auto const &item: json.get_child("kakko_tojiru")){
                kakko_tojirus_.push_back(item.second.get<std::string>(""));
            }
            for(auto const &item: json.get_child("kuten")){
                kutens_.push_back(item.second.get<std::string>(""));
            }
        }

        std::vector<std::string> const &spaces() const {
            return spaces_;
        }

    private:
        std::string const &path_;
        std::vector<std::string> spaces_;
        std::vector<std::string> hyphens_;
        std::vector<std::string> kakko_hirakus_;
        std::vector<std::string> kakko_tojirus_;
        std::vector<std::string> kutens_;
    };
}