#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include "GameView.h"
#include "ControlPanel.h"
#include "../core/GameEngine.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void showAboutDialog();
    void showLGPLInfo();
    void onGameStarted();
    void onGameStopped();
    void onVehicleOffRoad();

private:
    void createMenuBar();
    void createCentralWidget();
    void setupGameEngine();
    void setupConnections();
    
    
    QWidget *m_centralWidget;
    QSplitter *m_mainSplitter;
    GameView *m_gameView;
    ControlPanel *m_controlPanel;
    
    
    GameEngine *m_gameEngine;
    
    
    QAction *m_startAction;
    QAction *m_stopAction;
    QAction *m_pauseAction;
    QAction *m_resetAction;
};

#endif 
