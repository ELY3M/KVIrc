#include "KviApplication.h"
#include "KviCString.h"
#include "KviModule.h"



static bool ownstyle_module_init(KviModule * m)
{

	KviApplication * pTheApp;
	QString styleSheet = "* { color: black; background-color: #959CBF; } QTextEdit { background: #E2E2E2; } QLineEdit { background: #E2E2E2; } QListView { background: #E2E2E2; } QListWidget { background: #E2E2E2; } QMessageBox { background: #E2E2E2; } QSpinBox { background: #E2E2E2; } QTreeView { background: #E2E2E2; } QCheckBox { background: #E2E2E2; }";
	pTheApp->setStyleSheet(styleSheet);    
    
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