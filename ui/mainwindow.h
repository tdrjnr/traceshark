// SPDX-License-Identifier: (GPL-2.0-or-later OR BSD-2-Clause)
/*
 * Traceshark - a visualizer for visualizing ftrace and perf traces
 * Copyright (C) 2015-2019  Viktor Rosendahl <viktor.rosendahl@gmail.com>
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QHBoxLayout>
#include <QMainWindow>
#include <QVector>
#include <QString>
#include "analyzer/traceanalyzer.h"
#include "misc/setting.h"
#include "misc/traceshark.h"
#include "parser/traceevent.h"
#include "threads/workitem.h"

QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
class QMenu;
class QPlainTextEdit;
class QMouseEvent;
class QToolBar;
template<class T, class U> class QMap;
QT_END_NAMESPACE

class TraceAnalyzer;
class EventsWidget;
class InfoWidget;
class Cursor;
class CPUTask;
class ErrorDialog;
class GraphEnableDialog;
class LicenseDialog;
class EventInfoDialog;
class QCPAbstractPlottable;
class QCPGraph;
class QCPLayer;
class QCPLegend;
class QCustomPlot;
class QCPAbstractLegendItem;
class TaskToolBar;
class TracePlot;
class TraceEvent;
class TaskRangeAllocator;
class TaskSelectDialog;
class EventSelectDialog;
class YAxisTicker;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();
	virtual ~MainWindow();
	void openFile(const QString &name);
protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void openTrace();
	void closeTrace();
	void saveScreenshot();
	void about();
	void aboutQCustomPlot();
	void license();
	void mouseWheel();
	void mousePress();
	void plotDoubleClicked(QMouseEvent *event);
	void infoValueChanged(vtl::Time value, int nr);
	void moveActiveCursor(vtl::Time time);
	void showEventInfo(const TraceEvent &event);
	void taskTriggered(int pid);
	void handleEventSelected(const TraceEvent *event);
	void selectionChanged();
	void legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem
			       *abstractItem);
	void legendEmptyChanged(bool empty);
	void addTaskGraph(int pid);
	void doReplot();
	void addTaskToLegend(int pid);
	void removeTaskGraph(int pid);
	void cursorZoom();
	void defaultZoom();
	void showTaskSelector();
	void showEventFilter();
	void showGraphEnable();
	void showWakeupOrWaking(int pid, event_t wakevent);
	void showWaking(const TraceEvent *event);
	void createPidFilter(QMap<int, int> &map,
			     bool orlogic, bool inclusive);
	void createEventFilter(QMap<event_t, event_t> &map, bool orlogic);
	void resetPidFilter();
	void resetEventFilter();
	void resetFilters();
	void timeFilter();
	void exportEvents(TraceAnalyzer::exporttype_t export_type);
	void exportEventsTriggered();
	void exportCPUTriggered();
	void consumeSettings();
	void showStats();
	void showStatsTimeLimited();
	void removeQDockWidget(QDockWidget *widget);
	void taskFilter();

	void addTaskGraphTriggered();
	void addToLegendTriggered();
	void clearLegendTriggered();
	void findSleepTriggered();
	void findWakeupTriggered();
	void findWakingTriggered();
	void findWakingDirectTriggered();
	void removeTaskGraphTriggered();
	void clearTaskGraphsTriggered();
	void taskFilterTriggered();
	void taskFilterLimitedTriggered();

private:
	typedef enum {
		STATUS_NOFILE = 0,
		STATUS_FILE,
		STATUS_ERROR,
		STATUS_NR
	} status_t;

	void processTrace();
	void computeLayout();
	void computeStats();
	void rescaleTrace();
	void clearPlot();
	void showTrace();
	void loadSettings();
	void setupCursors();
	void setupCursors(const double &red, const double &blue);
	void setupCursors(const vtl::Time &redtime, const vtl::Time &bluetime);
	void _setupCursors(vtl::Time redtime, const double &red,
			   vtl::Time bluetime, const double &blue);

	void updateResetFiltersEnabled();

	void addSchedGraph(CPUTask &task);
	void addHorizontalWakeupGraph(CPUTask &task);
	void addWakeupGraph(CPUTask &task);
	void addPreemptedGraph(CPUTask &task);
	void addStillRunningGraph(CPUTask &task);
	void addUninterruptibleGraph(CPUTask &task);
	void addGenericAccessoryGraph(const QString &name,
				      const QVector<double> &timev,
				      const QVector<double> &scaledData,
				      QCPScatterStyle::ScatterShape sshape,
				      double size,
				      const QColor &color);
	void addAccessoryTaskGraph(QCPGraph **graphPtr, const QString &name,
				   const QVector<double> &timev,
				   const QVector<double> &scaledData,
				   QCPScatterStyle::ScatterShape sshape,
				   double size, const QColor &color);
	void addStillRunningTaskGraph(Task *task);
	void addPreemptedTaskGraph(Task *task);
	void addUninterruptibleTaskGraph(Task *task);

	void setTraceActionsEnabled(bool e);
	void setLegendActionsEnabled(bool e);
	void setCloseActionsEnabled(bool e);
	void setTaskActionsEnabled(bool e);
	void setWakeupActionsEnabled(bool e);
	void setAddTaskGraphActionEnabled(bool e);
	void setTaskGraphRemovalActionEnabled(bool e);
	void setTaskGraphClearActionEnabled(bool e);
	void setEventsWidgetEvents();
	void scrollTo(const vtl::Time &time);
	void handleLegendGraphDoubleClick(QCPGraph *legendGraph);

	void handleWakeUpChanged(bool selected);

	void checkStatsTimeLimited();

	bool selectQCPGraph(QCPGraph *graph);
	void selectTaskByPid(int pid, const unsigned int *preferred_cpu);
	bool isOpenGLEnabled();
	void setupOpenGL();
	void updateTaskGraphActions();

	TracePlot *tracePlot;
	YAxisTicker *yaxisTicker;
	TaskRangeAllocator *taskRangeAllocator;
	QCPLayer *cursorLayer;
	QWidget *plotWidget;
	QVBoxLayout *plotLayout;
	EventsWidget *eventsWidget;
	InfoWidget *infoWidget;
	QString traceFile;

	void createActions();
	void createToolBars();
	void createMenus();
	void createTracePlot();
	void createStatusBar();
	void createDialogs();
	void plotConnections();
	void widgetConnections();
	void dialogConnections();

	void setStatus(status_t status, const QString *fileName = nullptr);
	int loadTraceFile(const QString &);

	QMenu *fileMenu;
	QMenu *viewMenu;
	QMenu *helpMenu;
	QMenu *taskMenu;

	QToolBar *fileToolBar;
	QToolBar *viewToolBar;
	TaskToolBar *taskToolBar;

	QLabel *statusLabel;
	QString *statusStrings[STATUS_NR];

	QAction *openAction;
	QAction *closeAction;
	QAction *saveAction;
	QAction *exitAction;
	QAction *cursorZoomAction;
	QAction *defaultZoomAction;
	QAction *showTasksAction;
	QAction *showEventsAction;
	QAction *timeFilterAction;
	QAction *graphEnableAction;
	QAction *resetFiltersAction;
	QAction *exportEventsAction;
	QAction *exportCPUAction;
	QAction *showStatsAction;
	QAction *showStatsTimeLimitedAction;
	QAction *aboutAction;
	QAction *licenseAction;
	QAction *aboutQtAction;
	QAction *aboutQCPAction;

	QAction *addTaskGraphAction;
	QAction *addToLegendAction;
	QAction *clearLegendAction;
	QAction *findSleepAction;
	QAction *findWakeupAction;
	QAction *findWakingAction;
	QAction *findWakingDirectAction;
	QAction *removeTaskGraphAction;
	QAction *clearTaskGraphsAction;
	QAction *taskFilterAction;
	QAction *taskFilterLimitedAction;

	TraceAnalyzer *analyzer;

	ErrorDialog *errorDialog;
	LicenseDialog *licenseDialog;
	EventInfoDialog *eventInfoDialog;
	TaskSelectDialog *taskSelectDialog;
	TaskSelectDialog *statsDialog;
	TaskSelectDialog *statsLimitedDialog;
	EventSelectDialog *eventSelectDialog;
	GraphEnableDialog *graphEnableDialog;

	static const double bugWorkAroundOffset;
	static const double schedSectionOffset;
	static const double schedSpacing;
	static const double schedHeight;
	static const double cpuSectionOffset;
	static const double cpuSpacing;
	static const double cpuHeight;
	/*
	 * const double migrateHeight doesn't exist. The value used is the
	 * dynamically calculated inc variable in MainWindow::computeLayout()
	 */

	static const double migrateSectionOffset;

	static const QString RUNNING_NAME;
	static const QString PREEMPTED_NAME;
	static const QString UNINT_NAME;

	static const double RUNNING_SIZE;
	static const double PREEMPTED_SIZE;
	static const double UNINT_SIZE;

	static const QCPScatterStyle::ScatterShape RUNNING_SHAPE;
	static const QCPScatterStyle::ScatterShape PREEMPTED_SHAPE;
	static const QCPScatterStyle::ScatterShape UNINT_SHAPE;

	static const QColor RUNNING_COLOR;
	static const QColor PREEMPTED_COLOR;
	static const QColor UNINT_COLOR;

	double bottom;
	double top;
	QVector<double> ticks;
	QVector<QString> tickLabels;
	Cursor *cursors[TShark::NR_CURSORS];
	Setting settings[Setting::NR_SETTINGS];
	bool filterActive;
	double cursorPos[TShark::NR_CURSORS];
};

#endif /* MAINWINDOW_H */
