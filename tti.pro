VERSION = 2.4.0.1
QMAKE_TARGET_PRODUCT = Total Test Integrator
QMAKE_TARGET_DESCRIPTION = Test-case integrator
QMAKE_TARGET_COPYRIGHT = Copyright\xA9 Oleg Kopyev 2016-2017

QT       += core gui xml help

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tti
TEMPLATE = app

SOURCES += main.cpp\
    Common/DBManager/dbmanager.cpp \
    Common/DllManager/dllmanager.cpp \
    Common/RunManager/runmanager.cpp \
    Common/RunManager/testexecutor.cpp \
    UI/MainWindow/View/mainwindow.cpp \
    UI/MainWindow/Controller/test-tree-adapter.cpp \
    UI/MainWindow/Controller/test-table-adapter.cpp \
    UI/MainWindow/Model/mainwindow-model.cpp \
    UI/TestCase/Create/Controller/create-testcase-template-adapter.cpp \
    UI/TestCase/Create/View/create-testcase-dialog.cpp \
    UI/TestCase/Create/View/create-testcase-configuration-one-widget.cpp \
    UI/TestCase/Create/View/create-testcase-configuration-list-widget.cpp \
    UI/TestCase/Open/View/open-testcase-dialog.cpp \
    UI/Run/PlanDispatcher-New/View/plan-new-testcase-override-widget.cpp \
    UI/Run/PlanDispatcher-New/Controller/plan-new-testcase-list-adapter.cpp \
    UI/Run/PlanDispatcher-New/View/plan-new-dialog.cpp \
    UI/Run/PlanDispatcher-Add/View/plan-add-dialog.cpp \
    UI/Run/PlanDispatcher-Edit/View/plan-edit-dialog.cpp \
    UI/Run/PlanDispatcher/View/plan-dispatcher-dialog.cpp \
    UI/Run/PlanDispatcher/Controller/task-table-adapter.cpp \
    UI/Run/PlanDispatcher/Controller/log-adapter.cpp \
    UI/TestCase/Open/Model/open-testcase-model.cpp \
    UI/TestCase/Open/Controller/open-test-case-list-controller.cpp \
    UI/Tag/Create/View/tag-create-dialog.cpp \
    UI/Tag/Edit/View/tag-edit-dialog.cpp \
    UI/Tag/Create/Controller/tag-type-list-controller.cpp \
    UI/Tag/Create/View/tag-create-empty-widget.cpp \
    UI/Tag/Dispatcher/View/tag-dispatcher-dialog.cpp \
    UI/Tag/View/View/tag-view-dialog.cpp \
    UI/Search/View/set-filter-dialog.cpp \
    UI/TestCase/Open/Controller/open-testcase-rundescriptions-controller.cpp \
    UI/RunHeader/RunHeadersDialog/Controller/run-headers-table-controller.cpp \
    UI/RunHeader/RunHeadersDialog/View/run-headers-dialog.cpp \
    UI/MainWindow/View/mainwindow-menu.cpp \
    UI/Search/View/set-filter-tag-unknown-widget.cpp \
    UI/Search/View/set-filter-tag-folder-widget.cpp \
    UI/Search/View/set-filter-tag-collection-widget.cpp \
    UI/Search/Model/set-filter-dialog-model.cpp \
    UI/Tag/Dispatcher/Model/tag-dispatcher-model.cpp \
    UI/Tag/Dispatcher/Controller/tag-dispatcher-tree-adapter.cpp \
    UI/Tag/Dispatcher/View/tag-dispatcher-collection-widget.cpp \
    UI/Tag/Dispatcher/View/tag-dispatcher-folder-widget.cpp \
    UI/Tag/Dispatcher/View/tag-dispatcher-unknown-widget.cpp \
    UI/Tag/Create/View/tag-folder-create-dialog.cpp \
    UI/Tag/Create/View/tag-collection-create-dialog.cpp \
    UI/Search/Controller/set-filter-tag-tree-adapter.cpp \
    UI/Search/Controller/set-filter-testcase-list-adapter.cpp \
    UI/Search/Controller/set-filter-status-table-adapter.cpp \
    ConfigManager/config.cpp \
    TestCaseManager/testcasemanager.cpp \
    TagManager/tagmanager.cpp \
    HelpManager/helpdialog.cpp \
    HelpManager/helpmanager.cpp \
    HelpManager/helpbrowser.cpp

HEADERS  += Common/itestcasetemplate.h \
    Common/itagadapter.h \
    Common/DBManager/tag-collection.h \
    Common/DBManager/dbmanager.h \
    Common/DBManager/test-case.h \
    Common/DBManager/test-result.h \
    Common/DBManager/test-status.h \
    Common/DBManager/run-description.h \
    Common/DllManager/dllmanager.h \
    Common/RunManager/runmanager.h \
    Common/RunManager/testexecutor.h \
    ConfigManager/config.h \
    UI/MainWindow/View/mainwindow.h \
    UI/MainWindow/Controller/test-tree-adapter.h \
    UI/MainWindow/Controller/test-table-adapter.h \
    UI/MainWindow/Model/mainwindow-model.h \
    UI/TestCase/Create/Controller/create-testcase-template-adapter.h \
    UI/TestCase/Create/View/create-testcase-dialog.h \
    UI/TestCase/Create/View/create-testcase-configuration-list-widget.h \
    UI/TestCase/Create/View/create-testcase-configuration-one-widget.h \
    UI/TestCase/Open/View/open-testcase-dialog.h \
    UI/Run/PlanDispatcher-New/View/plan-new-testcase-override-widget.h \
    UI/Run/PlanDispatcher-New/Controller/plan-new-testcase-list-adapter.h \
    UI/Run/PlanDispatcher/View/plan-dispatcher-dialog.h \
    UI/Run/PlanDispatcher-Edit/View/plan-edit-dialog.h \
    UI/Run/PlanDispatcher-Add/View/plan-add-dialog.h \
    UI/Run/PlanDispatcher-New/View/plan-new-dialog.h \
    UI/Run/PlanDispatcher/Controller/task-table-adapter.h \
    UI/Run/PlanDispatcher/Controller/log-adapter.h \
    UI/TestCase/Open/Model/open-testcase-model.h \
    UI/TestCase/Open/Controller/open-test-case-list-controller.h \
    UI/Tag/Create/View/tag-create-dialog.h \
    UI/Tag/Edit/View/tag-edit-dialog.h \
    UI/Tag/Create/Controller/tag-type-list-controller.h \
    UI/Tag/Create/View/tag-create-empty-widget.h \
    UI/Tag/Dispatcher/View/tag-dispatcher-dialog.h \
    UI/Tag/View/View/tag-view-dialog.h \
    UI/Search/View/set-filter-dialog.h \
    UI/MainWindow/Model/mainwindow-treefolder.h \
    UI/MainWindow/Model/mainwindow-tableitem.h \
    UI/TestCase/Open/Model/open-testcase-item.h \
    UI/TestCase/Open/Model/open-testcase-runitem.h \
    UI/TestCase/Open/Controller/open-testcase-rundescriptions-controller.h \
    UI/MainWindow/Model/mainwindow-tableheader.h \
    UI/RunHeader/RunHeadersDialog/View/run-headers-dialog.h \
    UI/RunHeader/RunHeadersDialog/Controller/run-headers-table-controller.h \
    UI/RunHeader/RunHeadersDialog/Model/run-header-item.h \
    UI/Search/View/set-filter-tag-unknown-widget.h \
    UI/Search/View/set-filter-tag-folder-widget.h \
    UI/Search/View/set-filter-tag-collection-widget.h \
    UI/Search/Model/set-filter-dialog-model.h \
    UI/Search/Model/set-filter-tag-item.h \
    UI/Tag/Dispatcher/Model/tag-dispatcher-model.h \
    UI/Tag/Dispatcher/Model/tag-dispatcher-item.h \
    UI/Tag/Dispatcher/Controller/tag-dispatcher-tree-adapter.h \
    UI/Tag/Dispatcher/View/tag-dispatcher-collection-widget.h \
    UI/Tag/Dispatcher/View/tag-dispatcher-folder-widget.h \
    UI/Tag/Dispatcher/View/tag-dispatcher-unknown-widget.h \
    UI/Tag/Create/View/tag-folder-create-dialog.h \
    UI/Tag/Create/View/tag-collection-create-dialog.h \
    UI/Search/Controller/set-filter-tag-tree-adapter.h \
    UI/Search/Model/set-filter-status-collection.h \
    UI/Search/Controller/set-filter-testcase-list-adapter.h \
    UI/Search/Controller/set-filter-status-table-adapter.h \
    TagManager/tagmanager.h \
    TestCaseManager/testcasemanager.h \
    HelpManager/helpdialog.h \
    HelpManager/helpmanager.h \
    HelpManager/helpbrowser.h

FORMS    += UI/MainWindow/View/mainwindow.ui \
    UI/Run/PlanDispatcher-New/View/plan-new-testcase-override-widget.ui \
    UI/Run/PlanDispatcher-Add/View/plan-add-dialog.ui \
    UI/Run/PlanDispatcher/View/plan-dispatcher-dialog.ui \
    UI/Run/PlanDispatcher-Edit/View/plan-edit-dialog.ui \
    UI/Run/PlanDispatcher-New/View/plan-new-dialog.ui \
    UI/TestCase/Create/View/create-testcase-dialog.ui \
    UI/TestCase/Create/View/create-testcase-configuration-list-widget.ui \
    UI/TestCase/Create/View/create-testcase-configuration-one-widget.ui \
    UI/TestCase/Open/View/open-testcase-dialog.ui \
    UI/Tag/Create/View/tag-create-dialog.ui \
    UI/Tag/Edit/View/tag-edit-dialog.ui \
    UI/Tag/Create/View/tag-create-empty-widget.ui \
    UI/Tag/Dispatcher/View/tag-dispatcher-dialog.ui \
    UI/Tag/View/View/tag-view-dialog.ui \
    UI/Search/View/set-filter-dialog.ui \
    UI/RunHeader/RunHeadersDialog/View/run-headers-dialog.ui \
    UI/Search/View/set-filter-tag-unknown-widget.ui \
    UI/Search/View/set-filter-tag-folder-widget.ui \
    UI/Search/View/set-filter-tag-collection-widget.ui \
    UI/Tag/Dispatcher/View/tag-dispatcher-collection-widget.ui \
    UI/Tag/Dispatcher/View/tag-dispatcher-folder-widget.ui \
    UI/Tag/Dispatcher/View/tag-dispatcher-unknown-widget.ui \
    UI/Tag/Create/View/tag-folder-create-dialog.ui \
    UI/Tag/Create/View/tag-collection-create-dialog.ui \
    HelpManager/helpdialog.ui

RESOURCES +=

DISTFILES += \
    FAQ/Others.txt \
    toDo.txt \
    FAQ/Containers.txt \
    FAQ/Cast.txt \
    FAQ/Tags.txt
