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


    QTimer *clock_timer = new QTimer;
    //connect(clock_timer, &QTimer::timeout, time, &QWidget::update);
    connect(clock_timer, SIGNAL(timeout()), this, SLOT(update()));
    clock_timer->start(1000);// К часам

    QTime time(0, 0, 0);

    QTimer *stopwatch_timer = new QTimer;
    connect(stopwatch_timer, &QTimer::timeout, this, &QTime::currentTime);// К секундомеру

    // Создаем метку для отображения времени
    QLabel *stopwatch_label = new QLabel("00:00:00");
    stopwatch_label->setStyleSheet("font-size: 48pt;");
    gridLayout->addWidget(stopwatch_label, 0, 0);

    // Создаем кнопки "Старт", "Стоп" и "Сброс"
    QPushButton *startButton = new QPushButton("Старт");
    startButton->setFixedSize(100, 50);
    QPushButton *stopButton = new QPushButton("Стоп");
    stopButton->setFixedSize(100, 50);
    QPushButton *resetButton = new QPushButton("Сброс");
    resetButton->setFixedSize(100, 50);

    // Подключаем слот для обновления времени
    QObject::connect(stopwatch_timer, &QTimer::timeout, [=, &time]()
     {
         time = time.addSecs(1);
         stopwatch_label->setText(time.toString("hh:mm:ss"));
     });

    // Обработчик нажатия на кнопку "Старт"
    QObject::connect(startButton, &QPushButton::clicked, [=, &time]()
     {
        if (!stopwatch_timer->isActive())
         {
             stopwatch_timer->start(1000); // Запускаем таймер
         }
     });

    // Обработчик нажатия на кнопку "Стоп"
    QObject::connect(stopButton, &QPushButton::clicked, [=, &time]()
     {
         stopwatch_timer->stop(); // Остановка таймера
     });

    // Обработчик нажатия на кнопку "Сброс"
    QObject::connect(resetButton, &QPushButton::clicked, [=, &time]()
     {
         stopwatch_timer->stop();
         time.setHMS(0, 0, 0);
         stopwatch_label->setText(time.toString("hh:mm:ss"));
     });

    // Размещаем виджеты на форме
    gridLayout->addWidget(stopwatch_label);
    gridLayout->addWidget(startButton);
    gridLayout->addWidget(stopButton);
    gridLayout->addWidget(resetButton);

    // Создаем метку для отображения времени
    QLabel *clock_label = new QLabel;
    clock_label->setStyleSheet("font-size: 48pt;");
    gridLayout->addWidget(clock_label, 0, 0);

    // Подключаем слот для обновления времени
    QObject::connect(clock_timer, &QTimer::timeout, [=]() {
        clock_label->setText(QDateTime::currentDateTime().toString("hh:mm:ss"));
    });

    // Размещаем метку на форме
    gridLayout->addWidget(clock_label);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QTime time = QTime::currentTime();
    QPainter painter;
    QColor red(255, 0, 0);
    QColor green(0, 255, 0, 191);
    QColor blue(0, 0, 255, 220);

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
    QPolygon hour_polygon;
    hour_polygon << QPoint(-0.05*r, 0) << QPoint(0.05*r, 0) << QPoint(0, -0.5*r);
    painter.setBrush(red);
    painter.rotate(30 * (time.hour() + time.minute()/60));
    painter.drawPolygon(hour_polygon);
    painter.restore();

    //минутная стрелка
    painter.save();
    QPolygon minute_polygon;
    minute_polygon << QPoint(-0.05*r, 0) << QPoint(0.05*r, 0) << QPoint(0, -0.8*r);
    painter.setBrush(green);
    painter.rotate(6.0 * (time.minute() + time.second()/60));
    painter.drawPolygon(minute_polygon);
    painter.restore();

    //секундная стрелка
    painter.save();
    QPolygon second_polygon;
    second_polygon << QPoint(-0.02*r, 0) << QPoint(0.02*r, 0) << QPoint(0, -0.8*r);
    painter.setBrush(blue);
    painter.rotate(6.0 * time.second());
    painter.drawPolygon(second_polygon);
    painter.restore();

    painter.end();
}



