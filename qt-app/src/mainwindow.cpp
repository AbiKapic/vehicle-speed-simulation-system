#include "views/MainWindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Vehicle Speed Checkout");
    setMinimumSize(800, 600);
    
  
    createMenuBar();
    
   
    setCentralWidget(new QWidget());
}

void MainWindow::createMenuBar()
{
    QMenuBar *menuBar = this->menuBar();
    
   
    QMenu *helpMenu = menuBar->addMenu("&Help");
    
  
    QAction *aboutAction = helpMenu->addAction("&About");
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAboutDialog);
    
    QAction *lgplAction = helpMenu->addAction("&LGPL License Information");
    connect(lgplAction, &QAction::triggered, this, &MainWindow::showLGPLInfo);
}

void MainWindow::showAboutDialog()
{
    QMessageBox::about(this, "About Vehicle Speed Checkout",
        "<h3>Vehicle Speed Checkout v1.0.0</h3>"
        "<p>A professional Qt-based application for vehicle speed monitoring and analysis.</p>"
        "<p><b>License:</b> MIT License</p>"
        "<p><b>Qt Version:</b> 6.9.1</p>"
        "<p><b>Qt License:</b> LGPL v3</p>"
        "<p>This application uses Qt libraries under the GNU Lesser General Public License v. 3 (LGPL v3).</p>"
        "<p>For complete license information, see Help → LGPL License Information</p>");
}

void MainWindow::showLGPLInfo()
{
    QString lgplText;
    
   
    QFile file(":/QT_LGPL_NOTICE.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        lgplText = in.readAll();
        file.close();
    } else {
       
        lgplText = "Qt LGPL v3 License Notice\n"
                  "========================\n\n"
                  "This application uses Qt libraries under the GNU Lesser General Public License v. 3 (LGPL v3).\n\n"
                  "USER RIGHTS UNDER LGPL v3:\n"
                  "==========================\n\n"
                  "You have the following rights regarding this application and the Qt libraries it uses:\n\n"
                  "1. FREEDOM TO RUN: You may run this program for any purpose.\n"
                  "2. FREEDOM TO STUDY: You may study how the program works and adapt it to your needs.\n"
                  "3. FREEDOM TO REDISTRIBUTE: You may redistribute copies of this program.\n"
                  "4. FREEDOM TO IMPROVE: You may improve the program and release your improvements to the public.\n\n"
                  "For complete Qt LGPL compliance information, visit:\n"
                  "https://www.qt.io/licensing/open-source-lgpl-obligations\n\n"
                  "For the complete LGPL v3 license text, visit:\n"
                  "https://www.gnu.org/licenses/lgpl-3.0.html";
    }
    
    QMessageBox::information(this, "Qt LGPL v3 License Information", lgplText);
}

MainWindow::~MainWindow()
{
}
