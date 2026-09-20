#include "KviApplication.h"
#include "KviLocale.h"
#include "KviModule.h"
#include "KviWindow.h"
#include "kvi_out.h"


static bool ownstyle_cmd_about(KviKvsModuleCommandCall * c)
{

	c->window()->output(KVI_OUT_SYSTEMMESSAGE, __tr2qs_ctx("ownstyle module by ELY M.", "ownstyle"));
	return true;
}

static bool ownstyle_cmd_run(KviKvsModuleCommandCall * c)
{

	c->window()->output(KVI_OUT_SYSTEMMESSAGE, __tr2qs_ctx("ownstyle module loaded and changed the colors :)", "ownstyle"));
	if (g_pApp) {
		QString styleSheet = "* { color: black; background-color: #959CBF; } "
		                     "QTextEdit { background: #E2E2E2; } "
		                     "QLineEdit { background: #E2E2E2; } "
		                     "QListView { background: #E2E2E2; } "
		                     "QListWidget { background: #E2E2E2; } "
		                     "QMessageBox { background: #E2E2E2; } "
		                     "QSpinBox { background: #E2E2E2; } "
		                     "QTreeView { background: #E2E2E2; } "
		                     "QCheckBox { background: #E2E2E2; }";
		g_pApp->setStyleSheet(styleSheet);    
	} 	
		
	return true;
}


static bool ownstyle_module_init(KviModule * m)
{

	KVSM_REGISTER_SIMPLE_COMMAND(m, "about", ownstyle_cmd_about);
	KVSM_REGISTER_SIMPLE_COMMAND(m, "run", ownstyle_cmd_run);
	
	if (g_pApp) {
		QString styleSheet = "* { color: black; background-color: #959CBF; } "
		                     "QTextEdit { background: #E2E2E2; } "
		                     "QLineEdit { background: #E2E2E2; } "
		                     "QListView { background: #E2E2E2; } "
		                     "QListWidget { background: #E2E2E2; } "
		                     "QMessageBox { background: #E2E2E2; } "
		                     "QSpinBox { background: #E2E2E2; } "
		                     "QTreeView { background: #E2E2E2; } "
		                     "QCheckBox { background: #E2E2E2; }";
		g_pApp->setStyleSheet(styleSheet);    
	}    
	return true;
}


static bool ownstyle_module_cleanup(KviModule *)
{
	return true;
}

KVIRC_MODULE(
    "ownstyle",  // module name
    "1.0.0", // module version
    "ELY M.",
    "own style module",
    ownstyle_module_init,
    0,
    0,
    ownstyle_module_cleanup,
    0)