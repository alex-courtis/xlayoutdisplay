#ifndef XLAYOUTDISPLAY_SETTINGS_H
#define XLAYOUTDISPLAY_SETTINGS_H

#include <list>
#include <string>
#include <vector>
#include <boost/program_options/variables_map.hpp>

// user provided settings for this utility
class Settings {
public:
    Settings(const boost::program_options::variables_map &vm)
            : dpi(vm.count("dpi") ? vm["dpi"].as<const long>() : 0),
              info(vm.count("info")),
              noop(vm.count("noop")),
              mirror(vm.count("mirror")),
              order(vm.count("order") ? vm["order"].as<std::vector<std::string>>() : std::vector<std::string>()),
              primary(vm.count("primary") ? vm["primary"].as<std::string>() : std::string()),
              quiet(vm.count("quiet")) {}

    const long dpi;
    const bool info;
    const bool noop;
    const bool mirror;
    const std::vector<std::string> order;
    const std::string primary;
    const bool quiet;
};

#endif //XLAYOUTDISPLAY_SETTINGS_H
