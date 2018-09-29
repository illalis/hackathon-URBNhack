#include "models/service.h"

#include <QTextCodec>

const QString Service::ErrCreateDir = QObject::tr("Couldn't create directory '%1'");
const QString Service::ErrCreateFile = QObject::tr("Couldn't create file '%1'");
const QString Service::ErrOpenFile = QObject::tr("Couldn't open file '%1'");
const QString Service::ErrRemoveFile = QObject::tr("Couldn't remove file '%1'");
const QString Service::ErrCopyFile = QObject::tr("Couldn't copy file '%1' to '%2'");

const QString Service::ErrUnknownModel = QObject::tr("Unknown model '%1'");
const QString Service::ErrUnknownAction = QObject::tr("Unknown action '%1'");
const QString Service::ErrInvalidData = QObject::tr("Invalid value in %1 model %2 row %3 column");
const QString Service::ErrInvalidInput = QObject::tr("Invalid input values %1");
const QString Service::ErrInvalidCell = QObject::tr("Invalid cell requested");

void Service::createEmptyFile(const QString filePath) {
    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly)){
        throw ErrCreateFile.arg(file.fileName());
    }
    file.close();
}

void Service::overwriteFile(const QString source, const QString dest) {
    if(source == dest) return;
    if(QFile::exists(dest) && !QFile::remove(dest)){
        throw ErrRemoveFile.arg(source);
    }
    if(!QFile::copy(source, dest)){
        throw ErrCopyFile.arg(source, dest);
    }
}

QStringList Service::csvToList(const QString filename) {
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        throw ErrOpenFile.arg(file.fileName());
    QTextStream in(&file);
    in.setCodec(QTextCodec::codecForName("UTF-8"));
    QStringList list;
    while(!in.atEnd()){
        QString line = in.readLine().trimmed();
        if(line.at(line.size() - 1) == ';') line.chop(1);
        list.append(line);
    }
    file.close();
    return list;
}

QList<QStringList> Service::csvToListList(const QString filename, QByteArray codec) {
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        throw ErrOpenFile.arg(file.fileName());
    QTextStream in(&file);
    in.setCodec(QTextCodec::codecForName(codec));
    QList<QStringList> listList = csvToListList(in);
    file.close();
    return listList;
}

QList<QStringList> Service::csvToListList(QTextStream &in) {
    QList<QStringList> listList;
    while(!in.atEnd()){
        QString line = in.readLine().trimmed();
        if(line.isEmpty()) continue;
        if(line.at(line.size() - 1) == ';') line.chop(1);
        listList.append(line.split(";"));
    }
    return listList;
}

QStringList Service::listIntToStringList(const QList<int> list) {
    QStringList strs;
    for(const auto& intVal : list) {
        strs.append(QString::number(intVal));
    }
    return strs;
}

QList<int> Service::strToListInt(const QString str, const QString sep) {
    QList<int> list;
    if(!str.isEmpty()) {
        for(auto num : str.split(sep)) {
            bool ok;
            list.append(num.toInt(&ok));
            if(!ok) throw QObject::tr("Wrong model file format");
        }
    }
    return list;
}
