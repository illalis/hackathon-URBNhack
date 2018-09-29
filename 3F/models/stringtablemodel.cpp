#include "models/stringtablemodel.h"
#include "models/service.h"

StringTableModel::StringTableModel() { }

StringTableModel::StringTableModel(const QStringList& labels)
    : headers(labels) { }

StringTableModel::StringTableModel(const QVector< QVector< QString > >& data)
    : rows(data) { }

StringTableModel::StringTableModel(const QStringList& labels,
                                   const QVector< QVector< QString > >& data)
    : headers(labels),
      rows(data) { }

int StringTableModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return rows.size();
}

int StringTableModel::columnCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return headers.size();
}

QVariant StringTableModel::data(const QModelIndex& index, int role) const {
    if(!indexInBounds(index)) return QVariant();
    if(!(role == Qt::DisplayRole || role == Qt::EditRole || role == Qt::ToolTipRole))
        return QVariant();

    return rows[index.row()][index.column()];
}

QVariant StringTableModel::headerData(int section,
                                      Qt::Orientation orientation,
                                      int role) const {
    if(role != Qt::DisplayRole) return QVariant();

    if(orientation == Qt::Vertical) {
        return section + 1;
    } else {
        if(section < 0 || section >= headers.size()) return QVariant();

        return headers[section];
    }
}

Qt::ItemFlags StringTableModel::flags(const QModelIndex &index) const {
    if(!indexInBounds(index)) return Qt::NoItemFlags;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool StringTableModel:: setData(const QModelIndex &index,
                               const QVariant &value,
                               int role) {
    if(role != Qt::EditRole) return false;
    if(!indexInBounds(index)) return false;

    rows[index.row()][index.column()] = value.toString();
    emit dataChanged(index, index);
    return true;
}

bool StringTableModel::setHeaderData(int section,
                                     Qt::Orientation orientation,
                                     const QVariant &value,
                                     int role) {
    Q_UNUSED(role);
    if(orientation == Qt::Vertical)
        throw QString(tr("Can't set vertical headers"));
    auto initSize = headers.size();
    for(int i = initSize; i <= section; ++i) {
        headers.append("");
    }
    headers[section] = value.toString();
    // calculate borders for signal emiting
    int first, last;
    if(initSize == headers.size()) {
        first = section;
        last = section;
    } else {
        first = initSize;
        last = headers.size() - 1;
    }
    emit headerDataChanged(orientation, first, last);
    return true;
}

bool StringTableModel::insertRows(int position, int rowsCount, const QModelIndex &index) {
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position + rowsCount - 1);

    for(int i = 0; i < rowsCount; ++i)
        rows.insert(position, defauldRowValue());

    endInsertRows();
    return true;
}

bool StringTableModel::removeRows(int position, int rowsCount, const QModelIndex &index) {
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position + rowsCount - 1);

    for (int i = 0; i < rowsCount; ++i)
        rows.removeAt(position);

    endRemoveRows();
    return true;
}

void StringTableModel::sort(int column, Qt::SortOrder order) {
    layoutAboutToBeChanged(QList<QPersistentModelIndex>(), QAbstractItemModel::VerticalSortHint);
    std::sort(rows.begin(), rows.end(),
              [column, order](const QVector<QString>& a, const QVector<QString>& b) {
                    return order == Qt::AscendingOrder ?
                                a[column] < b[column]:
                                a[column] > b[column];
                });
    layoutChanged(QList<QPersistentModelIndex>(), QAbstractItemModel::VerticalSortHint);
}

QString StringTableModel::data(const int row, const int col) const {
    return data(index(row, col)).toString();
}

QString StringTableModel::data(const int row, const QString colName) const {
    return data(row, columnOf(colName));
}

bool StringTableModel::setData(const int row, const int col, const QString value) {
    if(row < 0 || row >= rows.size()) throw Service::ErrInvalidCell;
    if(col < 0 || col >= headers.size()) throw Service::ErrInvalidCell;

    rows[row][col] = value;
    auto ind = index(row, col);
    emit dataChanged(ind, ind);
    return true;
}

bool StringTableModel::setData(int row, const QString colName, const QString value) {
    return setData(row, columnOf(colName), value);
}

bool StringTableModel::setHeaderData(const QStringList newHeaders) {
    auto initSize = headers.size();
    headers = newHeaders;
    emit headerDataChanged(Qt::Horizontal, 0, qMax(initSize, headers.size()));
    return true;
}

bool StringTableModel::insertRows(int position, QList<QStringList> data) {
    beginInsertRows(QModelIndex(), position, position + data.size() - 1);
    rows.insert(position, data.size(), QVector<QString>(columnCount(), ""));
    auto rowIt = rows.begin() + position;
    for(const auto& dataRow : data) {
        auto dataCell = dataRow.begin();
        for(auto&& rowsCell : *rowIt) {
            rowsCell = *dataCell;
            ++dataCell;
            if(dataCell == dataRow.end()) break;
        }
        ++rowIt;
    }
    endInsertRows();
    return true;
}

void StringTableModel::fillIndexes(const QModelIndexList indexes,
                                   const QString value) {
    int minRow = rowCount(), maxRow = 0;
    int minCol = columnCount(), maxCol = 0;
    for(const auto& index : indexes) {
        minRow = qMin(minRow, index.row());
        maxRow = qMax(maxRow, index.row());
        minCol = qMin(minCol, index.column());
        maxCol = qMax(maxCol, index.column());
        rows[index.row()][index.column()] = value;
    }
    emit dataChanged(index(minRow, minCol), index(maxRow, maxCol));
}

void StringTableModel::fillColumn(const int col, const QString value) {
    for(auto&& row : rows)
        row[col] = value;

    emit dataChanged(index(0, col), index(rowCount(), col));
}

int StringTableModel::columnOf(const QString label) const {
    auto ind = headers.indexOf(label);
    if(ind == -1) throw Service::ErrInvalidInput.arg(label);
    return ind;
}

int StringTableModel::rowOf(const QString value, const int column) const {
    if(column < 0 || column >= headers.size()) throw Service::ErrInvalidCell;

    for(int i = 0; i < rows.size(); ++i)
        if(rows[i][column] == value)
            return i;
    return -1;
}

QStringList StringTableModel::rowToList(const int row) const {
    return rows[row].toList();
}

QStringList StringTableModel::colToList(const int col) const {
    QStringList res;
    for(const auto& row : rows) res << row[col];
    return res;
}

bool StringTableModel::indexInBounds(const QModelIndex& index) const {
    if(!index.isValid()) return false;
    if(index.row() < 0 || index.row() >= rows.size()) return false;
    if(index.column() < 0 || index.column() >= headers.size()) return false;
    return true;
}

QVector<QString> StringTableModel::defauldRowValue() const {
    return QVector<QString>(headers.size(), "");
}
