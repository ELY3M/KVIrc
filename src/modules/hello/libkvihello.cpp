#include "KviApplication.h"
#include "KviLocale.h"
#include "KviModule.h"
#include "KviWindow.h"
#include "kvi_out.h"


static bool hello_cmd_about(KviKvsModuleCommandCall * c)
{

	c->window()->output(KVI_OUT_SYSTEMMESSAGE, __tr2qs_ctx("hello world module by ELY M.", "hello"));
	return true;
}

static bool hello_cmd_run(KviKvsModuleCommandCall * c)
{

	c->window()->output(KVI_OUT_SYSTEMMESSAGE, __tr2qs_ctx("Hello World :)", "hello"));		
	return true;
}


static bool hello_module_init(KviModule * m)
{

	KVSM_REGISTER_SIMPLE_COMMAND(m, "about", hello_cmd_about);
	KVSM_REGISTER_SIMPLE_COMMAND(m, "run", hello_cmd_run);    
	return true;
}


static bool hello_module_cleanup(KviModule *)
{
	return true;
}

KVIRC_MODULE(
    "hello",  // module name
    "1.0.0", // module version
    "ELY M.",
    "own style module",
    hello_module_init,
    0,
    0,
    hello_module_cleanup,
    0)