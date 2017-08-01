#include "xrdbutil.h"

#include "Displ.h"

#include <sstream>

using namespace std;

const std::string renderXrdbCmd() {
    stringstream ss;
    ss << "echo \"Xft.dpi: "
       << Displ::desiredDpi
       << "\" | xrdb -merge";
    return ss.str();
}
