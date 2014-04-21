#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "iostream"
#include "math.h"


#include <QtDeclarative>


#include <QtWebKit/QWebView>
//#include <QtXml/QtXml>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtSql/QSqlQuery>
#include <QVBoxLayout>
#include <QSqlError>
#include <QDeclarativeItem>

#include "rodoublelineeditdelegate.h"
#include "rosolutes.h"
#include "rosolutesdatamodel.h"
#include "rosolutesdataview.h"
#include "romembranesdatamodel.h"
#include "qmlsolutesmodel.h"

//#include "rofeedelement.h"
#include "rosysteminputview.h"
#include "rotestscheme.h"
#include "rosysteminputdelegaterecylesform.h"
#include "ropass.h"

MainWindow::~MainWindow() {
    db.close();
    db.removeDatabase("QSQLITE");
    delete ui;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {

    ui->setupUi(this);


//    QStandardItemModel* dm1 = new QStandardItemModel();
//    dm1->verticalHeaderItem();
    // Design system
//    ROFeedElement* feed = system.addFeed();
//    ROPass* pass1 = system.addPass(1);
//    ROStage* stage11 = pass1->addStage(1);

    ROSystemInputView* systemConfigInput = new ROSystemInputView(&_system, this);
//    systemConfigInput->setSystem(&_system);
    QHBoxLayout* bl = new QHBoxLayout();
    bl->addWidget(systemConfigInput);
    ROTestScheme* schemeImage = new ROTestScheme(this);
    schemeImage->setMinimumWidth(400);
    bl->addWidget(schemeImage);
    //bl->addItem(new QSpacerItem(50, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    dynamic_cast<QVBoxLayout*>(ui->tab_SystemConfiguration->layout())->addLayout(bl);
    //ui->tab_SystemConfiguration->layout()->addWidget(systemConfigInput);
    //system.

    // SQL
    //connectDB("NanoDatabase.db");



    //TEST


    //ENDTEST




    // Files / Projects
    connect(ui->action_NewProject, SIGNAL(triggered()), this, SLOT(newProject()));
    connect(ui->action_OpenProject, SIGNAL(triggered()), this, SLOT(openProject()));
    connect(ui->action_SaveProject, SIGNAL(triggered()), this, SLOT(saveProject()));
    connect(ui->action_SaveProjectAs, SIGNAL(triggered()), this, SLOT(saveProjectAs()));
    connect(ui->action_About, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->action_Exit, SIGNAL(triggered()), this, SLOT(close()));
    // Options
    connect(ui->action_UseEnglishLanguage, SIGNAL(triggered(bool)), this, SLOT(setLanguage(bool)));
    connect(ui->action_UseRussianLanguage, SIGNAL(triggered(bool)), this, SLOT(setLanguage(bool)));

    // UI
    connect(ui->action_ReloadUi, SIGNAL(triggered()), this, SLOT(reloadUi()));


    dm = new ROSolutesDataModel(_system.feedFlow(0)->solutes());
    ROSolutesDataView* dv = new ROSolutesDataView(dm, this);
    dv->setFixedWidth(320);
    updateFeeds();
    ui->comboBox_CurrentFeedNumber->setCurrentIndex(0);
    connect(ui->comboBox_CurrentFeedNumber, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeFeedNumber(int)));
    connect(ui->pushButton_AddFeed, SIGNAL(clicked()),
            this, SLOT(addFeed()));
    connect(ui->pushButton_RemoveFeed, SIGNAL(clicked()),
            this, SLOT(removeFeed()));
    ui->horizontalLayout_4->insertWidget(0, dv);

//    ui->tableView->setModel(dm);
//    int solutesTableWidth = ui->tableView->verticalHeader()->width() + 142; // 70 * 2 + 2
//    ui->tableView->setFixedWidth(solutesTableWidth);
//    int solutesTableHeight = ui->tableView->horizontalHeader()->height() +
//            ui->tableView->model()->rowCount() * 21 + 2;
//    ui->tableView->setFixedHeight(solutesTableHeight);
//    // end adjust tableview
//    QAbstractItemDelegate *doubleLineEditDelegate = new RODoubleLineEditDelegate();
//    ui->tableView->setItemDelegate(doubleLineEditDelegate);
//    ui->tableView->verticalHeader()->setClickable(false);
//    ui->tableView->horizontalHeader()->setClickable(false);

    //************** PARAMETERS CONSTRAINS
    ui->lineEdit_Temperature->setValidator(new QDoubleValidator(ui->lineEdit_Temperature));
    ui->lineEdit_pH->setValidator(new QDoubleValidator(ui->lineEdit_pH));
    connect(ui->lineEdit_Temperature, SIGNAL(textEdited(QString)),
            this, SLOT(setTemperature(QString)));
    connect(ui->lineEdit_pH, SIGNAL(textEdited(QString)),
            this, SLOT(setPH(QString)));


    //************* PARAMETERS ADJUST
    QObject::connect(ui->pushButton_AdjustCations, SIGNAL(clicked()),
                     this, SLOT(adjustCationsClicked()));
    QObject::connect(ui->pushButton_AdjustAnions, SIGNAL(clicked()),
                     this, SLOT(adjustAnionsClicked()));



//    QSqlQuery membranesQuery("SELECT series, model, diameter, length, area, productivity, rejection, pressure "
//                             "FROM ROMembranes INNER JOIN ROMembranesSeries ON ROMembranes.series_id = ROMembranesSeries.id;");
//    if (membranesQuery.isActive() && membranesQuery.isSelect()) {
//        while (membranesQuery.next()) {
//            ROStage::MEMBRANES << ROMembrane(membranesQuery.value(0).toString(), // Series
//                                       membranesQuery.value(1).toString(), // Model
//                                       ROMembraneSize(membranesQuery.value(2).toDouble(), // Diameter
//                                                      membranesQuery.value(3).toDouble()), // Length
//                                       membranesQuery.value(4).toDouble(), // Area
//                                       membranesQuery.value(5).toDouble(), // Productivity
//                                       membranesQuery.value(6).toDouble(), // Rejection
//                                       membranesQuery.value(7).toDouble()); // Pressure
//        }
//        membranesQuery.clear();
//    } else {
//        QMessageBox::critical(this, tr("Database error!"),
//                              tr("Cannot load membranes data."), QMessageBox::Close);
//    }







    //reloadUi();

    // provides an environment for instantiating QML components
    //QDeclarativeEngine engine = qmlView->engine();

    // creates the graphics item for QML at the engine's root context
    //QGraphicsObject *item = qmlView->rootObject(); //qobject_cast<QDeclarativeItem *>(component.create());

    //QDeclarativeItem *taaItem = item->findChild<QDeclarativeItem*>("totalActiveArea");


//    QWidget *widget = myExistingWidget();
//    QVBoxLayout *layout = new QVBoxLayout(widget);
//    layout->addWidget(qmlView);
    //bl = new QHBoxLayout();
    //bl->addWidget(qmlView);

    //engine->rootContext()->setContextProperty("pass", new MyObject());
    //bl->addItem(new QSpacerItem(50, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    //dynamic_cast<QVBoxLayout*>(ui->tab_Results->layout())->addLayout(bl);








//    membranesModel = new ROMembranesDataModel(ROStage::membraneList);
//    QObject::connect(ui->pushButton_ChooseMembraneType, SIGNAL(clicked()),
//                     this, SLOT(chooseMembraneType()));

//    //************* Compute results
//    QObject::connect(ui->tabWidget, SIGNAL(currentChanged(int)),
//                     this, SLOT(computeResult(int)));

//    // ------------- Init data
//    //ionsModel->setData(ionsModel->index(0, 1), 100);
//    //ionsModel->setData(ionsModel->index(1, 1), 100);
//    chosenMembraneIndex = 0;
//    ui->label_ChosenMembrane->setText(ROStage::membraneList.at(chosenMembraneIndex).seriesAndModel());

}

void MainWindow::setLanguage(bool checked) {
    if (!checked) {
        qobject_cast<QAction *>(sender())->setChecked(true);
        return;
    }
    QTranslator newLanguageTranslator;
    if (sender() == ui->action_UseEnglishLanguage) {
        //bool success = newLanguageTranslator.load("nano_en");
        //qDebug() << success;
        ui->action_UseRussianLanguage->setChecked(false);
    } else if (sender() == ui->action_UseRussianLanguage) {
        //bool success = newLanguageTranslator.load("nano_ru");
        ui->action_UseEnglishLanguage->setChecked(false);
        //qDebug() << success;
    }
    qApp->installTranslator(&newLanguageTranslator);
    ui->retranslateUi(this);
    //qobject_cast<ROSolutesDataModel *>(ui->tableView->model())->retranslate();
    //qobject_cast<ROMembranesDataModel *>(this->membranesModel)->retranslate();
}


//QList<ROSystemParameter> MainWindow::computeStageParameters(){
//    QList<ROSystemParameter> outputParameters;
//    // coeffs
//    double PSIToBar = 0.06894757;
//    double barToPSI = 14.5037744;
//    double m3ToL = 1000.028;
//    double LToM3 = 1 / m3ToL;

//    // Inputs Parameters
//    double Yi = _system.pass(0)->recovery(); // Степень извлечения пермеата для i –го элемента (D38)
//    double Qpi = 1.0;               // TODO Производительность пермеата  для i –го элемента (D2)
//    double Ppi = 0.0;               // TODO Давление со стороны пермеата
//    double Qf = Qpi / Yi;           // Изначальный входной поток (D5)
//    double Qr = 3.0;                // TODO Поток рециркуляции i –го элемента (D7)
//    double Qfi = Qf + Qr;           // Входной поток для i –го элемента (D4)
//    double Qci = Qfi - Qpi;         // Поток концентрата i –го элемента (D6)
//    double n = 1;                   // TODO Количество элементов в серии
//    double qfc = (Qfi + Qci) / 2;   // Среднеарифметическое поток через коцентрат=1/2(вх.поток+вых поток) (D43)
//    double T = ui->lineEdit_Temperature->text().toDouble();
//    double TCF = 0.0; // Фактор температурной коррекции (D13)
//    if (T >= 25.0)  TCF = pow(M_E, 2640.0 * (1.0 / 298.0 - 1.0 / (273.0 + T)));
//    else            TCF = pow(M_E, 3020.0 * (1.0 / 298.0 - 1.0 / (273.0 + T)));
//    double Cfi = 1;//qobject_cast<ROSolutesDataModel *>(ui->tableView->model())->totalDisolvedSolids(ROSolute::Meql);   // Концентрация на входе i –го элемента (D28)
//    double TDS = 2; //qobject_cast<ROSolutesDataModel *>(ui->tableView->model())->totalDisolvedSolids(ROSolute::Mgl);//ui->lineEdit_TDS->text().toDouble();

//    double PIf = 1.123 / 14.5 * (273.0 + T) * (Cfi * LToM3); // Осмотическое давление входной воды (D24)

//    double beta = Qpi / Qci; // ????????????????? (D46)
//    double SPm = 0.0877 * pow(TCF, 2); // Прохождение солей (D50)
//    double temp_H52 = pow(1 + beta, SPm); // H52
//    double SPsr = ((temp_H52 - 1) / (Yi * temp_H52 - Yi*(1 + beta) + beta)); // Прохождение солей c рециклом (D52)
//    double Ri = 1 - SPsr; // Селективность для i –го элемента без рецикла (D26)
//    double Cci = ((1-Yi * (1-Ri)) / (1-Yi)) * Cfi; // Концентрация концентрата для i –го элемента (D31)
//    double PIfi = PIf;  // Входное осмотическое давление для i –го элемента (D22)
//    double PIc = 1.123 / 14.5 * (273.0 + T) * (Cci * LToM3); // Осмотическое давление концентрата входной воды (E24)

//    double pfi = exp(0.7 * Yi) * 1.0; // TODO Фактор КП для i –го элемента (D25)
//    double Cfir = (Cfi * Qf + Cci * Qr) / Qfi; // Концентрация на входе i –го элемента после рецикла (D29)
//    double PIi = PIfi * pfi * Cci / Cfir; // Среднее осмотическое давление со стороны концентрата (D21)

//    double PI = (PIc + PIi) / 2 * barToPSI; // (F20)
//    double A = 0.0; // Проницаемость мембраны (D9-D11)
//    if (PI <= 25.0) A = 0.125;
//    else if (PI <= 200) A = 0.125 - 0.011 * (PI - 25.0) / 35.0;
//    else A = 0.07 - 0.0001 * (PI - 200);
//    A *= 24.6236; // TODO Коэф. перевода из gfd в л/м2/час/бар

//    double dPfci = 0.04 * PSIToBar * n * pow(qfc, 2); // Перепад давления для i –го элемента (со стороны концентрата) (D19)
//    double PIpi = PIfi * (1 - Ri); // Осмотическое давление со стороны пермеата для i –го элемента (D23)
//    double S = _system.pass(0)->stage(0)->membrane()->activeArea(); // Площадь мембранного элемента (D12)
//    double FF = 1.0; // TODO Фактор мембранного забивания (D15)
//    double PIci = PIc;
//    double Pfi = (PIci - PIpi) + (Ppi + dPfci / 2) + (Qpi * m3ToL) / (A * S * TCF * FF); // Давление на входе i –го элемента (D16-D18)

//    double Qmi = 1.0; // Средняя производительность одного элемента  Qmi = Q / Ne (D42)
//    double F = (Qmi * m3ToL) / S; // Удельный съем, средний (Flux) (D47)
//    double Qc = Qci - Qr;
//    double Cpi = (Qf * Cfi - Qc * Cci) / Qpi;
//    double TDSp = Cpi * TDS / Cfi;
//    outputParameters.append(ROSystemParameter(tr("Stage"), 1, QObject::tr("")));
//    outputParameters.append(ROSystemParameter(QObject::tr("Element"), _system.pass(0)->stage(0)->membrane()->seriesAndModel(), ""));
//    outputParameters.append(ROSystemParameter(QObject::tr("#PV"), 1, ""));
//    outputParameters.append(ROSystemParameter(QObject::tr("#El"), 1, ""));
//    outputParameters.append(ROSystemParameter(QObject::tr("Feed flow"), Qfi, "gpm"));
//    outputParameters.append(ROSystemParameter(QObject::tr("Feed press"), Pfi, "bar"));
//    outputParameters.append(ROSystemParameter(QObject::tr("Recirc flow"), Qr, "m3/h"));
//    outputParameters.append(ROSystemParameter(QObject::tr("Concentrate flow"), Qci, "m3/h"));
//    outputParameters.append(ROSystemParameter(QObject::tr("Concentrate press"), Pfi - dPfci, "bar"));
//    outputParameters.append(ROSystemParameter(QObject::tr("Permeate flow"), Qpi, "m3/h"));
//    outputParameters.append(ROSystemParameter(QObject::tr("Average flux"), F, "lmh"));
//    outputParameters.append(ROSystemParameter(QObject::tr("Permeate TDS"), TDSp, "mg/l"));

//    return outputParameters;
//}

//QList<RODoubleSystemParameter*> MainWindow::computeSystemParameters(){

//    QList<RODoubleSystemParameter*> outputParameters;
//    // coeffs
//    double PSIToBar = 0.06894757;
//    double barToPSI = 14.5037744;
//    double m3ToL = 1000.028;
//    double LToM3 = 1 / m3ToL;

//    // Inputs Parameters
//    double Yi = _system.pass(0)->recovery(); // Степень извлечения пермеата для i –го элемента (D38)
//    double Qpi = 1.0;               // TODO Производительность пермеата  для i –го элемента (D2)
//    double Ppi = 0.0;               // TODO Давление со стороны пермеата
//    double Qf = Qpi / Yi;           // Изначальный входной поток (D5)
//    double Qr = 3.0;                // TODO Поток рециркуляции i –го элемента (D7)
//    double Qfi = Qf + Qr;           // Входной поток для i –го элемента (D4)
//    double Qci = Qfi - Qpi;         // Поток концентрата i –го элемента (D6)
//    double n = 1;                   // TODO Количество элементов в серии
//    double qfc = (Qfi + Qci) / 2;   // Среднеарифметическое поток через коцентрат=1/2(вх.поток+вых поток) (D43)
//    double T = ui->lineEdit_Temperature->text().toDouble();
//    double TCF = 0.0; // Фактор температурной коррекции (D13)
//    if (T >= 25.0)  TCF = pow(M_E, 2640.0 * (1.0 / 298.0 - 1.0 / (273.0 + T)));
//    else            TCF = pow(M_E, 3020.0 * (1.0 / 298.0 - 1.0 / (273.0 + T)));
//    double Cfi = 1;//qobject_cast<ROSolutesDataModel *>(ui->tableView->model())->totalDisolvedSolids(ROSolute::Meql);   // Концентрация на входе i –го элемента (D28)
//    //double TDS = qobject_cast<ROSolutesDataModel *>(ui->tableView->model())->totalDisolvedSolids(ROSolute::Mgl);
//    double PIf = 1.123 / 14.5 * (273.0 + T) * (Cfi * LToM3); // Осмотическое давление входной воды (D24)

//    double beta = Qpi / Qci; // ????????????????? (D46)
//    double SPm = 0.0877 * pow(TCF, 2); // Прохождение солей (D50)
//    double temp_H52 = pow(1 + beta, SPm); // H52
//    double SPsr = ((temp_H52 - 1) / (Yi * temp_H52 - Yi*(1 + beta) + beta)); // Прохождение солей c рециклом (D52)
//    double Ri = 1 - SPsr; // Селективность для i –го элемента без рецикла (D26)
//    double Cci = ((1-Yi * (1-Ri)) / (1-Yi)) * Cfi; // Концентрация концентрата для i –го элемента (D31)
//    double PIfi = PIf;  // Входное осмотическое давление для i –го элемента (D22)
//    double PIc = 1.123 / 14.5 * (273.0 + T) * (Cci * LToM3); // Осмотическое давление концентрата входной воды (E24)

//    double pfi = exp(0.7 * Yi) * 1.0; // TODO Фактор КП для i –го элемента (D25)
//    double Cfir = (Cfi * Qf + Cci * Qr) / Qfi; // Концентрация на входе i –го элемента после рецикла (D29)
//    double PIi = PIfi * pfi * Cci / Cfir; // Среднее осмотическое давление со стороны концентрата (D21)

//    double PI = (PIc + PIi) / 2 * barToPSI; // (F20)
//    double A = 0.0; // Проницаемость мембраны (D9-D11)
//    if (PI <= 25.0) A = 0.125;
//    else if (PI <= 200) A = 0.125 - 0.011 * (PI - 25.0) / 35.0;
//    else A = 0.07 - 0.0001 * (PI - 200);
//    A *= 24.6236; // TODO Коэф. перевода из gfd в л/м2/час/бар

//    double dPfci = 0.04 * PSIToBar * n * pow(qfc, 2); // Перепад давления для i –го элемента (со стороны концентрата) (D19)
//    double PIpi = PIfi * (1 - Ri); // Осмотическое давление со стороны пермеата для i –го элемента (D23)
//    double S = _system.pass(0)->stage(0)->membrane()->activeArea(); // Площадь мембранного элемента (D12)
//    double FF = 1.0; // TODO Фактор мембранного забивания (D15)
//    double PIci = PIc;
//    double Pfi = (PIci - PIpi) + (Ppi + dPfci / 2) + (Qpi * m3ToL) / (A * S * TCF * FF); // Давление на входе i –го элемента (D16-D18)

//    double Qmi = 1.0; // Средняя производительность одного элемента  Qmi = Q / Ne (D42)
//    double F = (Qmi * m3ToL) / S; // Удельный съем, средний (Flux) (D47)

////    outputParameters.append(new RODoubleSystemParameter(QObject::tr("Feed flow to stage 1"), Qfi, QStringList() << QObject::tr("gpm")));
////    outputParameters.append(new RODoubleSystemParameter(QObject::tr("Raw water flow to system"), Qf, QStringList() << QObject::tr("gpm")));
////    outputParameters.append(new RODoubleSystemParameter(QObject::tr("Feed pressure"), Pfi, QStringList() << QObject::tr("m3/h")));
////    outputParameters.append(new RODoubleSystemParameter(QObject::tr("Flow factor"), FF, QStringList() << QObject::tr("")));
////    outputParameters.append(new RODoubleSystemParameter(QObject::tr("Total active area"), S, QStringList() << QObject::tr("m2")));
////    outputParameters.append(new RODoubleSystemParameter(QObject::tr("Pass 1 permeate flow"), Qpi, QStringList() << QObject::tr("m3/h")));
////    outputParameters.append(new RODoubleSystemParameter(QObject::tr("Pass 1 recovery"), Yi, QStringList() << QObject::tr("%")));
////    outputParameters.append(new RODoubleSystemParameter(QObject::tr("Feed temperature"), T, QStringList() << QObject::tr("°C")));
////!    outputParameters.append(new RODoubleSystemParameter(QObject::tr("Feed TDS"), TDS, QStringList() << QObject::tr("mg/l")));
////    outputParameters.append(new RODoubleSystemParameter(QObject::tr("Number of elements"), n, QStringList() << QObject::tr("")));
////!    outputParameters.append(new RODoubleSystemParameter(QObject::tr("Average pass 1 flux"), F, QStringList() << QObject::tr("lmh")));
////!    outputParameters.append(new RODoubleSystemParameter(QObject::tr("Feed"), PIf, QStringList() << QObject::tr("bar")));
////!    outputParameters.append(new RODoubleSystemParameter(QObject::tr("Concentrate"), PIc, QStringList() << QObject::tr("bar")));
////!    outputParameters.append(new RODoubleSystemParameter(QObject::tr("Average"), (PIf + PIc) / 2, QStringList() << QObject::tr("bar")));
////    outputParameters.append(new RODoubleSystemParameter(QObject::tr("Average NDP"), 0.0, QStringList() << QObject::tr("bar")));

//    return outputParameters;
//}

void MainWindow::computeResult(int index){
    if (index != 4) return; // Not result page
    QDomDocument *doc = new QDomDocument("Results");
    QDomElement html = doc->createElement("html");
    doc->appendChild(html);
    QDomElement body = doc->createElement("body");
    html.appendChild(body);

    QDomElement systemDetails_p = doc->createElement("p");
    systemDetails_p.setAttribute("style", "text-weight: bold");
    QDomText systemDetails_text = doc->createTextNode("System Details");
    systemDetails_p.appendChild(systemDetails_text);
    body.appendChild(systemDetails_p);

    QDomElement systemParameters_table = doc->createElement("table");
    systemParameters_table.setAttribute("width", "70%");
    systemParameters_table.setAttribute("border", 1);
    systemParameters_table.setAttribute("rules", "rows");
    //systemParameters_table.setAttribute("frame", "box");
    systemParameters_table.setAttribute("style", "border-collapse: collapse");
    QList<RODoubleSystemParameter*> outputParameters;// = computeSystemParameters();
    for (int i = 0; i < outputParameters.size(); ++i){
        // TODO vinesti v funkciu
        QDomElement parameterName = doc->createElement("td");
        parameterName.appendChild(doc->createTextNode(outputParameters.at(i)->name() + ": "));
        QDomElement parameterValue = doc->createElement("td");
        parameterValue.setAttribute("align", "right");
        parameterValue.appendChild(doc->createTextNode(QString::number(outputParameters.at(i)->value())));
        QDomElement parameterUnits = doc->createElement("td");
       // domParser.setContent(outputParameters.at(i).units());
        parameterUnits.appendChild(doc->createTextNode(outputParameters.at(i)->unitsString()));
        QDomElement parameterRow = doc->createElement("tr");
        parameterRow.appendChild(parameterName);
        parameterRow.appendChild(parameterValue);
        parameterRow.appendChild(parameterUnits);
        systemParameters_table.appendChild(parameterRow);
        delete outputParameters[i];
    }
    body.appendChild(systemParameters_table);

    body.appendChild(doc->createElement("br"));

    QDomElement stageParameters_table = doc->createElement("table");
    stageParameters_table.setAttribute("width", "70%");
    stageParameters_table.setAttribute("border", 1);
    stageParameters_table.setAttribute("rules", "rows");
    stageParameters_table.setAttribute("style", "border-collapse: collapse");
    QList<ROSystemParameter> stageParameters;// = computeStageParameters();
    for (int i = 0; i < stageParameters.size(); ++i){
        // TODO vinesti v funkciu
        QDomElement parameterName = doc->createElement("td");
        parameterName.appendChild(doc->createTextNode(stageParameters.at(i).name() + ": "));
        QDomElement parameterValue = doc->createElement("td");
        parameterValue.setAttribute("align", "right");
        parameterValue.appendChild(doc->createTextNode(stageParameters.at(i).value().toString()));
        QDomElement parameterUnits = doc->createElement("td");
       // domParser.setContent(outputParameters.at(i).units());
        parameterUnits.appendChild(doc->createTextNode(stageParameters.at(i).units()));
        QDomElement parameterRow = doc->createElement("tr");
        parameterRow.appendChild(parameterName);
        parameterRow.appendChild(parameterValue);
        parameterRow.appendChild(parameterUnits);
        stageParameters_table.appendChild(parameterRow);
    }
    body.appendChild(stageParameters_table);

    body.appendChild(doc->createElement("br"));

    QDomElement ionsParameters_table = doc->createElement("table");
    ionsParameters_table.setAttribute("width", "70%");
    ionsParameters_table.setAttribute("border", 1);
    ionsParameters_table.setAttribute("style", "border-collapse: collapse");
    QDomElement ionsParametersTableHeader_td = doc->createElement("td");
    QDomElement ionsParametersTableHeader_tr1 = doc->createElement("tr");
    ionsParametersTableHeader_td.setAttribute("colspan", 7);
    ionsParametersTableHeader_td.setAttribute("align", "center");
    ionsParametersTableHeader_td.appendChild(doc->createTextNode("Pass Streams"));
    ionsParametersTableHeader_tr1.appendChild(ionsParametersTableHeader_td);
    ionsParameters_table.appendChild(ionsParametersTableHeader_tr1);

    QDomElement ionsParametersTableHeader_tr2 = doc->createElement("tr");
    QDomElement ionsParametersTableHeaderName_td = doc->createElement("td");
    ionsParametersTableHeaderName_td.setAttribute("align", "center");
    ionsParametersTableHeaderName_td.setAttribute("valign", "middle");
    ionsParametersTableHeaderName_td.setAttribute("rowspan", 2);
    ionsParametersTableHeaderName_td.appendChild(doc->createTextNode("Ion name"));
    ionsParametersTableHeader_tr2.appendChild(ionsParametersTableHeaderName_td);
    QDomElement ionsParametersTableHeaderFeed_td = doc->createElement("td");
    ionsParametersTableHeaderFeed_td.setAttribute("align", "center");
    ionsParametersTableHeaderFeed_td.setAttribute("valign", "middle");
    ionsParametersTableHeaderFeed_td.setAttribute("rowspan", 2);
    ionsParametersTableHeaderFeed_td.appendChild(doc->createTextNode("Feed"));
    ionsParametersTableHeader_tr2.appendChild(ionsParametersTableHeaderFeed_td);
    QDomElement ionsParametersTableHeaderAdjustedFeed_td = doc->createElement("td");
    ionsParametersTableHeaderAdjustedFeed_td.setAttribute("align", "center");
    ionsParametersTableHeaderAdjustedFeed_td.setAttribute("valign", "middle");
    //ionsParametersTableHeaderAdjustedFeed_td.setAttribute("rowspan", 2);
    ionsParametersTableHeaderAdjustedFeed_td.setAttribute("colspan", 2);
    ionsParametersTableHeaderAdjustedFeed_td.appendChild(doc->createTextNode("Adjusted feed"));
    ionsParametersTableHeader_tr2.appendChild(ionsParametersTableHeaderAdjustedFeed_td);
    QDomElement ionsParametersTableHeaderConcentrate_td = doc->createElement("td");
    ionsParametersTableHeaderConcentrate_td.setAttribute("align", "center");
    ionsParametersTableHeaderConcentrate_td.appendChild(doc->createTextNode("Concentrate"));
    ionsParametersTableHeader_tr2.appendChild(ionsParametersTableHeaderConcentrate_td);
    QDomElement ionsParametersTableHeaderPermeate_td = doc->createElement("td");
    ionsParametersTableHeaderPermeate_td.setAttribute("align", "center");
    ionsParametersTableHeaderPermeate_td.setAttribute("colspan", 2);
    ionsParametersTableHeaderPermeate_td.appendChild(doc->createTextNode("Permeate"));
    ionsParametersTableHeader_tr2.appendChild(ionsParametersTableHeaderPermeate_td);
    ionsParameters_table.appendChild(ionsParametersTableHeader_tr2);

    QDomElement ionsParametersTableHeader_tr3 = doc->createElement("tr");
    QDomElement ionsParametersTableHeaderAdjustedFeedK_td = doc->createElement("td");
    ionsParametersTableHeaderAdjustedFeedK_td.setAttribute("align", "center");
    ionsParametersTableHeaderAdjustedFeedK_td.appendChild(doc->createTextNode("K"));
    ionsParametersTableHeader_tr3.appendChild(ionsParametersTableHeaderAdjustedFeedK_td);
    QDomElement ionsParametersTableHeaderAdjustedFeedRecycle_td = doc->createElement("td");
    ionsParametersTableHeaderAdjustedFeedRecycle_td.setAttribute("align", "center");
    ionsParametersTableHeaderAdjustedFeedRecycle_td.appendChild(doc->createTextNode("Recycle"));
    ionsParametersTableHeader_tr3.appendChild(ionsParametersTableHeaderAdjustedFeedRecycle_td);
    QDomElement ionsParametersTableHeaderStage1Concentrate_td = doc->createElement("td");
    ionsParametersTableHeaderStage1Concentrate_td.setAttribute("align", "center");
    ionsParametersTableHeaderStage1Concentrate_td.appendChild(doc->createTextNode("Stage 1"));
    ionsParametersTableHeader_tr3.appendChild(ionsParametersTableHeaderStage1Concentrate_td);
    QDomElement ionsParametersTableHeaderStage1Permeate_td = doc->createElement("td");
    ionsParametersTableHeaderStage1Permeate_td.setAttribute("align", "center");
    ionsParametersTableHeaderStage1Permeate_td.appendChild(doc->createTextNode("Stage 1"));
    ionsParametersTableHeader_tr3.appendChild(ionsParametersTableHeaderStage1Permeate_td);
    QDomElement ionsParametersTableHeaderTotalPermeate_td = doc->createElement("td");
    ionsParametersTableHeaderTotalPermeate_td.setAttribute("align", "center");
    ionsParametersTableHeaderTotalPermeate_td.appendChild(doc->createTextNode("Total"));
    ionsParametersTableHeader_tr3.appendChild(ionsParametersTableHeaderTotalPermeate_td);
    ionsParameters_table.appendChild(ionsParametersTableHeader_tr3);

    // coeffs
    double PSIToBar = 0.06894757;
    double barToPSI = 14.5037744;
    double m3ToL = 1000.028;
    double LToM3 = 1 / m3ToL;

    // Inputs Parameters
    double Yi = _system.pass(0)->recovery(); // Степень извлечения пермеата для i –го элемента (D38)
    double Qpi = 1.0;               // TODO Производительность пермеата  для i –го элемента (D2)
    double Ppi = 0.0;               // TODO Давление со стороны пермеата
    double Qf = Qpi / Yi;           // Изначальный входной поток (D5)
    double Qr = 3.0;                // TODO Поток рециркуляции i –го элемента (D7)
    double Qfi = Qf + Qr;           // Входной поток для i –го элемента (D4)
    double Qci = Qfi - Qpi;         // Поток концентрата i –го элемента (D6)
    double n = 1;                   // TODO Количество элементов в серии
    double qfc = (Qfi + Qci) / 2;   // Среднеарифметическое поток через коцентрат=1/2(вх.поток+вых поток) (D43)
    double T = ui->lineEdit_Temperature->text().toDouble();
    double TCF = 0.0; // Фактор температурной коррекции (D13)
    if (T >= 25.0)  TCF = pow(M_E, 2640.0 * (1.0 / 298.0 - 1.0 / (273.0 + T)));
    else            TCF = pow(M_E, 3020.0 * (1.0 / 298.0 - 1.0 / (273.0 + T)));
    double Cfi = 1; //qobject_cast<ROSolutesDataModel *>(ui->tableView->model())->totalDisolvedSolids(ROSolute::Meql);   // Концентрация на входе i –го элемента (D28)
    double TDS = 2; //qobject_cast<ROSolutesDataModel *>(ui->tableView->model())->totalDisolvedSolids(ROSolute::Mgl);;
    double PIf = 1.123 / 14.5 * (273.0 + T) * (Cfi * LToM3); // Осмотическое давление входной воды (D24)

    double beta = Qpi / Qci; // ????????????????? (D46)
    double SPm = 0.0877 * pow(TCF, 2); // Прохождение солей (D50)
    double temp_H52 = pow(1 + beta, SPm); // H52
    double SPsr = ((temp_H52 - 1) / (Yi * temp_H52 - Yi*(1 + beta) + beta)); // Прохождение солей c рециклом (D52)
    double Ri = 1 - SPsr; // Селективность для i –го элемента без рецикла (D26)
    double Cci = ((1-Yi * (1-Ri)) / (1-Yi)) * Cfi; // Концентрация концентрата для i –го элемента (D31)
    double PIfi = PIf;  // Входное осмотическое давление для i –го элемента (D22)
    double PIc = 1.123 / 14.5 * (273.0 + T) * (Cci * LToM3); // Осмотическое давление концентрата входной воды (E24)

    double pfi = exp(0.7 * Yi) * 1.0; // TODO Фактор КП для i –го элемента (D25)
    double Cfir = (Cfi * Qf + Cci * Qr) / Qfi; // Концентрация на входе i –го элемента после рецикла (D29)
    double PIi = PIfi * pfi * Cci / Cfir; // Среднее осмотическое давление со стороны концентрата (D21)

    double PI = (PIc + PIi) / 2 * barToPSI; // (F20)
    double A = 0.0; // Проницаемость мембраны (D9-D11)
    if (PI <= 25.0) A = 0.125;
    else if (PI <= 200) A = 0.125 - 0.011 * (PI - 25.0) / 35.0;
    else A = 0.07 - 0.0001 * (PI - 200);
    A *= 24.6236; // TODO Коэф. перевода из gfd в л/м2/час/бар

    double dPfci = 0.04 * PSIToBar * n * pow(qfc, 2); // Перепад давления для i –го элемента (со стороны концентрата) (D19)
    double PIpi = PIfi * (1 - Ri); // Осмотическое давление со стороны пермеата для i –го элемента (D23)
    double S = _system.pass(0)->stage(0)->membrane()->activeArea(); // Площадь мембранного элемента (D12)
    double FF = 1.0; // TODO Фактор мембранного забивания (D15)
    double PIci = PIc;
    double Pfi = (PIci - PIpi) + (Ppi + dPfci / 2) + (Qpi * m3ToL) / (A * S * TCF * FF); // Давление на входе i –го элемента (D16-D18)

    double Qmi = 1.0; // Средняя производительность одного элемента  Qmi = Q / Ne (D42)
    double F = (Qmi * m3ToL) / S; // Удельный съем, средний (Flux) (D47)
    double Qc = Qci - Qr;
    double Cpi = (Qf * Cfi - Qc * Cci) / Qpi;
    double TDSp = Cpi * TDS / Cfi;
    double K = Cfir / Cfi;

//    for (int ionIndex = 0; ionIndex < ionsList.size(); ++ionIndex) {
//        double ionEqMass = ionsModel->data(ionsModel->index(ionIndex, 0)).toDouble();
//        QDomElement ionsParametersTableIonRecord_tr = doc->createElement("tr");
//        QDomElement ionsParametersTableIonName_td = doc->createElement("td");
//        ionsParametersTableIonName_td.appendChild(doc->createTextNode(ionsList.at(ionIndex).name));
//        ionsParametersTableIonRecord_tr.appendChild(ionsParametersTableIonName_td);

//        QDomElement ionsParametersTableIonFeed_td = doc->createElement("td");
//        ionsParametersTableIonFeed_td.setAttribute("align", "right");
//        ionsParametersTableIonFeed_td.appendChild(doc->createTextNode(QString::number(ionEqMass)));
//        ionsParametersTableIonRecord_tr.appendChild(ionsParametersTableIonFeed_td);

//        QDomElement ionsParametersTableIonK_td = doc->createElement("td");
//        ionsParametersTableIonK_td.setAttribute("align", "right");
//        ionsParametersTableIonK_td.appendChild(doc->createTextNode(QString::number(K)));
//        ionsParametersTableIonRecord_tr.appendChild(ionsParametersTableIonK_td);

//        QDomElement ionsParametersTableIonRecycle_td = doc->createElement("td");
//        ionsParametersTableIonRecycle_td.setAttribute("align", "right");
//        ionsParametersTableIonRecycle_td.appendChild(doc->createTextNode(QString::number(K * ionEqMass)));
//        ionsParametersTableIonRecord_tr.appendChild(ionsParametersTableIonRecycle_td);

//        QDomElement ionsParametersTableIonConcentrate_td = doc->createElement("td");
//        ionsParametersTableIonConcentrate_td.setAttribute("align", "right");
//        ionsParametersTableIonConcentrate_td.appendChild(doc->createTextNode("Unknown"));
//        ionsParametersTableIonRecord_tr.appendChild(ionsParametersTableIonConcentrate_td);

//        QDomElement ionsParametersTableIonPermeateStage1_td = doc->createElement("td");
//        ionsParametersTableIonPermeateStage1_td.setAttribute("align", "right");
//        ionsParametersTableIonPermeateStage1_td.appendChild(doc->createTextNode("Unknown"));
//        ionsParametersTableIonRecord_tr.appendChild(ionsParametersTableIonPermeateStage1_td);

//        QDomElement ionsParametersTableIonPermeateTotal_td = doc->createElement("td");
//        ionsParametersTableIonPermeateTotal_td.setAttribute("align", "right");
//        ionsParametersTableIonPermeateTotal_td.appendChild(doc->createTextNode(QString::number(SPsr * ionEqMass)));
//        ionsParametersTableIonRecord_tr.appendChild(ionsParametersTableIonPermeateTotal_td);

//        ionsParameters_table.appendChild(ionsParametersTableIonRecord_tr);
//    }


    QDomElement ionsParametersTableTDSRecord_tr = doc->createElement("tr");
    QDomElement ionsParametersTableTDSName_td = doc->createElement("td");
    ionsParametersTableTDSName_td.appendChild(doc->createTextNode("TDS"));
    ionsParametersTableTDSRecord_tr.appendChild(ionsParametersTableTDSName_td);

    QDomElement ionsParametersTableTDSFeed_td = doc->createElement("td");
    ionsParametersTableTDSFeed_td.setAttribute("align", "right");
    ionsParametersTableTDSFeed_td.appendChild(doc->createTextNode(QString::number(TDS)));
    ionsParametersTableTDSRecord_tr.appendChild(ionsParametersTableTDSFeed_td);

    QDomElement ionsParametersTableTDSK_td = doc->createElement("td");
    ionsParametersTableTDSK_td.setAttribute("align", "right");
    ionsParametersTableTDSK_td.appendChild(doc->createTextNode(""));
    ionsParametersTableTDSRecord_tr.appendChild(ionsParametersTableTDSK_td);

    QDomElement ionsParametersTableTDSRecycle_td = doc->createElement("td");
    ionsParametersTableTDSRecycle_td.setAttribute("align", "right");
    ionsParametersTableTDSRecycle_td.appendChild(doc->createTextNode(QString::number(K * TDS)));
    ionsParametersTableTDSRecord_tr.appendChild(ionsParametersTableTDSRecycle_td);

    QDomElement ionsParametersTableTDSConcentrate_td = doc->createElement("td");
    ionsParametersTableTDSConcentrate_td.setAttribute("align", "right");
    ionsParametersTableTDSConcentrate_td.appendChild(doc->createTextNode(QString::number(Cci * TDS / Cfi)));
    ionsParametersTableTDSRecord_tr.appendChild(ionsParametersTableTDSConcentrate_td);

    QDomElement ionsParametersTableTDSPermeateStage1_td = doc->createElement("td");
    ionsParametersTableTDSPermeateStage1_td.setAttribute("align", "right");
    ionsParametersTableTDSPermeateStage1_td.appendChild(doc->createTextNode("Unknown"));
    ionsParametersTableTDSRecord_tr.appendChild(ionsParametersTableTDSPermeateStage1_td);

    QDomElement ionsParametersTableTDSPermeateTotal_td = doc->createElement("td");
    ionsParametersTableTDSPermeateTotal_td.setAttribute("align", "right");
    ionsParametersTableTDSPermeateTotal_td.appendChild(doc->createTextNode(QString::number(TDSp)));
    ionsParametersTableTDSRecord_tr.appendChild(ionsParametersTableTDSPermeateTotal_td);

    ionsParameters_table.appendChild(ionsParametersTableTDSRecord_tr);
    body.appendChild(ionsParameters_table);
    QString res = doc->toString();
    //ui->resultView->setHtml(doc->toString());
    //QDomDocument
}


//void MainWindow::chooseMembraneType() {
//    QFrame* membranesForm = new QFrame(this, Qt::Popup | Qt::Window);
//    //membranesForm->setFrameStyle(QFrame::Panel | QFrame::Plain);
//    QPushButton *chooseButton = qobject_cast<QPushButton*>(QObject::sender());

//    QTableView *membranesView = new QTableView(membranesForm);

//    QVBoxLayout *membranesFormLayout = new QVBoxLayout(membranesForm);
//    membranesFormLayout->setContentsMargins(0, 0, 0, 0);
//    membranesFormLayout->addWidget(membranesView);

//    membranesView->verticalHeader()->setVisible(false);
//    membranesView->horizontalHeader()->setClickable(false);
//    membranesView->setSelectionMode(QAbstractItemView::SingleSelection);
//    membranesView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
//    membranesView->setSelectionBehavior(QAbstractItemView::SelectRows);
//    membranesView->setMinimumSize(400, 200);
//    membranesView->horizontalHeader()->setDefaultSectionSize(100);
//    membranesView->horizontalHeader()->setHighlightSections(false);
//    membranesView->horizontalHeader()->setMinimumSectionSize(50);
//    membranesView->horizontalHeader()->setStretchLastSection(true);
//    membranesView->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    QObject::connect(membranesView, SIGNAL(activated(const QModelIndex &)),
//                     this,          SLOT(selectMembraneType(const QModelIndex &)));

//    membranesView->setModel(membranesModel);

//    // TODO tanci s bubnom, a vse bi tak prosto...
//    //QPoint membranesFormTopLeftCorner = QWidget::mapToGlobal(chooseButton->geometry().bottomRight());
//    QPoint membranesFormTopLeftCorner;
//    QWidget *chainWidget = chooseButton;
//    while (chainWidget) {
//        membranesFormTopLeftCorner += chainWidget->geometry().topLeft();
//        chainWidget = chainWidget->parentWidget();
//    }
//    membranesFormTopLeftCorner.setX(membranesFormTopLeftCorner.x() + chooseButton->width());
//    //membranesFormTopLeftCorner.setY(membranesFormTopLeftCorner.y() + chooseButton->height());
//    membranesForm->move(membranesFormTopLeftCorner);
//    membranesForm->show();

//}

//void MainWindow::selectMembraneType(const QModelIndex & index) {
//    QTableView *membranesView = qobject_cast<QTableView*>(QObject::sender());

//    chosenMembraneIndex = index.row();
//    ui->label_ChosenMembrane->setText(ROStage::membraneList.at(chosenMembraneIndex).seriesAndModel());
//    ui->label_ChosenMembrane->setToolTip("Active Area: " + QString::number(ROStage::membraneList.at(chosenMembraneIndex).activeArea()));

//    membranesView->window()->close();
//}


//****************** TODO mb slit' v odno ?
void MainWindow::adjustCationsClicked(){
    dm->adjustCations();
}

void MainWindow::adjustAnionsClicked(){
    dm->adjustAnions();
}
//------------------ END TODO



void MainWindow::newProject() {
    if (maybeSave()) {
        // TODO vse polya na minus
        setCurrentFile("");
    }
}

bool MainWindow::saveProject() {
    if (curFile.isEmpty()) {
        return saveProjectAs();
    } else {
        return saveFile(curFile);
    }
}

bool MainWindow::saveProjectAs() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "",
                                                    tr("Nano Files (*.nano)"));
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

void MainWindow::openProject() {
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
                                                        tr("Nano Files (*.nano)"));
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (maybeSave()) {
        //writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About Nano"),
                 tr("Version #0.000001"));
}

bool MainWindow::maybeSave() {
    if (documentModified) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Application"),
                                   tr("The document has been modified.\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return saveProject();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void MainWindow::loadFile(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    loadProjectFromXMLFile(&file);
    file.close();

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    ui->statusBar->showMessage(tr("File loaded"), 2000);

}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    documentModified = false;
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled";
    setWindowFilePath(shownName);
}

bool MainWindow::saveFile(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    saveProjectToXMLFile(&file);
    file.close();

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    ui->statusBar->showMessage(tr("File saved"), 2000);
    return true;

}

void MainWindow::loadProjectFromXMLFile(QFile *xmlFile) {
    QDomDocument projectXML;
    QString errorMsg;
    int errLin, errCol;
    if (!projectXML.setContent(xmlFile, &errorMsg, &errLin, &errCol)) {
        QMessageBox::critical(this, tr("Error"), tr("Invalid file format: %1 on (%2, %3)")
                              .arg(errorMsg)
                              .arg(errLin)
                              .arg(errCol));
        return;
    }
    QDomElement e_Nano = projectXML.documentElement();
    QDomElement e_fileDescription = e_Nano.firstChildElement("File");
    QDomElement e_projectInfo = e_Nano.firstChildElement("Project");
    QString tt = e_projectInfo.firstChildElement("Name").firstChild().nodeValue();
    ui->lineEdit_ProjectName->setText(e_projectInfo.firstChildElement("Name").firstChild().nodeValue());
    ui->lineEdit_ProjectAuthor->setText(e_projectInfo.firstChildElement("Author").firstChild().nodeValue());
    ui->lineEdit_ProjectCompany->setText(e_projectInfo.firstChildElement("Company").firstChild().nodeValue());
    ui->plainTextEdit_ProjectNotes->document()->setPlainText(e_projectInfo.firstChildElement("Notes").firstChild().nodeValue());

    QDomElement e_ProjectCase = e_projectInfo.firstChildElement("Case");
    QDomElement e_Feed = e_ProjectCase.firstChildElement("Feed");
    ui->lineEdit_pH->setText(e_Feed.firstChildElement("pH").firstChild().nodeValue());
    ui->lineEdit_Temperature->setText(e_Feed.firstChildElement("Temperature").firstChild().nodeValue());
    ui->comboBox_WaterType->setCurrentIndex(e_Feed.firstChildElement("WaterType").attribute("Number").toInt());
    QDomElement e_IonIter = e_Feed.firstChildElement("Ions").firstChildElement();
//    while (!e_IonIter.isNull()) {
//        int ionRow = e_IonIter.attribute("Number").toInt();
//        double ionConcentration = e_IonIter.attribute("Concentration").toDouble();
//        ionsModel->setData(ionsModel->index(ionRow, 0), ionConcentration);
//        e_IonIter = e_IonIter.nextSiblingElement();
//    }

    QDomElement e_Pass = e_ProjectCase.firstChildElement("Pass");
//    ui->lineEdit_PermeateFlow->setText(e_Pass.firstChildElement("PermeateFlow").firstChild().nodeValue());
//    ui->lineEdit_Recovery->setText(e_Pass.firstChildElement("Recovery").firstChild().nodeValue());
//    ui->lineEdit_FeedFlow->setText(e_Pass.firstChildElement("FeedFlow").firstChild().nodeValue());
    QDomElement e_Stage = e_Pass.firstChildElement("Stage");
    //chosenMembraneIndex = e_Stage.firstChildElement("Membrane").attribute("Number").toInt();
    //ui->label_ChosenMembrane->setText(ROStage::membraneList.at(chosenMembraneIndex).seriesAndModel());
}

bool MainWindow::saveProjectToXMLFile(QFile *xmlFile) {
    QDomDocument projectXML;
    // File Info
    QDomElement e_Nano = projectXML.createElement("Nano");
    QDomElement e_fileDescription = projectXML.createElement("File");
    e_fileDescription.setAttribute("Type", "Nano");
    e_fileDescription.setAttribute("Version", 0.001);
    e_fileDescription.setAttribute("LastChangeDate", QDateTime::currentDateTime().toString());
    e_Nano.appendChild(e_fileDescription);

    // Project Info
    QDomElement e_projectInfo = projectXML.createElement("Project");
    QDomElement e_projectName = projectXML.createElement("Name");
    QString projectName = ui->lineEdit_ProjectName->text();
    e_projectName.appendChild(projectXML.createTextNode(projectName));
    e_projectInfo.appendChild(e_projectName);
    QDomElement e_projectAuthor = projectXML.createElement("Author");
    QString projectAuthor = ui->lineEdit_ProjectAuthor->text();
    e_projectAuthor.appendChild(projectXML.createTextNode(projectAuthor));
    e_projectInfo.appendChild(e_projectAuthor);
    QDomElement e_projectCompany = projectXML.createElement("Company");
    QString projectCompany = ui->lineEdit_ProjectCompany->text();
    e_projectCompany.appendChild(projectXML.createTextNode(projectCompany));
    e_projectInfo.appendChild(e_projectCompany);
    QDomElement e_projectNotes = projectXML.createElement("Notes");
    QString projectNotes = ui->plainTextEdit_ProjectNotes->document()->toPlainText();
    e_projectNotes.appendChild(projectXML.createTextNode(projectNotes));
    e_projectInfo.appendChild(e_projectNotes);
    e_Nano.appendChild(e_projectInfo);

    // System
    QDomElement e_ProjectCase = projectXML.createElement("Case");
    e_ProjectCase.setAttribute("Number", 1);
    QDomElement e_Feed = projectXML.createElement("Feed");
    e_Feed.setAttribute("Number", 1);
    e_Feed.setAttribute("Fraction", 1.0);
    QString pH = ui->lineEdit_pH->text();
    QDomElement e_pH = projectXML.createElement("pH");
    e_pH.appendChild(projectXML.createTextNode(pH));
    e_Feed.appendChild(e_pH);
    QString temperature = ui->lineEdit_Temperature->text();
    QDomElement e_Temperature = projectXML.createElement("Temperature");
    e_Temperature.appendChild(projectXML.createTextNode(temperature));
    e_Feed.appendChild(e_Temperature);
    QString waterType = ui->comboBox_WaterType->currentText();
    QDomElement e_WaterType = projectXML.createElement("WaterType");
    e_WaterType.setAttribute("Number", ui->comboBox_WaterType->currentIndex());
    e_WaterType.setAttribute("Name", waterType);
    e_Feed.appendChild(e_WaterType);
    QDomElement e_Ions = projectXML.createElement("Ions");
//    for (int ionIndex = 0; ionIndex < ionsList.size(); ++ionIndex) {
//        QDomElement e_Ion = projectXML.createElement("Ion");
//        e_Ion.setAttribute("Number", ionIndex);
//        //e_Ion.setAttribute("Name", ionsList.at(ionIndex).name);
//        double concentration = ionsModel->data(ionsModel->index(ionIndex, 0)).toDouble();
//        e_Ion.setAttribute("Concentration", concentration);
//        e_Ions.appendChild(e_Ion);
//    }
    e_Feed.appendChild(e_Ions);
    e_ProjectCase.appendChild(e_Feed);

    QDomElement e_Pass = projectXML.createElement("Pass");
    e_Pass.setAttribute("Number", 1);
//    QString permeateFlow = ui->lineEdit_PermeateFlow->text();
//    QDomElement e_PermeateFlow = projectXML.createElement("PermeateFlow");
//    e_PermeateFlow.appendChild(projectXML.createTextNode(permeateFlow));
//    e_Pass.appendChild(e_PermeateFlow);
//    QString recovery = ui->lineEdit_Recovery->text();
//    QDomElement e_Recovery = projectXML.createElement("Recovery");
//    e_Recovery.appendChild(projectXML.createTextNode(recovery));
//    e_Pass.appendChild(e_Recovery);
//    QString feedFlow = ui->lineEdit_FeedFlow->text();
//    QDomElement e_FeedFlow = projectXML.createElement("FeedFlow");
//    e_FeedFlow.appendChild(projectXML.createTextNode(feedFlow));
//    e_Pass.appendChild(e_FeedFlow);

    QDomElement e_Stage = projectXML.createElement("Stage");
    e_Stage.setAttribute("Number", 1);
    QDomElement e_Membrane = projectXML.createElement("Membrane");
    //e_Membrane.setAttribute("Number", chosenMembraneIndex);
    //e_Membrane.setAttribute("Name", membraneList.at(chosenMembraneIndex).name);
    e_Stage.appendChild(e_Membrane);
    e_Pass.appendChild(e_Membrane);

    e_ProjectCase.appendChild(e_Pass);
    e_projectInfo.appendChild(e_ProjectCase);
    e_Nano.appendChild(e_projectInfo);
    projectXML.appendChild(e_Nano);
    QTextStream out(xmlFile);
    projectXML.save(out, 1);
    //xmlFile->write(projectXML.toByteArray());
    return true;
}

//bool MainWindow::connectDB(const QString& dbname) {

//    QFile checkDB(dbname);
//    if (!checkDB.exists()) {
//        QMessageBox::critical(this, qApp->tr("Database error!"),
//                              qApp->tr("Database file does not exists.\n File path: %1\n")
//                              .arg(QFileInfo(checkDB).absoluteFilePath()), QMessageBox::Close);
//        return false;
//    }

//    db = QSqlDatabase::addDatabase("QSQLITE");
//    db.setDatabaseName(dbname);
//    if (!db.open()) {
//        QMessageBox::critical(this, qApp->tr("Database error!"),
//                              qApp->tr("Cannot open database.\n Database error: %1\n Driver error: %2\n")
//                              .arg(db.lastError().databaseText(),
//                                   db.lastError().driverText()), QMessageBox::Close);
//        return false;
//    }
//    return true;
//}


void MainWindow::changeFeedNumber(int feedFlowNumber) {
    if (_system.feedFlow(feedFlowNumber))
        dm->setSolutes(_system.feedFlow(feedFlowNumber)->solutes());
}

void MainWindow::addFeed() {
    _system.addFeedFlow();
    updateFeeds();
    ui->comboBox_CurrentFeedNumber->setCurrentIndex(_system.feedFlowsCount()-1);
}

void MainWindow::removeFeed() {
    int currentFeedIndex = ui->comboBox_CurrentFeedNumber->currentIndex();
    if (_system.removeFeedFlow(currentFeedIndex)) {
        updateFeeds();
        if (currentFeedIndex >= _system.feedFlowsCount()) --currentFeedIndex;
        ui->comboBox_CurrentFeedNumber->setCurrentIndex(currentFeedIndex);
    }

}

void MainWindow::updateFeeds() {
    ui->comboBox_CurrentFeedNumber->clear();
    for (int feedIndex = 0; feedIndex < _system.feedFlowsCount(); ++feedIndex)
        ui->comboBox_CurrentFeedNumber->addItem(QString::number(feedIndex+1));
}

void MainWindow::setTemperature(QString value) {
    _system.setTemperature(value.toDouble());
}

void MainWindow::setPH(QString value) {
    _system.setPH(value.toDouble());
}

//void MainWindow::reloadUi() {
//    QDeclarativeView *qmlView = new QDeclarativeView(this);
//    qmlView->rootContext()->setContextProperty("system", &_system);
//    qmlView->setSource(QUrl::fromLocalFile("TempResult.qml"));
//    qmlView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
//    QHBoxLayout* l = dynamic_cast<QHBoxLayout*>(ui->tab_Results->layout());
//    l->removeItem(l->takeAt(0));
//    l->addWidget(qmlView);

//    QDeclarativeView *sysConfView = new QDeclarativeView(this);
//    sysConfView->rootContext()->setContextProperty("system", &_system);
//    sysConfView->setSource(QUrl::fromLocalFile("ROConfSystem.qml"));
//    sysConfView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
//    l = dynamic_cast<QHBoxLayout*>(ui->tab_SysconfNew->layout());
//    l->removeItem(l->takeAt(0));
//    l->addWidget(sysConfView);

//    qmlSolutesModel* solmod = new qmlSolutesModel(_system.feedFlow(0)->solutes());
//    QDeclarativeView *feedWaterView = new QDeclarativeView(this);
//    feedWaterView->setAccessibleName("fwView");

//    feedWaterView->rootContext()->setContextProperty("system", &_system);
//    feedWaterView->rootContext()->setContextProperty("solutes", solmod);
////    QDeclarativeItem *item = feedWaterView->rootObject()->findChild<QDeclarativeItem*>("view");
////    if (item)
////        item->setProperty("model", reinterpret_cast<QVariant*>(&solmod));

//    feedWaterView->setSource(QUrl::fromLocalFile("FeedWaterPage.qml"));
//    feedWaterView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
//    QWidget* w = ui->feedWidget;
//    l = dynamic_cast<QHBoxLayout*>(w->layout());
//    if (l) {
//        while (l->itemAt(0)) {
//            delete l->itemAt(0);
//            l->removeItem(l->itemAt(0));
//        }
//    } else {
//        l = new QHBoxLayout(this);
//        w->setLayout(l);
//    }
//    l->addWidget(feedWaterView);

//    QDeclarativeView *newResView = new QDeclarativeView(this);
//    newResView->rootContext()->setContextProperty("system", &_system);
//    newResView->setSource(QUrl::fromLocalFile("ROResults.qml"));
//    newResView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
//    l = dynamic_cast<QHBoxLayout*>(ui->tab_NewResults->layout());
//    l->removeItem(l->takeAt(0));
//    l->addWidget(newResView);
//    //l->addWidget(feedWaterView);
//}
