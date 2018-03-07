#include "xrdbutil.h"

#include "Output.h"

#include <sstream>

using namespace std;

const std::string renderXrdbCmd(const long &dpi) {
    stringstream ss;
    ss << "echo \"Xft.dpi: "
       << dpi
       << "\" | xrdb -merge";
    return ss.str();
}
