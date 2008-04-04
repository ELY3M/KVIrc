//
//   File : optw_away.cpp
//   Creation date : Mon Oct 29 15:37:16 2001 GMT by Szymon Stefanek
//
//   This file is part of the KVirc irc client distribution
//   Copyright (C) 2001 Szymon Stefanek (pragma at kvirc dot net)
//
//   This program is FREE software. You can redistribute it and/or
//   modify it under the terms of the GNU General Public License
//   as published by the Free Software Foundation; either version 2
//   of the License, or (at your opinion) any later version.
//
//   This program is distributed in the HOPE that it will be USEFUL,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//   See the GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program. If not, write to the Free Software Foundation,
//   Inc. ,59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//


#include "optw_away.h"

#include <qlayout.h>

#include "kvi_options.h"
#include "kvi_locale.h"

//#warning "Info tips"


KviAwayOptionsWidget::KviAwayOptionsWidget(QWidget * parent)
: KviOptionsWidget(parent,"away_options_widget")
{
	createLayout(4,1);

	addStringSelector(0,0,0,0, __tr2qs_ctx("Default away message:","options"), KviOption_stringAwayMessage);

	addBoolSelector(0,1,0,1, __tr2qs_ctx("User input exits away mode","options"), KviOption_boolExitAwayOnInput);

	KviTalGroupBox *g = addGroupBox(0,2,0,2,1,Qt::Horizontal,__tr2qs_ctx("Away Nickname","options"));
	m_pEnableAwayNick = addBoolSelector(g,__tr2qs_ctx("Change nickname on away","options"),KviOption_boolChangeNickAway);	
	connect(m_pEnableAwayNick,SIGNAL(toggled(bool)),this,SLOT(enableCustomAwayText(bool)));
	
	m_pEnableCustomAwayNick = addBoolSelector(g,__tr2qs_ctx("Use automatic nickname ([5 letters]AWAY)","options"),KviOption_boolAutoGeneratedAwayNick,KVI_OPTION_BOOL(KviOption_boolChangeNickAway));
	connect(m_pEnableAwayNick,SIGNAL(toggled(bool)),m_pEnableCustomAwayNick,SLOT(setEnabled(bool)));
	connect(m_pEnableCustomAwayNick,SIGNAL(toggled(bool)),this,SLOT(enableCustomAwayText(bool)));
	
	// m_p <-- stands for pointer
	// m_sz <-- stands for a string :)
//	m_szCustomAwayNick = addStringSelector(0,4,0,4,__tr2qs_ctx("Custom away nick","options"),KviOption_stringCustomAwayNick,KVI_OPTION_BOOL(KviOption_boolChangeNickAway) && !(KVI_OPTION_BOOL(KviOption_boolAutoGeneratedAwayNick)));

	m_pCustomAwayNick = addStringSelector(g,__tr2qs_ctx("Custom nickname (%%nick%% means a current nick):","options"),KviOption_stringCustomAwayNick,KVI_OPTION_BOOL(KviOption_boolChangeNickAway) && !(KVI_OPTION_BOOL(KviOption_boolAutoGeneratedAwayNick)));

	addRowSpacer(0,3,0,3);
}

KviAwayOptionsWidget::~KviAwayOptionsWidget()
{
}

void KviAwayOptionsWidget::enableCustomAwayText(bool) 
{
	m_pCustomAwayNick->setEnabled(m_pEnableAwayNick->isChecked() && !(m_pEnableCustomAwayNick->isChecked()));

}


#ifndef COMPILE_USE_STANDALONE_MOC_SOURCES
#include "m_optw_away.moc"
#endif //!COMPILE_USE_STANDALONE_MOC_SOURCES
