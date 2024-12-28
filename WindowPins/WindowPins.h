#pragma once

#include <QtWidgets/QWidget>
#include "ui_WindowPins.h"
#include <Windows.h>
#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSystemTrayIcon>
#include <QMenu>



class WindowPins : public QWidget
{
    Q_OBJECT

public:
    WindowPins(QWidget *parent = nullptr);
    ~WindowPins();

protected:
    void closeEvent(QCloseEvent* event) override;    // Rewrite close event

private:
    Ui::WindowPinsClass ui;

    //Button for start to choose a window to be always top and stop the same window to be always top
    QPushButton* startButton;
    QPushButton* stopButton;

    //Checkbox for minimize
    QCheckBox* minimizingCheckBox;

    //Layout
    QWidget* centralWidget;
    QHBoxLayout* functionLayout;
    QVBoxLayout* mainLayout;

    QSystemTrayIcon* trayIcon;      // Icon for tray
    QMenu* trayMenu;       // Tray Right-Click Menu
    QAction* restoreAction;    // Restore Window Action
    QAction* quitAction;      // Quit Program Action

private:
    static HWND targetWindow;
    static void setAlwaysOnTop(HWND hwnd, bool alwaysOnTop, bool nonInteractive = false);
    static LRESULT CALLBACK mouseHookProc(int nCode, WPARAM wParam, LPARAM lParam);
    
    void startOnTop();
    void stopOnTop();
    void restoreWindow();
    void quitApplication();
};
