// SPDX-License-Identifier: (GPL-2.0-or-later OR BSD-2-Clause)
/*
 * Traceshark - a visualizer for visualizing ftrace and perf traces
 * Copyright (C) 2015, 2018, 2019  Viktor Rosendahl <viktor.rosendahl@gmail.com>
 *
 * This file is dual licensed: you can use it either under the terms of
 * the GPL, or the BSD license, at your option.
 *
 *  a) This program is free software; you can redistribute it and/or
 *     modify it under the terms of the GNU General Public License as
 *     published by the Free Software Foundation; either version 2 of the
 *     License, or (at your option) any later version.
 *
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public
 *     License along with this library; if not, write to the Free
 *     Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
 *     MA 02110-1301 USA
 *
 * Alternatively,
 *
 *  b) Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *     1. Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *     2. Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 *     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *     CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *     INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *     MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 *     CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *     SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *     NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *     HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *     CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 *     OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 *     EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SETTING_H
#define SETTING_H

#include <QString>
#include <QMap>

QT_BEGIN_NAMESPACE
class QTextStream;
QT_END_NAMESPACE

#define TS_SETTING_FILENAME ".traceshark"

class SettingDependency
{
public:
	int index;
	bool desiredValue;
};

class Setting
{
public:
	Setting();
	enum SettingIndex : int {
		SHOW_SCHED_GRAPHS = 0,
		HORIZONTAL_WAKEUP,
		VERTICAL_WAKEUP,
		SHOW_CPUFREQ_GRAPHS,
		SHOW_CPUIDLE_GRAPHS,
		SHOW_MIGRATION_GRAPHS,
		SHOW_MIGRATION_UNLIMITED,
		NR_SETTINGS,
		/* These are not regular settings but must have unique values */
		OPENGL_ENABLED,
		LINE_WIDTH,
		END_SETTINGS,
	};
	static void setupSettings();
	static bool isWideScreen();
	static bool isLowResScreen();
	static void setEnabled(enum SettingIndex idx, bool e);
	static void clearDependencies(enum SettingIndex idx);
	static unsigned int getNrDependencies(enum SettingIndex idx);
	static unsigned int getNrDependents(enum SettingIndex idx);
	static const QString &getName(enum SettingIndex idx);
	static bool isEnabled(enum SettingIndex idx);
	static const SettingDependency &getDependency(enum SettingIndex idx,
						      unsigned int nr);
	static const SettingDependency &getDependent(enum SettingIndex idx,
						     unsigned int nr);
	static void setLineWidth(int width);
	static int getLineWidth();
	static void setOpenGLEnabled(bool e);
	static bool isOpenGLEnabled();
	static int loadSettings();
	static int saveSettings();
	static const QString &getFileName();
private:
	static void setName(enum SettingIndex idx, const QString &n);
	static void setKey(enum SettingIndex idx, const QString &key);
	static void addDependency(enum SettingIndex idx,
				  const SettingDependency &d);
	static void setOpenGLEnabledKey(const QString &key);
	static void setLineWidthKey(const QString &key);
	static int readKeyValuePair(QTextStream &stream, QString &key,
				    QString &value);
	static bool boolFromValue(bool *ok, const QString &value);
	static bool isIrregularIndex(enum SettingIndex idx);
	static bool isRegularIndex(enum SettingIndex idx);
	static void handleIrregularIndex(enum SettingIndex idx,
					 const QString &value);
	static void handleRegularIndex(enum SettingIndex idx,
				       const QString &value);
	static int handleOlderVersion(int oldver, int newver);
	static const QString &boolToQString(bool b);
	bool enabled;
	QString name;
	SettingDependency dependency[4];
	SettingDependency dependent[4];
	unsigned int nrDep;
	unsigned int nrDependents;
	static Setting settings[];
	static int line_width;
	static bool opengl;
	static QMap<QString, enum SettingIndex> fileKeyMap;
	static const int this_version;
};

#endif /* SETTING_H */
