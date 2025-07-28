#include <QApplication>
#include <QMessageBox>
#include <QSettings>
#include "views/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    app.setApplicationName("Vehicle Speed Checkout");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("VehicleSpeedCheckout");

    QSettings settings;
    if (!settings.contains("lgpl_notice_shown")) {
        QMessageBox::information(nullptr, "Qt LGPL License Notice",
            "This application uses Qt libraries under the GNU Lesser General Public License v. 3 (LGPL v3).\n\n"
            "You have the right to:\n"
            "• Run this program for any purpose\n"
            "• Study how the program works and adapt it\n"
            "• Redistribute copies\n"
            "• Improve the program and release improvements\n\n"
            "For complete license information, visit:\n"
            "https://www.qt.io/licensing/open-source-lgpl-obligations");
        settings.setValue("lgpl_notice_shown", true);
    }

    MainWindow mainWindow;
    mainWindow.show();
    
    return app.exec();
} 