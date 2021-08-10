#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Borrar label de la sintaxis
    ui->lblResultado->setText("");

    // --------------------------------------------

    // Preparar tabla del Léxico
    // Columnas: Valor | Token

    ui->tableL->setColumnCount(2);
    ui->tableL->setFont(QFont(QString("Lato"),16));
    QStringList titulosL;
    titulosL << "Valor" << "Token";
    ui->tableL->setHorizontalHeaderLabels(titulosL);

    ui->tableL->setColumnWidth(0,130);
    ui->tableL->setColumnWidth(1,130);

    // --------------------------------------------

    // Preparar tabla del Sintáctico
    // Columnas: Variable | Producción

    ui->tableS->setColumnCount(2);
    ui->tableS->setFont(QFont(QString("Lato"),16));
    QStringList titulosS;
    titulosS << "Variable" << "Producción";
    ui->tableS->setHorizontalHeaderLabels(titulosS);

    ui->tableS->setColumnWidth(0,120);
    ui->tableS->setColumnWidth(1,120);

    // --------------------------------------------
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionSalir_triggered()
{
    exit(0); // --> Salir de la aplicación
}

void MainWindow::on_actionAbrir_triggered()
{

    // Abrir un archivo con QFileDialog

    QString nombreArchivo, entrada;

    QFileDialog dialogo(this);
    dialogo.setNameFilter("Archivos de texto");
    dialogo.setViewMode(QFileDialog::Detail);

    nombreArchivo = QFileDialog::getOpenFileName(this, "Abrir archivo",
                                                  "C:\\", "Archivos de texto(*.txt)");

    QFile archivo(nombreArchivo);

    if(nombreArchivo.isEmpty())
        return;

    if (!archivo.open(QIODevice::ReadWrite | QIODevice::Text))
        return;

    QTextStream contenido(&archivo);

    entrada = contenido.readAll();

    archivo.close();

    // Llenar el textEdit con el contenido archivo
    ui->textEdit->clear();
    ui->textEdit->setPlainText(entrada);

    nombreArchivo = "";
    entrada = "";

    ui->lblResultado->setText("");
}

void MainWindow::on_actionGuardar_triggered()
{
    // Guardar contenido del textEdit en un archivo
    // usando QFileDialog

    QString contenido = ui->textEdit->toPlainText();
    QFileDialog saveDialog;
    QFile archivo(saveDialog.getSaveFileName(this, "Guardar archivo",
                                             "C://", "Archivos de texto(*.txt)"));
    archivo.open(QIODevice::WriteOnly);
    archivo.write(contenido.toUtf8());

    ui->textEdit->setFocus();
}

void MainWindow::on_btnAnalizar_clicked()
{

    // Ejecución del análisis Léxico y Sintáctico

    borrarTablas();

    // Obtener el contenido del textEdit
    // Convertirlo a arreglo de char para usar los analizadores
    QString contenido = ui->textEdit->toPlainText();
    QByteArray ba = contenido.toLocal8Bit();
    char *cadena = ba.data(); // --> Contenido a analizar

    // Crear FileManager
    // Generar y abrir el archivo con la cadena
    FileManager *FL = new FileManager();
    FL->generarArchivo(cadena);
    FL->abrirArchivo();

    // ------------------------------------------------------------------------------

    // ANALIZADOR LÉXICO

    // Crear el Analizador Léxico con el archivo del FileManager
    // Iniciar el analizador indicando los bytes del archivo
    AnalizadorLexico *AL = new AnalizadorLexico(FL->getFd());
    AL->analizador(FL->bytesarch());

    QStringList tokensList = AL->getTokenResult(); // --> Obtener lista de tokens
    QStringList valoresList = AL->getValoresResult(); // --> Obtener lista de valores

    // Llenar la tabla del Léxico:

    for(int i = 0; i< tokensList.size(); i++){
        ui->tableL->insertRow(ui->tableL->rowCount());
        ui->tableL->setItem(ui->tableL->rowCount() - 1,
                            1,new QTableWidgetItem(tokensList[i]));
        ui->tableL->setItem(ui->tableL->rowCount() - 1,
                            0, new QTableWidgetItem(valoresList[i]));
    }

    // ------------------------------------------------------------------------------

    // ANALIZADOR SINTÁCTICO

    // Crear el analizador sintáctico
    AnalizadorSintactico *AS = new AnalizadorSintactico();

    // Iniciar el análisis y asignar el resultado a una bandera
    // (el análisis indica si la sintaxis es válida o no)
    bool flag = AS->analizador(tokensList);

    QStringList variables = AS->getVariables(); // --> Obtener lista de variables
    QStringList producciones = AS->getProducciones(); // --> Obtener lista de producciones

    // Llenar la tabla del sintáctico

    for(int i = 0; i< variables.size(); i++){
        ui->tableS->insertRow(ui->tableS->rowCount());
        ui->tableS->setItem(ui->tableS->rowCount() - 1,
                            0,new QTableWidgetItem(variables[i]));
        ui->tableS->setItem(ui->tableS->rowCount() - 1,
                            1, new QTableWidgetItem(producciones[i]));
    }

    // Indicar si la sintaxis es válida o no según la bandera

    if (flag)
        ui->lblResultado->setText("SINTAXIS VÁLIDA");
    else
        ui->lblResultado->setText("ERROR DE SINTAXIS");

    // ------------------------------------------------------------------------------
}

void MainWindow::borrarTablas()
{
    // Limpiar las tablas
    ui->tableL->setRowCount(0);
    ui->tableS->setRowCount(0);
}

void MainWindow::on_actionReiniciar_triggered()
{
    // Limpiar textEdit y tablas
    ui->textEdit->clear();
    ui->textEdit->setFocus();

    borrarTablas();

    ui->lblResultado->setText("");
}
