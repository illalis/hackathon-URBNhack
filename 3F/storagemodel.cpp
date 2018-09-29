#include "models/storagemodel.h"
#include "models/service.h"

const QByteArray StorageModel::defaultCodec = "UTF-8";

StorageModel::StorageModel(const QString modelFilename)
        : filePath (modelFilename) {
    if(filePath == "") return;
    QFile file(filePath);
    if(!file.exists()) {
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            throw Service::ErrOpenFile.arg(file.fileName());
        }
        return;
    }
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        throw Service::ErrOpenFile.arg(file.fileName());
    QTextStream in(&file);
    in.setCodec(QTextCodec::codecForName(defaultCodec));

    // reading metadata from the first line
    auto metadata = in.readLine().split(";");
    setObjectName(metadata[0].split(":")[1]);
    setTitle(metadata[1].split(":")[1]);
    setSearchColumns(Service::strToListInt(metadata[2].split(":")[1], ","));
    setUpdateCols(Service::strToListInt(metadata[3].split(":")[1], ","));
    setBoolCols(Service::strToListInt(metadata[4].split(":")[1], ","));

    // set headers and read data
    setHeaderData(in.readLine().split(";"));
    appendRows(in);
}

Qt::ItemFlags StorageModel::flags(const QModelIndex &index) const {
    auto flags = StringTableModel::flags(index);
    if(boolColumns.contains(index.column()))
        flags = (flags ^ Qt::ItemIsEditable) | Qt::ItemIsUserCheckable;
    return flags;
}

QVariant StorageModel::data(const QModelIndex& index, int role) const {
    if(!indexInBounds(index)) return QVariant();
    if(!boolColumns.contains(index.column()))
        return StringTableModel::data(index, role);
    if(role == Qt::EditRole) return StringTableModel::data(index, role);
    if(role != Qt::CheckStateRole) return QVariant();

    if(rows[index.row()][index.column()] == "0") {
        return Qt::Unchecked;
    } else if(rows[index.row()][index.column()] == "1") {
        return Qt::Checked;
    } else {
        throw Service::ErrInvalidData.arg(getTitle()).arg(index.row()).arg(index.column());
    }
}

bool StorageModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if(!indexInBounds(index)) return false;
    if(!boolColumns.contains(index.column()))
        return StringTableModel::setData(index, value, role);
    if(role != Qt::CheckStateRole) return false;

    if(value == Qt::Checked) {
        rows[index.row()][index.column()] = "1";
    } else if(value == Qt::Unchecked) {
        rows[index.row()][index.column()] = "0";
    } else {
        throw Service::ErrInvalidInput.arg(value.toString());
    }
    emit dataChanged(index, index);
    return true;
}

void StorageModel::appendRows(const QString fileName,
                              const QByteArray codec,
                              const QList<int> cols) {
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        throw Service::ErrOpenFile.arg(file.fileName());
    QTextStream in(&file);
    in.setCodec(QTextCodec::codecForName(codec));
    appendRows(in, cols);
}

void StorageModel::appendRows(QString data, const QList<int> cols) {
    QTextStream in(&data, QIODevice::ReadOnly);
    appendRows(in, cols);
}

void StorageModel::appendRows(QTextStream &in, const QList<int> cols) {
    auto _cols = checkColumnsList(cols);

    auto input = Service::csvToListList(in);

    beginInsertRows(QModelIndex(), rowCount(), rowCount() + qMax(input.size() - 1, 0));
    for(auto inputIt = input.constBegin(); inputIt != input.constEnd(); ++inputIt) {
        if((*inputIt).isEmpty()) continue;
        auto row = QVector<QString>(headers.size());
        for(int i = 0; i < _cols.size(); ++i) {
            if(_cols[i] != -1)
                row[_cols[i]] = (*inputIt).value(i, "");
        }
        rows.append(row);
    }
    endInsertRows();
}

void StorageModel::updateRows(const QString fileName,
                              const QByteArray codec,
                              const QList<int> cols) {
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        throw Service::ErrOpenFile.arg(file.fileName());
    QTextStream in(&file);
    in.setCodec(QTextCodec::codecForName(codec));
    updateRows(in, cols);
}

void StorageModel::updateRows(QTextStream &in, const QList<int> cols) {
    auto _cols = checkColumnsList(cols);

    // find zero index
    int zeroInd;
    for(zeroInd = 0; zeroInd < _cols.count(); ++zeroInd) {
        if(_cols[zeroInd] == 0) break;
    }
    if(zeroInd == _cols.count())
        throw tr("No zero index occured in update cols");

    // remember this to emit signal later
    // set minRow to max possible to decrese it and vice versa
    int minRow = rowCount(), maxRow = 0;

    while(!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if(line.isEmpty()) continue;
        QStringList updated = line.split(";");
        int row = rowOf(updated[zeroInd], 0);
        if(row == -1) {
            if(rowsPolicy == Abort) {
                throw Service::ErrInvalidInput.arg(line);
            } else if(rowsPolicy == Ignore) {
                continue;
            }
        }
        if(minRow > row) minRow = row;
        if(maxRow < row) maxRow = row;
        for(int fileColumn = 0; fileColumn < _cols.size(); ++fileColumn){
            if(_cols[fileColumn] != -1)
                rows[row][_cols[fileColumn]] = updated[fileColumn];
        }
    }

    emit dataChanged(index(minRow, 0), index(maxRow, columnCount() - 1));
}

void StorageModel::removeRows(const QString fileName,
                              const QByteArray codec,
                              const int column) {
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        throw Service::ErrOpenFile.arg(file.fileName());
    QTextStream in(&file);
    in.setCodec(QTextCodec::codecForName(codec));
    removeRows(in, column);
}

void StorageModel::removeRows(QString data, const int column) {
    QTextStream in(&data, QIODevice::ReadOnly);
    removeRows(in, column);
}

void StorageModel::removeRows(QTextStream &in, const int column) {
    while(!in.atEnd()) {
        QString value = in.readLine().trimmed();
        int row = rowOf(value, column);
        while (row != -1) {
            removeRow(row);
            row = rowOf(value, column);
        }
    }
}

void StorageModel::saveToDisk() const {
    QFile file(filePath);
    if(file.exists() && !file.remove())
        throw Service::ErrRemoveFile.arg(file.fileName());
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        throw Service::ErrCreateFile.arg(file.fileName());
    QTextStream out(&file);
    out.setCodec(QTextCodec::codecForName(defaultCodec));
    //сперва пишем метаинформацию
    QString metadata = "RastStorageModel:%1;title:%2;searchCols:%3;updateCols:%4;boolCols:%5\n";
    auto searchColsStrs = Service::listIntToStringList(getSearchColumns());
    auto updateColsStrs = Service::listIntToStringList(getUpdateCols());
    auto boolColsStrs = Service::listIntToStringList(getBoolCols());
    out << metadata.arg(objectName()).arg(getTitle())
                    .arg(searchColsStrs.join(",")).arg(updateColsStrs.join(","))
                    .arg(boolColsStrs.join(","));
    out << getHeaders().join(";") << '\n';
    //затем содержимое модели
    for(int i = 0; i < rowCount(); ++i) {
        QString line = "";
        for(int j = 0; j < columnCount(); ++j)
            line += rows[i][j] + ";";
        //убираем последнюю лишнюю точку с запятой
        line.chop(1);
        out << line << "\n";
    }
}

QStringList StorageModel::getSearchList() const {
    QStringList searchList;
    for(int i = 0; i < rowCount(); ++i) {
        QString line;
        for(int j = 0; j < searchColumns.size(); ++j) {
            line.append(data(i, searchColumns[j]) + " ");
        }
        searchList.append(line);
    }
    return searchList;
}

QList<int> StorageModel::checkColumnsList(const QList<int> cols) {
    QList<int> _cols;
    if(cols.isEmpty()) {
        int colCount = columnCount();
        for(int i = 0; i < colCount; ++i) {
            _cols.append(i);
        }
    } else {
        _cols = cols;
        for(auto it = _cols.constBegin(); it != _cols.constEnd(); ++it) {
            if(*it < -1 || *it >= headers.size())
                throw Service::ErrInvalidCell;
        }
    }
    return _cols;
}

QVector<QString> StorageModel::defauldRowValue() const {
    auto row = QVector<QString>(headers.size(), "");
    for(const auto& boolIndex : boolColumns) {
        row[boolIndex] = "0";
    }
    return row;
}
