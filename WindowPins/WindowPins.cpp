#include "WindowPins.h"
#include <QCloseEvent>

// Define and initialize static member variable
HWND WindowPins::targetWindow = NULL; // Initialize to NULL (or nullptr in C++)

WindowPins::WindowPins(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    startButton = new QPushButton("Start", this);
    startButton->setToolTip("Right click a window to make it always top after clicking Start button");
    stopButton = new QPushButton("Stop", this);
    minimizingCheckBox = new QCheckBox("Minimizing to tray", this);
    minimizingCheckBox->setChecked(true);

    //Layout
    centralWidget = new QWidget(this);
    functionLayout = new QHBoxLayout();
    mainLayout = new QVBoxLayout();

    //Add components to layout
    functionLayout->addWidget(startButton);
    functionLayout->addWidget(stopButton);

    mainLayout->addLayout(functionLayout);
    mainLayout->addWidget(minimizingCheckBox);

    centralWidget->setLayout(mainLayout);

    //Create icon of tray
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/WindowPins/Icon/Trigger.png"));    // Set Tray Icon
    trayIcon->setToolTip("WindowPins Application");    // Set tray tips

    //Create menu of tray
    trayMenu = new QMenu(this);
    restoreAction = new QAction("Restore", this);
    quitAction = new QAction("Quit", this);

    trayMenu->addAction(restoreAction);
    trayMenu->addSeparator();
    trayMenu->addAction(quitAction);

    trayIcon->setContextMenu(trayMenu);    // Set the right-click menu

    //Connecting signal and slot
    connect(startButton, &QPushButton::clicked, this, &WindowPins::startOnTop);
    connect(stopButton, &QPushButton::clicked, this, &WindowPins::stopOnTop);
    
    connect(restoreAction, &QAction::triggered, this, &WindowPins::restoreWindow);
    connect(quitAction, &QAction::triggered, this, &WindowPins::quitApplication);
    connect(trayIcon, &QSystemTrayIcon::activated, this, [=](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::Trigger)
        {
            restoreWindow();
        }
        });

    trayIcon->show();
}

WindowPins::~WindowPins()
{}

void WindowPins::closeEvent(QCloseEvent * event)
{
    //check whether checkbox is checked
    bool isChecked = minimizingCheckBox->isChecked();

    //Intercept the close eventand minimize to the system tray
    if (trayIcon->isVisible() && isChecked)
    {
        hide();
        trayIcon->showMessage("Application Minimized",
            "The application is still running in the system tray.",
            QSystemTrayIcon::Information, 3000);  // Display a notification
        event->ignore();  // Ignore the close event to prevent the window from closing

        trayIcon->show();
        return;
    }

    stopOnTop();
}

void WindowPins::setAlwaysOnTop(HWND hwnd, bool alwaysOnTop, bool nonInteractive)
{
    if (alwaysOnTop)
    {
        SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

        if (nonInteractive)
        {
            LONG style = GetWindowLong(hwnd, GWL_EXSTYLE);
            style |= (WS_EX_TRANSPARENT | WS_EX_LAYERED);
            SetWindowLong(hwnd, GWL_EXSTYLE, style);
        }
    }
    else
    {
        SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

        // Remove WS_EX_TRANSPARENT and WS_EX_LAYERED if they were set
        LONG style = GetWindowLong(hwnd, GWL_EXSTYLE);
        style &= ~(WS_EX_TRANSPARENT | WS_EX_LAYERED);
        SetWindowLong(hwnd, GWL_EXSTYLE, style);
    }
}

LRESULT WindowPins::mouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION && wParam == WM_LBUTTONDOWN)
    {
        // Get cursor position
        POINT cursorPos;
        GetCursorPos(&cursorPos);

        // Get the window under the cursor
        targetWindow = WindowFromPoint(cursorPos);
        if (targetWindow)
        {
            setAlwaysOnTop(targetWindow, true, true);
            PostQuitMessage(0);   // Send WM_QUIT to stop the message loop
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void WindowPins::startOnTop()
{
    //Stop the previous on top window
    stopOnTop();

    // Install mouse hook
    HHOOK mouseHook = SetWindowsHookEx(WH_MOUSE_LL, mouseHookProc, NULL, 0);

    // Message loop
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {

    }

    // Uninstall mouse hook
    UnhookWindowsHookEx(mouseHook);
}

void WindowPins::stopOnTop()
{
    setAlwaysOnTop(targetWindow, false);
}

void WindowPins::restoreWindow()
{
    showNormal();       // Restore the window
    raise();    // Raise the window (display on top of other windows)
    activateWindow();    // Activate the window
}

void WindowPins::quitApplication()
{
    stopOnTop();
    //trayIcon->hide();  // Hide the tray icon (Without this sentence is also working)
    QApplication::quit();  // Exit the application
}
