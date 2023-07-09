#include "mainwindow.h"
#include "avl.h"
#include "blockchain.h"
#include "date.h"
#include "patricia_trie.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QString>
#include <QStringList>

#include <iostream>
#include <QDebug>
#include "chainhash.h"
#include <streambuf>
#include <sstream>



using namespace std;
//#include "funcionamiento_structs.h"


BlockChain<string>* cadena_bloques = new BlockChain<string>(); // blockchain

//arboles (2: no string<> date, int)
AVLTree2<int,int>* avl1 = new AVLTree2<int,int>(); // id monto
AVLTree2<int,long>* avl2 = new AVLTree2<int,long>(); //id fechas

//table_hash (4: todas las columnas)
ChainHash<string,int>* string1 = new ChainHash<string,int>(); //primer string
ChainHash<string,int>* string2 = new ChainHash<string,int>(); //segundo string
ChainHash<int,int>* numero = new ChainHash<int,int>(); //para monto id/monto
ChainHash<long,int>* fecha = new ChainHash<long,int>(); //para fecha/id>

TriePatricia<int>* inicia_string1 = new TriePatricia<int>();
TriePatricia<int>* inicia_string2 = new TriePatricia<int>();

QStringList attr;
string *values = new string[4];
int opcion = 1;

int cant_datos = 0;
string* new_data = new string[4];
string data;
int columnas = 4;
string* atributos = new string [columnas]; // atributos de cada bloque

int ind=0;
int size_qdata =4 ;
string* qdata = new string[size_qdata]; //vector <string> qdata;

int en=0;

//int n1=0;

string* del_datos(int nro_block){
    string* datos;
    int size_datos = cadena_bloques->get_block(datos,nro_block); // recibe todos los datos

    int ind = -1; // para saber qué dato vamos a eliminar por iteracion (en el caso de +1 fila)

    while (size_datos>0){// eliminamos todo lo relacionado con el anterior bloque

        // eliminamos clientes
        string1->remove(datos[++ind],nro_block); // eliminacion en hash
        //cout<<datos[ind]<<endl;
        inicia_string1->remove(datos[ind]); // eliminacion en patricia
        //cout<<ind; // ind = 0, 4, 8 , ...

        // eliminamos lugar
        string2->remove(datos[++ind],nro_block); // eliminacion en hash
        inicia_string2->remove(datos[ind]); // eliminacion en patricia
        //cout<<ind; // ind = 1, 5, 9, ...

        // eliminamos monto // ind = 2, 6, 10,...
        numero->remove(stoi(datos[++ind]),nro_block); // eliminacion en hash
        avl1->remove(nro_block,stoi(datos[ind])); // eliminacion en avl
        //cout<<ind;  // ind = 1, 5, 9, ...

        // eliminamos fecha
        fecha->remove(convertToUnixTimestamp(datos[++ind]),nro_block);//eliminacion en hash
        avl2->remove(nro_block,convertToUnixTimestamp(datos[ind])); // eliminacion en avl
        //cout<<ind; // ind = 3, 7, 11,...

        size_datos-=columnas; // eliminamos un registro por iteracion
    }
}



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("BlockChain Grupo 5");
    ui->comboBox->setHidden(1);
    ui->plainTextEdit_2->hide();
    ui->plainTextEdit->hide();
    //ui->label->hide();
    ui->searchbtn->hide();
    ui->searchblckbtn->hide();
    ui->selectbtn->hide();
    ui->label->hide();
    ui->label_2->hide();
    ui->label_3->hide();
    ui->label_4->hide();
    ui->plainTextEdit_3->hide();
    ui->plainTextEdit_4->hide();
    ui->plainTextEdit_5->hide();
    ui->plainTextEdit_6->hide();
    ui->addbtn->hide();
    ui->donebtn->hide();
    ui->lineEdit->hide();
    ui->label_5->hide();


    // Assuming you have a QPushButton pointer for button1 and button2
    // and assuming the function is inside a QObject-derived class

    // Connect button1's clicked signal to the function
    QObject::connect(ui->addbtn, &QPushButton::clicked, this, &MainWindow::handleButtonClicked);

    // Connect button2's clicked signal to the same function
    QObject::connect(ui->donebtn, &QPushButton::clicked, this, &MainWindow::handleButtonClicked);


}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_loadbtn_clicked()
{

    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(fileName);


    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // Handle file open error
        qDebug()<<"line 32";
        return;
    }

    int count1 =0;
    QTextStream in(&file);

    QStringList words;
    QStringList words1;
    QStringList words2;
    QStringList words3;
    QStringList words4;
    //QStringList attr;
    //int rowCount = 0;
    int count = 0;
    string item;
    int cantidad_registros = 10; //registros por bloque
    string* values = new string[columnas*cantidad_registros]; // columnas = 4 (datos)
    int j=0;
    while (!in.atEnd()){
        QString line = in.readLine();
        words = line.split(',');
        if(count==0){
        attr=words;

        for(int i=0; i<4;i++){
            atributos[i]=attr[i].toStdString();
        }
        cadena_bloques->set_atributos(atributos,4);
        count++;
        }

        else if(words.size()==4 || count>0 ){


            words1.append(words.at(0));                         //First element
            string1->insert(words.at(0).toStdString(),count-1); // hash
                                                                 //boyer
            inicia_string1->insert(count-1,words.at(0).toStdString()); //patricia
            values[j++] = words.at(0).toStdString(); //values.push_back(item);

            //-------------------------------------------------------------

            words2.append(words.at(1));
            string2->insert(words.at(1).toStdString(),count-1); // hash
            inicia_string2->insert(count-1,words.at(1).toStdString()); //patricia
            values[j++] = words.at(1).toStdString(); //values.push_back(item);

            //-------------------------------------------------------------

            words3.append(words.at(2));
            int number=stoi(words.at(2).toStdString());
            avl1->insert(count-1,number); // key: nro bloque  //  value:monto -- lo ordena por monto(por value)
            numero->insert(number,count-1);
            values[j++] = words.at(2).toStdString();

            //-------------------------------------------------------------

            words4.append(words.at(3));
            time_t date = convertToUnixTimestamp(words.at(3).toStdString());
            avl2->insert(count-1,date); // key: nro bloque   //  value:fecha -- lo ordena por fecha(por value)
            fecha->insert(date,count-1);
            values[j++] = words.at(3).toStdString(); //values.push_back(item);

            if (j==columnas*cantidad_registros){

                cadena_bloques->insert(values,j);
                j =0;
                count++;
            }
        }

    }
    if(j!=0){
        cadena_bloques->insert(values,j); // se inserta en el bloque
    }
    delete [] values;
    file.close();
    //string *atributos = new string[4];




    ui->comboBox->setHidden(0);
    ui->comboBox->addItem("1. Agregar registro");
    ui->comboBox->addItem("2. Buscar por " + attr[0]);
    ui->comboBox->addItem("3. Buscar por "+ attr[1]);
    ui->comboBox->addItem("4. Buscar por "+ attr[2]);
    ui->comboBox->addItem("5. Buscar por "+ attr[3]);
    ui->comboBox->addItem("6. Buscar por rango en "+ attr[2]);
    ui->comboBox->addItem("7. Buscar por rango en "+ attr[3]);
    ui->comboBox->addItem("8. Buscar minimo por "+ attr[2]);
    ui->comboBox->addItem("9. Buscar minimo por "+ attr[3]);
    ui->comboBox->addItem("10. Buscar maximo por "+ attr[2]);
    ui->comboBox->addItem("11. Buscar maximo por "+ attr[3]);
    ui->comboBox->addItem("12. Inicia con " + attr[0]);
    ui->comboBox->addItem("13. Inicia con " + attr[1]);
    ui->comboBox->addItem("14. Buscar el patron " + attr[0]);
    ui->comboBox->addItem("15. Buscar el patron "+ attr[1]);
    ui->comboBox->addItem("16. Recalculo en cascada");
    ui->comboBox->addItem("17. Modificar bloque");
    ui->selectbtn->show();

}


void MainWindow::on_dispbtn_clicked()
{   ui->label->hide();
    ui->plainTextEdit_2->show();
    ui->label_2->hide();
    ui->label_3->hide();
    ui->label_4->hide();
    ui->plainTextEdit_3->hide();
    ui->plainTextEdit_4->hide();
    ui->plainTextEdit_5->hide();
    ui->plainTextEdit_6->hide();
    ui->plainTextEdit->show();
    ui->plainTextEdit_2->hide();
    //ui->label->hide();
    ui->addbtn->hide();
    ui->donebtn->hide();
    ui->searchbtn->hide();
    // Call the void function
    cadena_bloques->display();
    // Retrieve the output generated by the void function
    std::stringstream buffer;
    std::streambuf* oldCoutBuffer = std::cout.rdbuf(buffer.rdbuf());

    cadena_bloques->display();

    std::cout.rdbuf(oldCoutBuffer);

    // Convert the output to QString
    QString output = QString::fromStdString(buffer.str());

    // Display the output in the QPlainTextEdit widget
    ui->plainTextEdit->setPlainText(output);
}


void MainWindow::on_selectbtn_clicked()
{
    ui->plainTextEdit_3->clear();
    ui->plainTextEdit_4->clear();
    ui->plainTextEdit_5->clear();
    ui->plainTextEdit_6->clear();
    int index = ui->comboBox->currentIndex();

    if(index == 0){         //agregar registro
        ui->label_5->hide();
        ui->lineEdit->hide();
        ui->searchblckbtn->hide();
        ui->plainTextEdit->hide();
        ui->plainTextEdit_2->hide();
        ui->searchbtn->hide();
        ui->label->show();
        ui->label_2->show();
        ui->label_3->show();
        ui->label_4->show();
        ui->plainTextEdit_3->show();
        ui->plainTextEdit_3->setPlaceholderText("");
        ui->plainTextEdit_4->show();
        ui->plainTextEdit_4->setPlaceholderText("");
        ui->plainTextEdit_5->show();
        ui->plainTextEdit_6->show();
        ui->label->setText("Ingresar "+attr[0]);
        ui->label_2->setText("Ingresar "+attr[1]);
        ui->label_3->setText("Ingresar "+attr[2]);
        ui->label_4->setText("Ingresar "+attr[3]);
        ui->addbtn->show();
        ui->donebtn->show();
        en=0;

    }

    if(index == 1){         //buscar por attr[0]
        ui->label_5->hide();
        ui->lineEdit->hide();
        ui->searchblckbtn->hide();
        ui->addbtn->hide();
        ui->donebtn->hide();
        ui->plainTextEdit->hide();
        ui->plainTextEdit_2->hide();
        ui->label->show();
        ui->label_2->hide();
        ui->label_3->hide();
        ui->label_4->hide();
        ui->plainTextEdit_3->show();
        ui->plainTextEdit_3->setPlaceholderText("");
        ui->plainTextEdit_4->hide();
        ui->plainTextEdit_5->hide();
        ui->plainTextEdit_6->hide();
        ui->searchbtn->show();
        ui->horizontalLayoutWidget->show();
        ui->horizontalLayoutWidget_2->show();
        //int width = 100; // Specify the desired width
        //int height = 50; // Specify the desired height
        //ui->plainTextEdit_3->setFixedSize(width, height);
        ui->label->setText("Ingresar " +attr[0]);
    }

    if(index == 2){         //buscar por attr[1]
        ui->label_5->hide();
        ui->lineEdit->hide();
        ui->searchblckbtn->hide();
        ui->addbtn->hide();
        ui->donebtn->hide();
        ui->plainTextEdit->hide();
        ui->plainTextEdit_2->hide();
        ui->label->show();
        ui->label_2->hide();
        ui->label_3->hide();
        ui->label_4->hide();
        ui->plainTextEdit_3->show();
        ui->plainTextEdit_3->setPlaceholderText("");
        ui->plainTextEdit_4->hide();
        ui->plainTextEdit_5->hide();
        ui->plainTextEdit_6->hide();
        ui->searchbtn->show();
        ui->horizontalLayoutWidget->show();
        ui->horizontalLayoutWidget_2->show();
        //int width = 100; // Specify the desired width
        //int height = 50; // Specify the desired height
        //ui->plainTextEdit_3->setFixedSize(width, height);
        ui->label->setText("Ingresar " +attr[1]);
    }

    if(index == 3){         //buscar por attr[2]
        ui->label_5->hide();
        ui->lineEdit->hide();
        ui->searchblckbtn->hide();
        ui->addbtn->hide();
        ui->donebtn->hide();
        ui->plainTextEdit->hide();
        ui->plainTextEdit_2->hide();
        ui->label->show();
        ui->label_2->hide();
        ui->label_3->hide();
        ui->label_4->hide();
        ui->plainTextEdit_3->show();
        ui->plainTextEdit_3->setPlaceholderText("");
        ui->plainTextEdit_4->hide();
        ui->plainTextEdit_5->hide();
        ui->plainTextEdit_6->hide();
        ui->searchbtn->show();
        ui->horizontalLayoutWidget->show();
        ui->horizontalLayoutWidget_2->show();
        //int width = 100; // Specify the desired width
        //int height = 50; // Specify the desired height
        //ui->plainTextEdit_3->setFixedSize(width, height);
        ui->label->setText("Ingresar " +attr[2]);
    }

    if(index == 4){         //buscar por attr[3]
        ui->label_5->hide();
        ui->lineEdit->hide();
        ui->searchblckbtn->hide();
        ui->plainTextEdit_3->setPlaceholderText("dd-mm-aaaa");
        ui->addbtn->hide();
        ui->donebtn->hide();
        ui->plainTextEdit->hide();
        ui->plainTextEdit_2->hide();
        ui->label->show();
        ui->label_2->hide();
        ui->label_3->hide();
        ui->label_4->hide();
        ui->plainTextEdit_3->show();
        ui->plainTextEdit_4->hide();
        ui->plainTextEdit_5->hide();
        ui->plainTextEdit_6->hide();
        ui->searchbtn->show();
        ui->horizontalLayoutWidget->show();
        ui->horizontalLayoutWidget_2->show();
        //int width = 100; // Specify the desired width
        //int height = 50; // Specify the desired height
        //ui->plainTextEdit_3->setFixedSize(width, height);
        ui->label->setText("Ingresar " +attr[3]);
    }

    if(index == 5){         //Rango monto
        ui->label_5->hide();
        ui->lineEdit->hide();
        ui->searchblckbtn->hide();
        ui->addbtn->hide();
        ui->donebtn->hide();
        ui->plainTextEdit->hide();
        ui->plainTextEdit_2->hide();
        ui->label->show();
        ui->label_2->show();
        ui->label_3->hide();
        ui->label_4->hide();
        ui->plainTextEdit_3->show();
        ui->plainTextEdit_3->setPlaceholderText("");
        ui->plainTextEdit_4->show();
        ui->plainTextEdit_4->setPlaceholderText("");
        ui->plainTextEdit_5->hide();
        ui->plainTextEdit_6->hide();
        ui->searchbtn->show();
        ui->horizontalLayoutWidget->show();
        ui->horizontalLayoutWidget_2->show();
        //int width = 100; // Specify the desired width
        //int height = 50; // Specify the desired height
        //ui->plainTextEdit_3->setFixedSize(width, height);
        ui->label->setText("Ingresar " +attr[2]+" 1");
        ui->label_2->setText("Ingresar " +attr[2]+" 2");
    }

    if(index == 6){         //Rango fecha
        ui->label_5->hide();
        ui->lineEdit->hide();
        ui->searchblckbtn->hide();
        ui->addbtn->hide();
        ui->donebtn->hide();
        ui->plainTextEdit->hide();
        ui->plainTextEdit_2->hide();
        ui->label->show();
        ui->label_2->show();
        ui->label_3->hide();
        ui->label_4->hide();
        ui->plainTextEdit_3->show();
        ui->plainTextEdit_3->setPlaceholderText("dd-mm-aaaa");
        ui->plainTextEdit_4->show();
        ui->plainTextEdit_4->setPlaceholderText("dd-mm-aaaa");
        ui->plainTextEdit_5->hide();
        ui->plainTextEdit_6->hide();
        ui->searchbtn->show();
        ui->horizontalLayoutWidget->show();
        ui->horizontalLayoutWidget_2->show();
        //int width = 100; // Specify the desired width
        //int height = 50; // Specify the desired height
        //ui->plainTextEdit_3->setFixedSize(width, height);
        ui->label->setText("Ingresar " +attr[3]+" 1");
        ui->label_2->setText("Ingresar " +attr[3]+" 2");
    }

    if(index == 7){         //Minimo por monto
        ui->label_5->hide();
        ui->lineEdit->hide();
        ui->searchblckbtn->hide();
        ui->addbtn->hide();
        ui->donebtn->hide();
        ui->plainTextEdit->hide();
        ui->plainTextEdit_2->hide();
        ui->label->hide();
        ui->label_2->hide();
        ui->label_3->hide();
        ui->label_4->hide();
        ui->plainTextEdit_3->hide();
        ui->plainTextEdit_4->hide();
        ui->plainTextEdit_5->hide();
        ui->plainTextEdit_6->hide();
        ui->searchbtn->show();
        ui->horizontalLayoutWidget->show();
        ui->horizontalLayoutWidget_2->show();
        //int width = 100; // Specify the desired width
        //int height = 50; // Specify the desired height
        //ui->plainTextEdit_3->setFixedSize(width, height);
        //ui->label->setText("Ingresar " +attr[2]);
    }

    if(index == 8){         //Minimo por fecha
        ui->label_5->hide();
        ui->lineEdit->hide();
        ui->searchblckbtn->hide();
        ui->addbtn->hide();
        ui->donebtn->hide();
        ui->plainTextEdit->hide();
        ui->plainTextEdit_2->hide();
        ui->label->hide();
        ui->label_2->hide();
        ui->label_3->hide();
        ui->label_4->hide();
        ui->plainTextEdit_3->hide();
        ui->plainTextEdit_4->hide();
        ui->plainTextEdit_5->hide();
        ui->plainTextEdit_6->hide();
        ui->searchbtn->show();
        ui->horizontalLayoutWidget->show();
        ui->horizontalLayoutWidget_2->show();
        //int width = 100; // Specify the desired width
        //int height = 50; // Specify the desired height
        //ui->plainTextEdit_3->setFixedSize(width, height);
        //ui->label->setText("Ingresar " +attr[3]);
    }

    if(index == 9){         //Maximo por Monto
        ui->label_5->hide();
        ui->lineEdit->hide();
        ui->searchblckbtn->hide();
        ui->addbtn->hide();
        ui->donebtn->hide();
        ui->plainTextEdit->hide();
        ui->plainTextEdit_2->hide();
        ui->label->hide();
        ui->label_2->hide();
        ui->label_3->hide();
        ui->label_4->hide();
        ui->plainTextEdit_3->hide();
        ui->plainTextEdit_4->hide();
        ui->plainTextEdit_5->hide();
        ui->plainTextEdit_6->hide();
        ui->searchbtn->show();
        ui->horizontalLayoutWidget->show();
        ui->horizontalLayoutWidget_2->show();
        //int width = 100; // Specify the desired width
        //int height = 50; // Specify the desired height
        //ui->plainTextEdit_3->setFixedSize(width, height);

    }

    if(index == 10){         //Maximo por fecha
        ui->label_5->hide();
        ui->lineEdit->hide();
        ui->searchblckbtn->hide();
        ui->addbtn->hide();
        ui->donebtn->hide();
        ui->plainTextEdit->hide();
        ui->plainTextEdit_2->hide();
        ui->label->hide();
        ui->label_2->hide();
        ui->label_3->hide();
        ui->label_4->hide();
        ui->plainTextEdit_3->hide();
        ui->plainTextEdit_4->hide();
        ui->plainTextEdit_5->hide();
        ui->plainTextEdit_6->hide();
        ui->searchbtn->show();
        ui->horizontalLayoutWidget->show();
        ui->horizontalLayoutWidget_2->show();
        //int width = 100; // Specify the desired width
        //int height = 50; // Specify the desired height
        //ui->plainTextEdit_3->setFixedSize(width, height);

    }

    if(index == 11){         //Patricia Nombre
        ui->label_5->hide();
        ui->lineEdit->hide();
        ui->searchblckbtn->hide();
        ui->addbtn->hide();
        ui->donebtn->hide();
        ui->plainTextEdit->hide();
        ui->plainTextEdit_2->hide();
        ui->label->show();
        ui->label_2->hide();
        ui->label_3->show();
        ui->label_4->hide();
        ui->plainTextEdit_3->show();
        ui->plainTextEdit_4->hide();
        ui->plainTextEdit_5->hide();
        ui->plainTextEdit_6->hide();
        ui->searchbtn->show();
        ui->horizontalLayoutWidget->show();
        ui->horizontalLayoutWidget_2->show();
        ui->label->setText("Inicia con #" +attr[0]);
    }

    if(index == 12){         //Patricia Lugar
        ui->label_5->hide();
        ui->lineEdit->hide();
        ui->searchblckbtn->hide();
        ui->addbtn->hide();
        ui->donebtn->hide();
        ui->plainTextEdit->hide();
        ui->plainTextEdit_2->hide();
        ui->label->show();
        ui->label_2->hide();
        ui->label_3->hide();
        ui->label_4->hide();
        ui->plainTextEdit_3->show();
        ui->plainTextEdit_4->hide();
        ui->plainTextEdit_5->hide();
        ui->plainTextEdit_6->hide();
        ui->searchbtn->show();
        ui->horizontalLayoutWidget->show();
        ui->horizontalLayoutWidget_2->show();
        //int width = 100; // Specify the desired width
        //int height = 50; // Specify the desired height
        //ui->plainTextEdit_3->setFixedSize(width, height);
        ui->label->setText("Inicia con #" +attr[1]);
    }

    if(index == 13){         //buscar patron Monto
        ui->label_5->hide();
        ui->lineEdit->hide();
        ui->searchblckbtn->hide();
        ui->addbtn->hide();
        ui->donebtn->hide();
        ui->plainTextEdit->hide();
        ui->plainTextEdit_2->hide();
        ui->label->show();
        ui->label_2->hide();
        ui->label_3->hide();
        ui->label_4->hide();
        ui->plainTextEdit_3->show();
        ui->plainTextEdit_4->hide();
        ui->plainTextEdit_5->hide();
        ui->plainTextEdit_6->hide();
        ui->searchbtn->show();
        ui->horizontalLayoutWidget->show();
        ui->horizontalLayoutWidget_2->show();
        ui->label->setText("Buscar Patron " +attr[0]);
    }

    if(index == 14){         //buscar patron Lugar
        ui->label_5->hide();
        ui->lineEdit->hide();
        ui->searchblckbtn->hide();
        ui->addbtn->hide();
        ui->donebtn->hide();
        ui->plainTextEdit->hide();
        ui->plainTextEdit_2->hide();
        ui->label->show();
        ui->label_2->hide();
        ui->label_3->hide();
        ui->label_4->hide();
        ui->plainTextEdit_3->show();
        ui->plainTextEdit_4->hide();
        ui->plainTextEdit_5->hide();
        ui->plainTextEdit_6->hide();
        ui->searchbtn->show();
        ui->horizontalLayoutWidget->show();
        ui->horizontalLayoutWidget_2->show();
        ui->label->setText("Buscar Patron " +attr[1]);
    }

    if(index == 15){         //Recalculo
        ui->label_5->hide();
        ui->lineEdit->hide();
        ui->searchblckbtn->hide();
        ui->addbtn->hide();
        ui->donebtn->hide();
        ui->plainTextEdit->hide();
        ui->plainTextEdit_2->hide();
        ui->label->hide();
        ui->label_2->hide();
        ui->label_3->hide();
        ui->label_4->hide();
        ui->plainTextEdit_3->hide();
        ui->plainTextEdit_4->hide();
        ui->plainTextEdit_5->hide();
        ui->plainTextEdit_6->hide();
        ui->searchbtn->show();
        ui->horizontalLayoutWidget->show();
        ui->horizontalLayoutWidget_2->show();
    }

    if(index == 16){         //Modificar bloque
        ui->label_5->show();
        ui->lineEdit->show();
        ui->plainTextEdit->hide();
        ui->plainTextEdit_2->hide();
        ui->searchbtn->hide();
        ui->searchblckbtn->show();
        ui->label->hide();
        ui->label_2->hide();
        ui->label_3->hide();
        ui->label_4->hide();
        ui->plainTextEdit_3->hide();

        ui->plainTextEdit_4->hide();
        ui->plainTextEdit_4->setPlaceholderText("");
        ui->plainTextEdit_5->hide();
        ui->plainTextEdit_6->hide();
        //ui->label->setText("Ingresar "+attr[0]);
        //ui->label_2->setText("Ingresar "+attr[1]);
        //ui->label_3->setText("Ingresar "+attr[2]);
        //ui->label_4->setText("Ingresar "+attr[3]);
        //ui->addbtn->show();
        //ui->donebtn->show();
        en=1;

    }
}


void MainWindow::on_clearbtn_clicked()
{
    ui->plainTextEdit->clear();
    //ui->plainTextEdit->hide();
}

void MainWindow::on_searchbtn_clicked()
{
    CircularArray<int> res;

    int index = ui->comboBox->currentIndex();
    QString qattr=ui->plainTextEdit_3->toPlainText();
    string sattr = qattr.toStdString();

    QString qattr2=ui->plainTextEdit_4->toPlainText();
    string sattr2 = qattr2.toStdString();

    time_t c1, c2;
    if(index==1){

        ui->plainTextEdit_2->show();

        //// Retrieve the output generated by the void function
        stringstream buffer;
        streambuf* oldCoutBuffer = cout.rdbuf(buffer.rdbuf());
        try{
        cadena_bloques->get_block(string1->search(sattr));
        } catch (...) {
        cout<<"Registro no existe";
        }
        cout.rdbuf(oldCoutBuffer);

        //// Convert the output to QString
        QString output = QString::fromStdString(buffer.str());

        //// Display the output in the QPlainTextEdit widget
        ui->plainTextEdit_2->setPlainText(output);



       }

    if(index==2){
        ui->plainTextEdit_2->show();
        cadena_bloques->get_block(string2->search(sattr));
        //// Retrieve the output generated by the void function
        std::stringstream buffer;
        std::streambuf* oldCoutBuffer = std::cout.rdbuf(buffer.rdbuf());

        cadena_bloques->get_block(string2->search(sattr));

        std::cout.rdbuf(oldCoutBuffer);

        //// Convert the output to QString
        QString output = QString::fromStdString(buffer.str());

        //// Display the output in the QPlainTextEdit widget
        ui->plainTextEdit_2->setPlainText(output);
       }

    if(index==3){
        ui->plainTextEdit_2->show();

        //cadena_bloques->get_block(numero->search(stoi(sattr)));
        //// Retrieve the output generated by the void function
        std::stringstream buffer;
        std::streambuf* oldCoutBuffer = std::cout.rdbuf(buffer.rdbuf());
        try{
        cadena_bloques->get_block(numero->search(stoi(sattr)));
        }catch(...){
        cout<<"Registro no existe";
        }
        std::cout.rdbuf(oldCoutBuffer);

        //// Convert the output to QString
        QString output = QString::fromStdString(buffer.str());

        //// Display the output in the QPlainTextEdit widget
        ui->plainTextEdit_2->setPlainText(output);
    }

    if(index==4){           //Search fecha
        ui->plainTextEdit_2->show();
        //QString formato = "dd-mm-aaaa";
        ui->plainTextEdit_3->setPlaceholderText("dd-mm-aaaa");
        string s4;
        try {
            QString qs4 = ui->plainTextEdit_3->toPlainText();
            QStringList list4 = qs4.split('-');
            if (1 < list4.count()){
            QString dia = list4[0];
            QString mes = list4[1];
            QString year = list4[2];
            QString qfecha = year + '-' +mes + '-' +dia;


            s4 = qfecha.toStdString(); // fecha
            }
            //cout<<"\n"<<s4;


            //cout<<"\n"<<c1;
            //cadena_bloques->get_block(fecha->search(c1));


            //// Retrieve the output generated by the void function
            std::stringstream buffer;
            std::streambuf* oldCoutBuffer = std::cout.rdbuf(buffer.rdbuf());
            try{
            c1 = convertToUnixTimestamp(s4);
            cadena_bloques->get_block(fecha->search(c1));
            }catch(...){
            cout<<"Registro no existe";
            }

            std::cout.rdbuf(oldCoutBuffer);

            //// Convert the output to QString
            QString output = QString::fromStdString(buffer.str());

            //// Display the output in the QPlainTextEdit widget
            ui->plainTextEdit_2->setPlainText(output);
        }catch(...){
            std::stringstream buffer;
            std::streambuf* oldCoutBuffer = std::cout.rdbuf(buffer.rdbuf());

            cout<<"Ingresar fecha con el formato correcto";

            std::cout.rdbuf(oldCoutBuffer);

            //// Convert the output to QString
            QString output = QString::fromStdString(buffer.str());

            //// Display the output in the QPlainTextEdit widget
            ui->plainTextEdit_2->setPlainText(output);
        }
    }

    if(index==5){           //Rango Monto
        ui->plainTextEdit_2->show();
        avl1->search_by_range(res, stoi(sattr), stoi(sattr2));

        //// Retrieve the output generated by the void function
        std::stringstream buffer;
        std::streambuf* oldCoutBuffer = std::cout.rdbuf(buffer.rdbuf());

        for (int i = 0; i<int(res.size()); i++) {
            cadena_bloques->get_block(res[i]);
        }

        std::cout.rdbuf(oldCoutBuffer);

        //// Convert the output to QString
        QString output = QString::fromStdString(buffer.str());

        //// Display the output in the QPlainTextEdit widget
        ui->plainTextEdit_2->setPlainText(output);
    }

    if(index==6){            //Rango fecha
        ui->plainTextEdit_2->show();

        //QString formato = "dd-mm-aaaa";
        //ui->plainTextEdit_2->setPlaceholderText(formato);
        QString qs4 = ui->plainTextEdit_3->toPlainText();
        QString qs5 = ui->plainTextEdit_4->toPlainText();
        QStringList list4 = qs4.split('-');
        QStringList list5 = qs5.split('-');
        QString dia = list4[0];
        QString mes = list4[1];
        QString year = list4[2];

        QString qfecha = year + '-' +mes + '-' +dia;
        QString dia5 = list5[0];
        QString mes5 = list5[1];
        QString year5 = list5[2];
        QString qfecha5 = year5 + '-' +mes5 + '-' +dia5;

        string s4;
        string s5;
        s4 = qfecha.toStdString(); // fecha
        s5 = qfecha5.toStdString();
        c1 = convertToUnixTimestamp(s4);
        c2 = convertToUnixTimestamp(s5);

        avl2->search_by_range(res, c1, c2);

        //// Retrieve the output generated by the void function
        std::stringstream buffer;
        std::streambuf* oldCoutBuffer = std::cout.rdbuf(buffer.rdbuf());

        for (int i = 0; i<int(res.size()); i++) {
            cadena_bloques->get_block(res[i]);
        }

        std::cout.rdbuf(oldCoutBuffer);

        //// Convert the output to QString
        QString output = QString::fromStdString(buffer.str());

        //// Display the output in the QPlainTextEdit widget
        ui->plainTextEdit_2->setPlainText(output);
    }

    if(index==7){           //Minimo Monto
        ui->plainTextEdit_2->show();

        //// Retrieve the output generated by the void function
        std::stringstream buffer;
        std::streambuf* oldCoutBuffer = std::cout.rdbuf(buffer.rdbuf());

        cadena_bloques->get_block(avl1->minValue());

        std::cout.rdbuf(oldCoutBuffer);

        //// Convert the output to QString
        QString output = QString::fromStdString(buffer.str());

        //// Display the output in the QPlainTextEdit widget
        ui->plainTextEdit_2->setPlainText(output);
    }

    if(index==8){           //Minimo Fecha
        ui->plainTextEdit_2->show();

        //QString formato = "dd-mm-aaaa";
        //ui->plainTextEdit_2->setPlaceholderText(formato);




        //// Retrieve the output generated by the void function
        std::stringstream buffer;
        std::streambuf* oldCoutBuffer = std::cout.rdbuf(buffer.rdbuf());

        cadena_bloques->get_block(avl2->minValue());

        std::cout.rdbuf(oldCoutBuffer);

        //// Convert the output to QString
        QString output = QString::fromStdString(buffer.str());

        //// Display the output in the QPlainTextEdit widget
        ui->plainTextEdit_2->setPlainText(output);
    }

    if(index==9){           //Maximo Monto
        ui->plainTextEdit_2->show();

        //QString formato = "dd-mm-aaaa";
        //ui->plainTextEdit_2->setPlaceholderText(formato);




        //// Retrieve the output generated by the void function
        std::stringstream buffer;
        std::streambuf* oldCoutBuffer = std::cout.rdbuf(buffer.rdbuf());

        cadena_bloques->get_block(avl1->maxValue());

        std::cout.rdbuf(oldCoutBuffer);

        //// Convert the output to QString
        QString output = QString::fromStdString(buffer.str());

        //// Display the output in the QPlainTextEdit widget
        ui->plainTextEdit_2->setPlainText(output);
    }

    if(index==10){          //Maximo Fecha
        ui->plainTextEdit_2->show();

        //QString formato = "dd-mm-aaaa";
        //ui->plainTextEdit_2->setPlaceholderText(formato);




        //// Retrieve the output generated by the void function
        std::stringstream buffer;
        std::streambuf* oldCoutBuffer = std::cout.rdbuf(buffer.rdbuf());

        cadena_bloques->get_block(avl2->maxValue());

        std::cout.rdbuf(oldCoutBuffer);

        //// Convert the output to QString
        QString output = QString::fromStdString(buffer.str());

        //// Display the output in the QPlainTextEdit widget
        ui->plainTextEdit_2->setPlainText(output);
    }

    if(index==11){          //Patricia Nombre
        ui->plainTextEdit_2->show();
        QString tmp1 = ui->plainTextEdit_3->toPlainText();
        tmp1 = tmp1[0].toUpper();
        string data = tmp1.toStdString();
        for (int i=0; i<int(data.size()); i++) data[i] = tolower(data[i]);

        inicia_string1->start_with(res, data);




        //// Retrieve the output generated by the void function
        std::stringstream buffer;
        std::streambuf* oldCoutBuffer = std::cout.rdbuf(buffer.rdbuf());

        if (int(res.size())==0){
            cout<<"No existen cadenas que inicien con "<<data<<endl;
        }
        else{
          for (int i=0; i<int(res.size()); i++){
          cadena_bloques->get_block(res[i]);
          }
        }

        std::cout.rdbuf(oldCoutBuffer);

        //// Convert the output to QString
        QString output = QString::fromStdString(buffer.str());

        //// Display the output in the QPlainTextEdit widget
        ui->plainTextEdit_2->setPlainText(output);
    }

    if(index==12){          //Patricia Lugar
        ui->plainTextEdit_2->show();
        QString tmp1 = ui->plainTextEdit_3->toPlainText();
        tmp1 = tmp1[0].toUpper();
        string data = tmp1.toStdString();
        for (int i=0; i<int(data.size()); i++) data[i] = tolower(data[i]);

        inicia_string2->start_with(res, data);




        //// Retrieve the output generated by the void function
        std::stringstream buffer;
        std::streambuf* oldCoutBuffer = std::cout.rdbuf(buffer.rdbuf());

        if (int(res.size())==0){
          cout<<"No existen cadenas que inicien con "<<data<<endl;
        }
        else{
          for (int i=0; i<int(res.size()); i++){
          cadena_bloques->get_block(res[i]);
          }
        }

        std::cout.rdbuf(oldCoutBuffer);

        //// Convert the output to QString
        QString output = QString::fromStdString(buffer.str());

        //// Display the output in the QPlainTextEdit widget
        ui->plainTextEdit_2->setPlainText(output);
    }

    if(index==13){          //Patron Nombre
        ui->plainTextEdit_2->show();

        //QString formato = "dd-mm-aaaa";
        //ui->plainTextEdit_2->setPlaceholderText(formato);




        //// Retrieve the output generated by the void function
        std::stringstream buffer;
        std::streambuf* oldCoutBuffer = std::cout.rdbuf(buffer.rdbuf());

        cadena_bloques->get_block(avl2->maxValue());

        std::cout.rdbuf(oldCoutBuffer);

        //// Convert the output to QString
        QString output = QString::fromStdString(buffer.str());

        //// Display the output in the QPlainTextEdit widget
        ui->plainTextEdit_2->setPlainText(output);
    }

    if(index==14){          //Patron lugar
        ui->plainTextEdit_2->show();

        //QString formato = "dd-mm-aaaa";
        //ui->plainTextEdit_2->setPlaceholderText(formato);




        //// Retrieve the output generated by the void function
        std::stringstream buffer;
        std::streambuf* oldCoutBuffer = std::cout.rdbuf(buffer.rdbuf());

        cadena_bloques->get_block(avl2->maxValue());

        std::cout.rdbuf(oldCoutBuffer);

        //// Convert the output to QString
        QString output = QString::fromStdString(buffer.str());

        //// Display the output in the QPlainTextEdit widget
        ui->plainTextEdit_2->setPlainText(output);
    }

    if(index==15){          //Recalculo
        //ui->plainTextEdit_2->show();

        //QString formato = "dd-mm-aaaa";
        //ui->plainTextEdit_2->setPlaceholderText(formato);


        //cadena_bloques->recalculo_cascada();

        //// Retrieve the output generated by the void function
        //std::stringstream buffer;
        //std::streambuf* oldCoutBuffer = std::cout.rdbuf(buffer.rdbuf());

        cadena_bloques->recalculo_cascada();

        ////std::cout.rdbuf(oldCoutBuffer);

        //////// Convert the output to QString
        ////QString output = QString::fromStdString(buffer.str());

        //////// Display the output in the QPlainTextEdit widget
        ////ui->plainTextEdit_2->setPlainText(output);
    }

    if(index==16){          //Modificar bloque
        //ui->plainTextEdit_2->show();

        //int n1 = ui->plainTextEdit_3->toPlainText().toInt();

        //if (cadena_bloques->exist_block(n1)){
        //    //avl1->remove(n1);
        //    //avl2->remove(n1);
        //    cant_datos = 0;
        //    new_data = nuevos_datos(cant_datos,n1);
        //    cadena_bloques->modificar_bloque(n1, new_data,cant_datos);

        //    delete [] new_data; cant_datos=0; new_data = nullptr;
        //}

        ////// Retrieve the output generated by the void function
        //std::stringstream buffer;
        //std::streambuf* oldCoutBuffer = std::cout.rdbuf(buffer.rdbuf());

        //cadena_bloques->get_block(avl2->maxValue());

        //std::cout.rdbuf(oldCoutBuffer);

        ////// Convert the output to QString
        //QString output = QString::fromStdString(buffer.str());

        ////// Display the output in the QPlainTextEdit widget
        //ui->plainTextEdit_2->setPlainText(output);
    }
}

void MainWindow::handleButtonClicked()
{
    // Identify which button triggered the function
    QPushButton* senderButton = qobject_cast<QPushButton*>(sender());
    if(en==0){
        if(senderButton == ui->addbtn) {

            string s1,s2;
            int s3;
            string s4;
            int size = cadena_bloques->get_size();
            if (ind == size_qdata) { //resize
                string* copia = new string[size_qdata];
                for (int i = 0; i < size_qdata; i++)
                    copia[i] = qdata[i];

                delete[] qdata; // Liberar la memoria del arreglo original

                qdata = new string[size_qdata * 2];
                for (int i = 0; i < size_qdata; i++)
                    qdata[i] = copia[i];

                size_qdata *= 2;
                delete[] copia;
                }

            QString qs1 = ui->plainTextEdit_3->toPlainText();
            s1 = qs1.toStdString(); // string1
            string1->insert(s1,size);
            inicia_string1->insert(size,s1);

            QString qs2 = ui->plainTextEdit_4->toPlainText();
            s2 = qs2.toStdString();  //string2
            string2->insert(s2,size);
            inicia_string2->insert(size,s2);

            QString qs3 = ui->plainTextEdit_5->toPlainText();
            s3 = qs3.toInt(); // entero(monto)
            numero->insert(s3,size);
            avl1->insert(size,s3);

            QString qs4 = ui->plainTextEdit_6->toPlainText();
            QStringList list4 = qs4.split('-');
            QString dia = list4[0];
            QString mes = list4[1];
            QString year = list4[2];
            QString qfecha = year + '-' +mes + '-' +dia;


            s4 = qfecha.toStdString(); // fecha
            time_t date = convertToUnixTimestamp(s4);
            fecha->insert(date,size);
            avl2->insert(size,date);

            qdata[ind++] = s1; //qdata.push_back(s1);
            qdata[ind++] = s2; //qdata.push_back(s2);
            qdata[ind++] = to_string(s3); //qdata.push_back(to_string(s3));
            qdata[ind++] = s4; //qdata.push_back(s4);
            //cadena_bloques->insert(qdata,ind);


            }
        if(senderButton == ui->donebtn){
            cadena_bloques->insert(qdata,ind);
            //delete[] qdata;
            ind=1;
            cout<<"donebtn /t";
        }
    }
    if(en==1){
        bool ok;
        int n1 = ui->lineEdit->text().toInt(&ok);
            if(senderButton == ui->addbtn) {

                string s1,s2;
                int s3;
                string s4;
                int size = cadena_bloques->get_size();
                if (cant_datos   == size_qdata) { //resize
                string* copia = new string[size_qdata];
                for (int i = 0; i < size_qdata; i++)
                    copia[i] = qdata[i];

                delete[] qdata; // Liberar la memoria del arreglo original

                qdata = new string[size_qdata * 2];
                for (int i = 0; i < size_qdata; i++)
                    qdata[i] = copia[i];

                size_qdata *= 2;
                delete[] copia;
                }

                QString qs1 = ui->plainTextEdit_3->toPlainText();
                s1 = qs1.toStdString(); // string1
                string1->insert(s1,n1);
                inicia_string1->insert(n1,s1);

                QString qs2 = ui->plainTextEdit_4->toPlainText();
                s2 = qs2.toStdString();  //string2
                string2->insert(s2,n1);
                inicia_string2->insert(n1,s2);

                QString qs3 = ui->plainTextEdit_5->toPlainText();
                s3 = qs3.toInt(); // entero(monto)
                numero->insert(s3,n1);
                avl1->insert(n1,s3);

                QString qs4 = ui->plainTextEdit_6->toPlainText();
                QStringList list4 = qs4.split('-');
                QString dia = list4[0];
                QString mes = list4[1];
                QString year = list4[2];
                QString qfecha = year + '-' +mes + '-' +dia;


                s4 = qfecha.toStdString(); // fecha
                time_t date = convertToUnixTimestamp(s4);
                fecha->insert(date,n1);
                avl2->insert(n1,date);

                qdata[cant_datos++] = s1; //qdata.push_back(s1);
                qdata[cant_datos++] = s2; //qdata.push_back(s2);
                qdata[cant_datos++] = to_string(s3); //qdata.push_back(to_string(s3));
                qdata[cant_datos++] = s4; //qdata.push_back(s4);
                //cadena_bloques->insert(qdata,ind);
                new_data= qdata;

            }
            if(senderButton == ui->donebtn){
                ui->plainTextEdit_2->show();
                QStringList qDataList;
                for (int i = 0; i < size_qdata; ++i) {
                QString qElement = QString::fromStdString(qdata[i]);
                qDataList.append(qElement);
                }
                QString tmp2 = qDataList.join(" ");
                ui->plainTextEdit_2->setPlainText(tmp2);


                cadena_bloques->modificar_bloque(n1, new_data,cant_datos);

                delete [] new_data; cant_datos=0; new_data = nullptr;

            }

    }
}

void MainWindow::on_searchblckbtn_clicked()
{
    bool ok;
    int n1 = ui->lineEdit->text().toInt(&ok);
    qDebug()<<n1;

    if (cadena_bloques->exist_block(n1)){

            //Mostrando las widgets
            ui->plainTextEdit_3->show();
            ui->plainTextEdit_4->show();
            ui->plainTextEdit_5->show();
            ui->plainTextEdit_6->show();
            ui->label->show();
            ui->label_2->show();
            ui->label_3->show();
            ui->label_4->show();
            ui->label->setText("Ingresar "+attr[0]);
            ui->label_2->setText("Ingresar "+attr[1]);
            ui->label_3->setText("Ingresar "+attr[2]);
            ui->label_4->setText("Ingresar "+attr[3]);
            ui->addbtn->show();
            ui->donebtn->show();

        //avl1->remove(n1);
        //avl2->remove(n1);
        cant_datos = 0;
        del_datos(n1);
        //new_data = nuevos_datos(cant_datos,n1);
        cadena_bloques->modificar_bloque(n1, new_data,cant_datos);

        delete [] new_data; cant_datos=0; new_data = nullptr;
    }
    else {

        ui->plainTextEdit_3->hide();
        ui->plainTextEdit_4->hide();
        ui->plainTextEdit_5->hide();
        ui->plainTextEdit_6->hide();
        ui->label->hide();
        ui->label_2->hide();
        ui->label_3->hide();
        ui->label_4->hide();
        ui->addbtn->hide();
        ui->donebtn->hide();

        ui->plainTextEdit_2->show();

        //// Retrieve the output generated by the void function
        std::stringstream buffer;
        std::streambuf* oldCoutBuffer = std::cout.rdbuf(buffer.rdbuf());


        cout<<"Bloque no existe\n";
        std::cout.rdbuf(oldCoutBuffer);

        //// Convert the output to QString
        QString output = QString::fromStdString(buffer.str());

        //// Display the output in the QPlainTextEdit widget
        ui->plainTextEdit_2->setPlainText(output);
    }
}

