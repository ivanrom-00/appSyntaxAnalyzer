#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QTextStream>

// Clases utilizadas (POO):
#include "filemanager.h"
#include "analizadorlexico.h"
#include "analizadorsintactico.h"

#define MAXTOKEN 255 // --> Ya no se usa

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionSalir_triggered();

    void on_actionAbrir_triggered();

    void on_actionGuardar_triggered();

    void on_btnAnalizar_clicked();

    void on_actionReiniciar_triggered();

private:
    Ui::MainWindow *ui;
    void borrarTablas();
};
#endif // MAINWINDOW_H
