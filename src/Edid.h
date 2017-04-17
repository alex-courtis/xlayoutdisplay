#ifndef XLAYOUTDISPLAYS_EDID_H
#define XLAYOUTDISPLAYS_EDID_H

#include <memory>

// Edid info from monitor - assumes 1.4 spec
class Edid {
public:
    Edid(const unsigned char *edid, const size_t length, const char *name);
    virtual ~Edid();

    int maxCmHoriz() const;
    int maxCmVert() const;

private:
    unsigned char *edid;
};

typedef std::shared_ptr<Edid> EdidP;

#endif //XLAYOUTDISPLAYS_EDID_H
