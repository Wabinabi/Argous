#ifndef DATATRANSLATOR_H
#define DATATRANSLATOR_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDate>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QMultiMap>


class DataTranslator : public QObject
{
    Q_OBJECT
public:
    explicit DataTranslator(QObject *parent = nullptr);
    ~DataTranslator();

    void SetFilePath(QString dataLocation,QString dataDestination);
    bool GenerateFile();

private:
    QString _dataPath;
    QString _destPath;

    QVector<QString> _headers;
    QVector<QVector<QString>> _data;

    //InteralString, ExternalString, cloumn
    //InternalString shouldn't be changed
    QVector<QVector<QString>> _titles{
        {"DronePos_X"  , "DronePos_X"  , "-1"},
        {"DronePos_Y"  , "DronePos_Y"  , "-1"},
        {"DronePos_Z"  , "DronePos_Z"  , "-1"},
        {"DroneHeading", "DroneHeading", "-1"},
        {"US_Xp"       , "US_Xp"       , "-1"},
        {"US_Xn"       , "US_Xn"       , "-1"},
        {"US_Yp"       , "US_Yp"       , "-1"},
        {"US_Yn"       , "US_Yn"       , "-1"},
        {"US_Zp"       , "US_Up"       , "-1"},
        {"US_Zn"       , "US_Down"     , "-1"}
    };

    struct Point{
        QString sensor;
        double value;
        double x;
        double y;
        double z;
    };
    QVector<Point> _points;

    struct {
        double x;
        double y;
        double z;
        double angle;
    } Drone;

    bool ReadData();
    bool AssignColumn();
    bool Calculate();
    bool OutputToFile();
};

#endif // DATATRANSLATOR_H
