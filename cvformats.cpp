#include "cfptr.h"
#include <CoreVideo/CVPixelFormatDescription.h>
#include <string>
#include <sstream>
using namespace std;
using namespace apple;


string fourcc_to_str(uint32_t fcc)
{
    stringstream ss;
    ss << std::hex << fcc << std::dec;
    char t[] = { '\'', char((fcc>>24)&0xff), char((fcc>>16)&0xff),char((fcc>>8)&0xff),  char(fcc & 0xff), '\''};
    for (auto i : t)
        if (!i)
            return ss.str();
    return string(t, std::size(t));
}


int main()
{
    cfptr<CFArrayRef> a = CVPixelFormatDescriptionArrayCreateWithAllPixelFormatTypes(kCFAllocatorDefault);
    cfptr<> desc = CFStringCreateWithFormat(nullptr, nullptr, CFSTR("%@"), (CFTypeRef)a);
    const auto n = CFArrayGetCount(a);
    for (CFIndex i = 0; i < n; i++) {
        cfptr<CFNumberRef> v = (CFNumberRef)CFArrayGetValueAtIndex(a, i);
        cfptr<> vv = CFStringCreateWithFormat(nullptr, nullptr, CFSTR("%@"), (CFTypeRef)v);
        UInt32 x = 0;
        CFNumberGetValue((CFNumberRef)v, CFNumberGetType(v)/*kCFNumberSInt32Type*/, &x);
        cfptr<CFDictionaryRef> d = CVPixelFormatDescriptionCreateWithPixelFormatType(kCFAllocatorDefault, x);
        desc = CFStringCreateWithFormat(nullptr, nullptr, CFSTR("%@"), (CFTypeRef)d);
        printf("%ld (%s) - %s\n", i, fourcc_to_str(x).data(), CFStringGetCStringPtr(desc, kCFStringEncodingUTF8));
    };
}
