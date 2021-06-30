/*
   Copyright 2018 Alexander Courtis

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>

#include "src/layout.h"
#include "src/util.h"

using namespace std;
namespace po = boost::program_options;

int main(int argc, const char **argv) {
    try {
        po::variables_map vm;

        // common options
        po::options_description options("CLI, $XDG_CONFIG_HOME/.xlayoutdisplay, $HOME/.xlayoutdisplay and /etc/xlayoutdisplay");
        options.add_options()
                ("dpi,d", po::value<long>(), "DPI override")
                ("mirror,m", "mirror outputs using the lowest common resolution")
                ("order,o", po::value<vector<string>>(), "order of outputs, repeat as needed")
                ("primary,p", po::value<string>(), "primary output")
                ("quiet,q", "suppress feedback");

        // file options
        po::options_description fileOptions("$XDG_CONFIG_HOME/.xlayoutdisplay, $HOME/.xlayoutdisplay and /etc/xlayoutdisplay");
        fileOptions.add(options);

        // command line options
        po::options_description cliOptions("CLI");
        cliOptions.add_options()
                ("help,h", "print this help text and exit")
                ("info,i", "print information about current outputs and exit")
                ("noop,n", "perform a trial run and exit")
                ("version,v", "print version string");
        cliOptions.add(options);

        // command line options take precedence
        po::store(po::command_line_parser(argc, argv).options(cliOptions).run(), vm);
        po::notify(vm);

        // file options afterwards
        ifstream ifs(resolveEnvPath("XDG_CONFIG_HOME", ".xlayoutdisplay"));
        if (!ifs)
            ifs = ifstream(resolveEnvPath("HOME", ".xlayoutdisplay"));
        if (!ifs)
            ifs = ifstream("/etc/xlayoutdisplay");
        if (ifs) {
            po::store(parse_config_file(ifs, fileOptions), vm);
            po::notify(vm);
        }

        // usage
        if (vm.count("help")) {
            cout << "Arranges outputs in a left to right manner, using highest resolution and refresh.\n"
                    "DPI is calculated based on the first or primary output's EDID information and rounded to the nearest 12.\n"
                    "Laptop outputs are turned off when the lid is closed.\n"
                    "\n"
                    "e.g.  xlayoutdisplay -p DP-4 -o HDMI-0 -o DP-4\n"
                    "\n";
            cout << cliOptions;
            return EXIT_SUCCESS;
        }

        // version
        if (vm.count("version")) {
            cout << argv[0] << " " << VERSION << endl;
            return EXIT_SUCCESS;
        }

        // render settings
        const Settings settings(vm);

        // execute
        return WEXITSTATUS(layout(settings));
    } catch (const exception &e) {
        cerr << argv[0] << ": " << e.what() << ", exiting\n";
        return EXIT_FAILURE;
    } catch (...) {
        cerr << argv[0] << ": unknown exception, exiting\n";
        return EXIT_FAILURE;
    }
}
