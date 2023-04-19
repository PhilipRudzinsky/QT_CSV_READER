#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QTextStream>
#include <QLineEdit>
#include <QTableWidget>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &MainWindow::on_lineEdit_textChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString fileName;

void MainWindow::on_pushButton_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, tr("Open CSV File"), "", tr("CSV Files (*.csv)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QStringList headers = in.readLine().split(",");
            ui->tableWidget->setColumnCount(headers.size());
            ui->tableWidget->setHorizontalHeaderLabels(headers);
            while (!in.atEnd()) {
                QString line = in.readLine();
                QStringList fields = line.split(",");
                int row = ui->tableWidget->rowCount();
                ui->tableWidget->insertRow(row);
                for(int i = 0; i < fields.size(); ++i){
                    QTableWidgetItem *item = new QTableWidgetItem(fields[i]);
                    ui->tableWidget->setItem(row, i, item);
                }
            }
            file.close();
        }
    }
}

void MainWindow::on_lineEdit_textChanged(const QString &text)
{
    if(text.isEmpty()){
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QStringList headers = in.readLine().split(",");
            ui->tableWidget->setColumnCount(headers.size());
            ui->tableWidget->setHorizontalHeaderLabels(headers);
            while (!in.atEnd()) {
                QString line = in.readLine();
                QString sp = ui->comboBox->currentText();
                QStringList fields = line.split(sp);
                int row = ui->tableWidget->rowCount();
                ui->tableWidget->insertRow(row);
                for(int i = 0; i < fields.size(); ++i){
                    QTableWidgetItem *item = new QTableWidgetItem(fields[i]);
                    ui->tableWidget->setItem(row, i, item);
                }
            }
            file.close();
        }
    } else {
        QStringList items;
        for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
            bool match = false;
            for(int j = 0; j < ui->tableWidget->columnCount(); ++j){
                QTableWidgetItem *item = ui->tableWidget->item(i, j);
                if (item->text().contains(text, Qt::CaseInsensitive)) {
                    match = true;
                    break;
                }
            }
            if(match){
                ui->tableWidget->showRow(i);
            }else{
                ui->tableWidget->hideRow(i);
            }
        }
    }
}
