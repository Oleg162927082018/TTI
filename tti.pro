VERSION = 2.1.0.0
QMAKE_TARGET_PRODUCT = Total Test Integrator
QMAKE_TARGET_DESCRIPTION = Test-case integrator
QMAKE_TARGET_COPYRIGHT = Copyright\xA9 Oleg Kopyev 2016-2017

QT       += core gui xml help

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tti
TEMPLATE = app

SOURCES += main.cpp\
    GUI/MainWindow/View/mainwindow.cpp \
    Common/DBManager/dbmanager.cpp \
    Common/DllManager/dllmanager.cpp \
    Common/RunManager/runmanager.cpp \
    Common/RunManager/testexecutor.cpp \
    GUI/MainWindow/Controller/test-tree-adapter.cpp \
    GUI/MainWindow/Controller/test-table-adapter.cpp \
    GUI/MainWindow/Model/mainwindow-model.cpp \
    GUI/TestCase/Create/create-testcase-dialog.cpp \
    GUI/TestCase/Create/create-testcase-model.cpp \
    GUI/TestCase/Create/create-testcase-configuration-one-widget.cpp \
    GUI/TestCase/Create/create-testcase-configuration-list-widget.cpp \
    GUI/TestCase/Open/View/open-testcase-dialog.cpp \
    GUI/Run/PlanDispatcher/task-table-adapter.cpp \
    GUI/Run/PlanDispatcher/plan-add-dialog.cpp \
    GUI/Run/PlanDispatcher/plan-edit-dialog.cpp \
    GUI/Run/PlanDispatcher/plan-dispatcher-dialog.cpp \
    GUI/Run/PlanDispatcher/plan-new-dialog.cpp \
    GUI/Run/PlanDispatcher/log-adapter.cpp \
    GlobalConfig/config.cpp \
    GUI/Run/Common/RunDescription/View/run-description-table-widget.cpp \
    GUI/Run/Common/RunDescription/Controller/run-description-table-widget-controller.cpp \
    GUI/Run/RunDescription-Show/View/rundescription-show-dialog.cpp \
    GUI/TestCase/Open/Model/open-testcase-model.cpp \
    GUI/TestCase/Open/Controller/open-test-case-list-controller.cpp \
    TagManager/tagmanager.cpp \
    GUI/Tag/Dispatcher/Controller/tag-collection-tree-adapter.cpp \
    GUI/Tag/Create/View/tag-create-dialog.cpp \
    GUI/Tag/Edit/View/tag-edit-dialog.cpp \
    GUI/Tag/Create/Controller/tag-type-list-controller.cpp \
    GUI/Tag/Create/View/tag-create-empty-widget.cpp \
    GUI/Tag/Dispatcher/View/tag-view-collection-widget.cpp \
    GUI/Tag/Dispatcher/View/tag-view-folder-widget.cpp \
    GUI/Tag/Dispatcher/View/tag-view-unknown-widget.cpp \
    GUI/Tag/Dispatcher/View/tag-dispatcher-dialog.cpp \
    GUI/Tag/View/View/tag-view-dialog.cpp \
    HelpManager/helpdialog.cpp \
    HelpManager/helpmanager.cpp \
    HelpManager/helpbrowser.cpp

HEADERS  += GUI/MainWindow/View/mainwindow.h \
    Common/itestcasetemplate.h \
    Common/DBManager/dbmanager.h \
    Common/DBManager/test-case.h \
    Common/DBManager/test-result.h \
    Common/DBManager/test-status.h \
    Common/DBManager/run-description.h \
    Common/DBManager/test-case-cache.h \
    Common/DllManager/dllmanager.h \
    Common/RunManager/runmanager.h \
    Common/RunManager/testexecutor.h \
    GUI/MainWindow/Controller/test-tree-adapter.h \
    GUI/MainWindow/Controller/test-table-adapter.h \
    GUI/MainWindow/Model/mainwindow-model.h \
    GUI/TestCase/Create/create-testcase-dialog.h \
    GUI/TestCase/Create/create-testcase-model.h \
    GUI/TestCase/Create/create-testcase-configuration-list-widget.h \
    GUI/TestCase/Create/create-testcase-configuration-one-widget.h \
    GUI/TestCase/Open/View/open-testcase-dialog.h \
    GUI/Run/PlanDispatcher/task-table-adapter.h \
    GUI/Run/PlanDispatcher/plan-dispatcher-dialog.h \
    GUI/Run/PlanDispatcher/plan-edit-dialog.h \
    GUI/Run/PlanDispatcher/plan-add-dialog.h \
    GUI/Run/PlanDispatcher/plan-new-dialog.h \
    GUI/Run/PlanDispatcher/log-adapter.h \
    GlobalConfig/config.h \
    GUI/Run/Common/RunDescription/Controller/run-description-table-widget-controller.h \
    GUI/Run/Common/RunDescription/View/run-description-table-widget.h \
    GUI/Run/RunDescription-Show/View/rundescription-show-dialog.h \
    GUI/TestCase/Open/Model/open-testcase-model.h \
    GUI/TestCase/Open/Controller/open-test-case-list-controller.h \
    TagManager/tagmanager.h \
    Common/itagadapter.h \
    GUI/Tag/Dispatcher/Controller/tag-collection-tree-adapter.h \
    Common/DBManager/tag-collection.h \
    GUI/Tag/Create/View/tag-create-dialog.h \
    GUI/Tag/Edit/View/tag-edit-dialog.h \
    GUI/Tag/Create/Controller/tag-type-list-controller.h \
    GUI/Tag/Create/View/tag-create-empty-widget.h \
    GUI/Tag/Dispatcher/View/tag-view-folder-widget.h \
    GUI/Tag/Dispatcher/View/tag-view-collection-widget.h \
    GUI/Tag/Dispatcher/View/tag-view-unknown-widget.h \
    GUI/Tag/Dispatcher/View/tag-dispatcher-dialog.h \
    GUI/Tag/View/View/tag-view-dialog.h \
    HelpManager/helpdialog.h \
    HelpManager/helpmanager.h \
    HelpManager/helpbrowser.h

FORMS    += GUI/MainWindow/View/mainwindow.ui \
    GUI/Run/PlanDispatcher/plan-add-dialog.ui \
    GUI/Run/PlanDispatcher/plan-dispatcher-dialog.ui \
    GUI/Run/PlanDispatcher/plan-edit-dialog.ui \
    GUI/Run/PlanDispatcher/plan-new-dialog.ui \
    GUI/TestCase/Create/create-testcase-dialog.ui \
    GUI/TestCase/Create/create-testcase-configuration-list-widget.ui \
    GUI/TestCase/Create/create-testcase-configuration-one-widget.ui \
    GUI/TestCase/Open/View/open-testcase-dialog.ui \
    GUI/Run/Common/RunDescription/View/run-description-table-widget.ui \
    GUI/Run/RunDescription-Show/View/rundescription-show-dialog.ui \
    GUI/Tag/Create/View/tag-create-dialog.ui \
    GUI/Tag/Edit/View/tag-edit-dialog.ui \
    GUI/Tag/Create/View/tag-create-empty-widget.ui \
    GUI/Tag/Dispatcher/View/tag-view-collection-widget.ui \
    GUI/Tag/Dispatcher/View/tag-view-folder-widget.ui \
    GUI/Tag/Dispatcher/View/tag-view-unknown-widget.ui \
    GUI/Tag/Dispatcher/View/tag-dispatcher-dialog.ui \
    GUI/Tag/View/View/tag-view-dialog.ui \
    HelpManager/helpdialog.ui

RESOURCES +=
