#include "ownstyle.h"
#include "KviApplication.h"
#include "KviMainWindow.h"

#include <QString>

void OwnStyle()
{
    if (!g_pApp) return;

    // Define your Qt Style Sheet (QSS) layout using CSS rules
    QString cssStyles = 
"* { color: black; background-color: #959CBF }"
"QTextEdit { background: #E2E2E2 }"
"QLineEdit { background: #E2E2E2 }"
"QListView { background: #E2E2E2 }"
"QListWidget { background: #E2E2E2 }"
"QMessageBox { background: #E2E2E2 }"
"QSpinBox { background: #E2E2E2 }"
"QTreeView { background: #E2E2E2 }"
"QCheckBox { background: #E2E2E2 }";

    // Option A: Apply style globally across all KVIrc forms, widgets, and dialogs
    g_pApp->setStyleSheet(cssStyles);

    // Option B: If global overrides cause issues with chat rendering, 
    // target only the underlying main framework layer:
    /*
    KviMainWindow * pMainWin = g_pApp->mainWindow();
    if (pMainWin) {
        pMainWin->setStyleSheet(cssStyles);
    }
    */
}


