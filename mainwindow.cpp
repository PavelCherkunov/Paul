#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QGridLayout *gridLayout = new QGridLayout(centralWidget);

    setWindowTitle("ЧАСЫ");
    QTimer *timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);



    // Создаем метку для отображения времени
    QLabel *Label = new QLabel("00:00:00");
    Label->setStyleSheet("font-size: 48pt;");
    gridLayout->addWidget(Label, 0, 0);

    // Создаем кнопки "Старт", "Стоп" и "Сброс"
    QPushButton *startButton = new QPushButton("Старт");
    startButton->setFixedSize(100, 50);
    QPushButton *stopButton = new QPushButton("Стоп");
    stopButton->setFixedSize(100, 50);
    QPushButton *resetButton = new QPushButton("Сброс");
    resetButton->setFixedSize(100, 50);

    // Создаем таймер
    QTimer *Timer1 = new QTimer;
    QTime time(0, 0, 0);

    // Подключаем слот для обновления времени
    QObject::connect(Timer1, &QTimer::timeout, [=, &time]()
     {
         time = time.addSecs(1);
         Label->setText(time.toString("hh:mm:ss"));
     });

    // Обработчик нажатия на кнопку "Старт"
    QObject::connect(startButton, &QPushButton::clicked, [=, &time]()
     {
        if (!Timer1->isActive())
         {
             Timer1->start(1000); // Запускаем таймер
         }
     });

    // Обработчик нажатия на кнопку "Стоп"
    QObject::connect(stopButton, &QPushButton::clicked, [=, &time]()
     {
         Timer1->stop(); // Остановка таймера
     });

    // Обработчик нажатия на кнопку "Сброс"
    QObject::connect(resetButton, &QPushButton::clicked, [=, &time]()
     {
         Timer1->stop();
         time.setHMS(0, 0, 0);
         Label->setText(time.toString("hh:mm:ss"));
     });

    // Размещаем виджеты на форме
    gridLayout->addWidget(Label);
    gridLayout->addWidget(startButton);
    gridLayout->addWidget(stopButton);
    gridLayout->addWidget(resetButton);

    // Создаем метку для отображения времени
    QLabel *label = new QLabel;
    label->setStyleSheet("font-size: 48pt;");
    gridLayout->addWidget(label, 0, 0);


    // Создаем таймер для обновления времени
    QTimer *Timer = new QTimer;
    Timer->start(1000); // Обновлять каждую секунду

    // Подключаем слот для обновления времени
    QObject::connect(Timer, &QTimer::timeout, [=]() {
        label->setText(QDateTime::currentDateTime().toString("hh:mm:ss"));
    });

    // Размещаем метку на форме
    QVBoxLayout *layout = new QVBoxLayout;
    gridLayout->addWidget(label);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QTime time = QTime::currentTime();
    QPainter painter;

    painter.begin(this);

    painter.setRenderHint(QPainter::Antialiasing);

    //смещаем нули в центр
    painter.translate(width()/1.5, height()/2.3);

    //положение циферблата
    int r = 0.5 * qMin(width()/2.7, height()/2.7);

    //циферблат
    for(int i = 0; i < 60; i++)
    {
        if(i % 5 == 0)
        {
            painter.drawLine(0, r, 0, r-10);
        }
        else
        {
            painter.drawLine(0, r, 0, r-4);
        }
        painter.rotate(6.0);
    }

    painter.setPen(Qt::NoPen);

    //часовая стрелка
    painter.save();
    QPolygon polygon1;
    polygon1 << QPoint(-0.05*r, 0) << QPoint(0.05*r, 0) << QPoint(0, -0.5*r);
    painter.setBrush(QColor(12, 34, 56));
    painter.rotate(30 * (time.hour() + time.minute()/60));
    painter.drawPolygon(polygon1);
    painter.restore();

    //минутная стрелка
    painter.save();
    QPolygon polygon2;
    polygon2 << QPoint(-0.05*r, 0) << QPoint(0.05*r, 0) << QPoint(0, -0.8*r);
    painter.setBrush(QColor(78, 90, 12, 191));
    painter.rotate(6.0 * (time.minute() + time.second()/60));
    painter.drawPolygon(polygon2);
    painter.restore();

    //секундная стрелка
    painter.save();
    QPolygon polygon3;
    polygon3 << QPoint(-0.02*r, 0) << QPoint(0.02*r, 0) << QPoint(0, -0.8*r);
    painter.setBrush(QColor(16, 03, 02, 220));
    painter.rotate(6.0 * time.second());
    painter.drawPolygon(polygon3);
    painter.restore();

    painter.end();
}


