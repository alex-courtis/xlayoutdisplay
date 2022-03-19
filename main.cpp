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
#include <regex>
#include <getopt.h>

#include "src/layout.h"
#include "src/util.h"

using namespace std;

bool parseBool(const string &str) {
    return regex_match(str, regex("true|on|1", regex::icase));
}

long parseLong(const string &name, const string &val) {
    try {
        return stoi(val);
    } catch (invalid_argument &e) {
        throw invalid_argument("invalid " + name + ": " + val);
    }
}

void usage(std::ostream &os) {
    os <<
        "Arranges outputs in a left to right manner, using highest resolution and refresh.\n"
        "DPI is calculated based on the first or primary output's EDID information and rounded to the nearest 12.\n"
        "Laptop outputs are turned off when the lid is closed.\n"
        "\n"
        "e.g.  xlayoutdisplay -p DP-4 -o HDMI-0 -o DP-4\n"
        "\n"
        "CLI:\n"
        "-h [ --help ]          print this help text and exit\n"
        "-i [ --info ]          print information about current outputs and exit\n"
        "-n [ --noop ]          perform a trial run and exit\n"
        "-v [ --version ]       print version string\n"
        "\n"
        "CLI, $XDG_CONFIG_HOME/.xlayoutdisplay, $HOME/.xlayoutdisplay and /etc/xlayoutdisplay:\n"
        "-d [ --dpi ] arg       DPI override\n"
        "-r [ --rate ] arg      Refresh rate override\n"
        "-m [ --mirror ]        mirror outputs using the lowest common resolution\n"
        "-o [ --order ] arg     order of outputs, repeat as needed\n"
        "-p [ --primary ] arg   primary output\n"
        "-q [ --quiet ]         suppress feedback\n";
}

void parseCfgFile(ifstream &ifs, Settings &settings) {
    string line;
    while (getline(ifs, line)) {

        // comments and empties
        if (regex_match(line, regex("\\s*#.*")) || regex_match(line, regex("\\s*"))) {
            continue;
        }

        // name=val
        smatch match;
        if (!regex_match(line, match, regex("\\s*(\\S+)\\s*=\\s*(\\S+)\\s*")) || match.size() != 3) {
            continue;
        }

        if (match[1] == "dpi") {
            settings.dpi = parseLong(match[1], match[2]);
        } else if (match[1] == "rate") {
            settings.rate = parseLong(match[1], match[2]);
        } else if (match[1] == "mirror") {
            settings.mirror = parseBool(match[2]);
        } else if (match[1] == "order") {
            settings.order.push_back(match[2]);
        } else if (match[1] == "primary") {
            settings.primary = match[2];
        } else if (match[1] == "quiet") {
            settings.quiet = parseBool(match[2]);
        } else {
            throw invalid_argument("unrecognised file option '" + match[0].str() + "'");
        }
    }

}

void parseArgs(int argc, char **argv, Settings &settings) {
	static struct option long_options[] = {
		{ "help",          no_argument,       0, 'h' },
		{ "info",          no_argument,       0, 'i' },
		{ "noop",          no_argument,       0, 'n' },
		{ "version",       no_argument,       0, 'v' },
		{ "dpi",           required_argument, 0, 'd' },
		{ "rate",          required_argument, 0, 'r' },
		{ "mirror",        no_argument,       0, 'm' },
		{ "order",         required_argument, 0, 'o' },
		{ "primary",       required_argument, 0, 'p' },
		{ "quiet",         no_argument,       0, 'q' },
		{ 0,               0,                 0,  0  }
	};
	static const char *short_options = "hinvd:r:mo:p:q";

	int c;
	while (1) {
		int long_index = 0;
		c = getopt_long(argc, argv, short_options, long_options, &long_index);
		if (c == -1)
			break;
		switch (c) {
			case 'h':
				usage(cout);
				exit(EXIT_SUCCESS);
			case 'i':
                settings.info = true;
				break;
			case 'n':
                settings.noop = true;
				break;
			case 'v':
                cout << argv[0] << " " << VERSION << endl;
                exit(EXIT_SUCCESS);
			case 'd':
                settings.dpi = parseLong("dpi", optarg);
                break;
			case 'r':
                settings.rate = parseLong("rate", optarg);
                break;
			case 'm':
                settings.mirror = true;
                break;
			case 'o':
                settings.order.push_back(optarg);
                break;
			case 'p':
                settings.primary = optarg;
                break;
			case 'q':
                settings.quiet = true;
                break;
			case '?':
			default:
				usage(cerr);
				exit(EXIT_FAILURE);
		}
	}
}

int main(int argc, char **argv) {
    try {
        Settings settings;
        try {
            // file options
            ifstream ifs(resolveEnvPath("XDG_CONFIG_HOME", ".xlayoutdisplay"));
            if (!ifs)
                ifs = ifstream(resolveEnvPath("HOME", ".xlayoutdisplay"));
            if (!ifs)
                ifs = ifstream("/etc/xlayoutdisplay");
            if (ifs) {
                parseCfgFile(ifs, settings);
            }

            // command line options override
            parseArgs(argc, argv, settings);

        } catch (invalid_argument &e) {
            cerr << argv[0] << ": " << e.what() << endl << endl;
            usage(cerr);
            return EXIT_FAILURE;
        }

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
