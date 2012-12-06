#include <QCoreApplication>
#include <QDebug>
#include <iostream>
#include <QFile>
#include <QStringList>
#include <QTextStream>

void gnuplot_header()
{
    std::cout << std::endl
           //<< "set terminal x11 size 120000,40000" << std::endl
              << "set terminal png" << std::endl
              << "set output 'image.png' " << std::endl
            << "set palette defined (0 \"white\", 1 \"black\")"<< std::endl
            << "plot '-' matrix with image"<< std::endl;
}

void gnuplot_footer()
{
    std::cout
            << "e\ne\n pause -1"<< std::endl;
}

struct Event
{
    long tick;
    QString name;
};

void write_line(const QString &active, QMap<QString,int>& names)
{
     for(QMap<QString,int>::const_iterator i = names.begin(); i!=names.end(); ++i)
    {
        std::cout << ((i.key() == active) ? ("1  ") : "0 ");
    }
    std::cout << std::endl;
}

void read_input()
{


    QFile file;
    file.open(stdin,QIODevice::ReadOnly);
    QMap<QString,int> thread_line;
    QVector<Event> events;


    int count = 0;
    while(!file.atEnd())
    {
        QString  line = file.readLine();
        QStringList tokens = line.split(" ");
        count ++;
        if(tokens.size()<2)
        {
            qDebug() << "invalid format at " << count;
            continue;
        }

        thread_line[tokens.at(1).simplified()]=0;
        Event e = { tokens.at(0).toLong(), tokens.at(1).simplified()};
        qDebug() << e.name;
        events.push_back(e);
    }
    qWarning() << "Found events: " << count << ", unique: " << thread_line.size();

    int time=0;
    Event current = events[0];

    for(int i=1; i<events.size();++i)
    {
        long time = current.tick;
        Event next = events[i];
        while(next.tick > time++)
            write_line(current.name,thread_line);
        current = next;
    }
}


int main(int c, char **v)
{
    QCoreApplication app(c,v);

    gnuplot_header();
    read_input();
    gnuplot_footer();

    return 0;//app.exec();
}

