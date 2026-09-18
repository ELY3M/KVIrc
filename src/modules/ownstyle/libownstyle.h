#ifndef _OWNSTYLE_H_
#define _LIBKVIOWNSTYLE_H_

#include "KviModule.h"

// Mandatory KVIrc entry points
extern "C" {
    KVIRC_MODULE_API bool ownstyle_module_init(KviModule * m);
    KVIRC_MODULE_API bool ownstyle_module_cleanup(KviModule * m);
    KVIRC_MODULE_API bool ownstyle_module_can_unload(KviModule * m);
}

#endif // _OWNSTYLE_H_
