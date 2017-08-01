#include "xrdbutil.h"

#include "Displ.h"

#include <sstream>

using namespace std;

const std::string renderXrdbCmd() {
    stringstream ss;
    ss << "xrdb -merge <(echo \"Xft.dpi: "
       << Displ::desiredDpi
       << "\")";
    return ss.str();
}
