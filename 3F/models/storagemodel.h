#ifndef CURRENTSTATEMODEL_H
#define CURRENTSTATEMODEL_H

#include <QFile>
#include <QTextStream>
#include <QTextCodec>

#include "models/stringtablemodel.h"

/*!
 * \brief класс расширяет StringTableModel для работы с файлами .csv в формате
 *      RastStorage
 * \details позволяет загружать модель из файла, добавлять и удалять
 *          строки из файлов, записывать модель в файл.
 *          Реализует удобный интерфейс для массового CRUD.
*/
class StorageModel : public StringTableModel {
    Q_OBJECT
public:
    //! политика обработки неизвестных строк во входных файлах
    enum UnknownRowsPolicy : int { Ignore = 0, Abort = 1 };
    /*!
     * \brief создает и заполняет модель из указаного файла модели
     * \details Файл должен быть в формате, описаном в StorageDataFormats.txt,
     *      Если файл не указан, то создается пустая модель
     * \param modelFilename полный адрес файла
     */
    explicit StorageModel(const QString modelFilename = "");

    // QAbstractItemModel interface
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    /*!
     * \brief реализует интерфейс QAbstractItemModel, добавляя возможность
     *      отображать checkbox-ы для некоторых столбцов
     * \details для столбцов из boolCols будут отображаться только checkbox-ы
     *      без подписи
     */
    using StringTableModel::data;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    using StringTableModel::setData;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    /*!
     * \brief добавляет в модель строки из файла
     * \details пустые строки в файле игнорируются
     * \param fileName полное имя файла .csv
     * \param имя кодека файла fileName
     * \param cols числа по количеству столбцов во входном файле,
     *      i-ое число в cols - номер столбца модели, в который нужно записать
     *      содержимое i-ого столбца в файле, либо -1, если столбец не унужно использовать.
     *      -1 <= cols[i] < model.columnCount(). Значения в cols должны быть уникальными,
     *      исключая -1. Если лист пуст, то предполагается записывать первые
     *      model.columnCount() столбцов файла по порядку в модель, в этом случае
     *      лишние столбцы игнорируются, недостающие заполняются пустыми занчениями
     */
    void appendRows(const QString fileName,
                    const QByteArray codec,
                    const QList<int> cols = QList<int>());
    //! перегруженная функция, где data - строковое представление данных
    void appendRows(QString data, const QList<int> cols = QList<int>());
    /*!
     * \brief перегруженная функция
     * \param in поток, открытый для чтения
     * \param cols см. addItems(const QString fileName, QList<int> cols)
     * \see addItems(const QString fileName, QList<int> cols)
     */
    void appendRows(QTextStream &in, const QList<int> cols = QList<int>());
    /*!
     * \brief обновляет значения модели из файла
     * \details для каждой входной строчки находит строчку с таким же
     *          значением в 0 столбце и перезаписывает остальные значения на входные.
     *          Если не находит значения, то игнорирует его
     * \param fileName имя файла .csv
     * \param cols значение аналогично значению в ф-и addItems, но здесь в cols
     *          обязательно должно присутствовать значение 0 (если лист не пуст),
     *          для дефолтных колонок есть перегруженная функция
     */
    void updateRows(const QString fileName,
                    const QByteArray codec,
                    const QList<int> cols);
    /*!
     * \brief перегруженная функция
     * \details если подать пустой лист cols, то предполагается наличие всех
     *      колонок в прямом порядке, а не дефолтные (для них есть специальная
     *      функция)
     * \see updateItems(const QString, const QList<int>, const QList<int>)
     */
    void updateRows(QTextStream &in, const QList<int> cols);
    /*!
     * \brief перегруженная функция, использует дефолтные столбцы (прописанные
     *      в поле updateColumns.
     * \see updateItems(const QString fileName, const QByteArray codec, )
     */
    inline void updateRows(const QString fileName, const QByteArray codec)
                                { updateRows(fileName, codec, updateColumns); }
    /*!
     * \brief перегруженная функция, использует дефолтные столбцы (прописанные
     *      в поле updateColumns.
     * \see updateItems(QTextStream &in, const QList<int> cols)
     */
    inline void updateRows(QTextStream &in) { updateRows(in, updateColumns); }
    using StringTableModel::removeRows; // to prevent name hiding
    /*!
     * \brief удаляет из модели все строки у которых в столбце column содрежатся
     *          значения из файла
     * \details пустые строки в файле игнорируются
     * \param fileName полное имя файла, в котором в каждой строке записано по одному
     *          значению, которое нужно удалить
     * \param column столбец, с которым сравниваются значения из file
     */
    void removeRows(const QString fileName,
                    const QByteArray codec,
                    const int column = 0);
    /*!
     * \brief перегруженная функция
     * \param data имена для удаления, разделённые переводом строки
     */
    void removeRows(QString data, const int column = 0);
    /*!
     * \brief перегруженная функция
     * \param in поток, открытый для чтения
     * \see removeItems(const QString fileName, const int column);
     */
    void removeRows(QTextStream &in, const int column = 0);
    /*!
     * \brief пишет модель в файл
     * \details если файл существует, он будет перезаписан,
     *      если файл пуст, выбросит exception
     */
    void saveToDisk() const;
    // геттеры-сеттеры
    inline QString getFileName() const { return filePath; }
    inline void setFileName(const QString fileName) { filePath = fileName; }
    inline UnknownRowsPolicy getRowPolicy() const { return rowsPolicy; }
    inline void setRowPolicy(UnknownRowsPolicy policy) { rowsPolicy = policy; }
    inline QString getTitle() const { return title; }
    inline void setTitle(const QString newTitle) { title = newTitle; }
    inline QList<int> getUpdateCols() const { return updateColumns; }
    inline void setUpdateCols(const QList<int> newColumns) { updateColumns = newColumns; }
    inline QList<int> getBoolCols() const { return boolColumns; }
    inline void setBoolCols(QList<int> newBoolCols) { boolColumns = newBoolCols; }
    inline QList<int> getSearchColumns() const { return searchColumns; }
    inline void setSearchColumns(const QList<int> searchColumnsList)
                                { searchColumns = searchColumnsList; }
    /*!
     * \brief Возвращает строки для поиска
     * \details для каждой строки выдает разделенные пробелом значения колонок
     *          из searchColumns
     */
    QStringList getSearchList() const;
private: // members
    static const QByteArray defaultCodec;
    //! название для отображения пользователю
    QString title;
    //! адрес файла, в котором хранятся данные модели
    QString filePath;
    //! список номеров колонок, которые входят в выдачу getSearchList
    QList<int> searchColumns;
    //! список номеров колонок для дефолтного обновления из файла
    QList<int> updateColumns;
    //! список номеров колонок, для которых будет отображаться checkbox-ы.
    //! Значения в этих столбцах есть '1' и '0'
    QList<int> boolColumns;
    //! значение политики относительно неизвестных столбцов
    UnknownRowsPolicy rowsPolicy = UnknownRowsPolicy::Ignore;
private: // functions
    /*!
     * \brief проверяет валидность списка колонок, если cols.isEmpty, то заполняет
     *          от 0 до columnCount
     */
    QList<int> checkColumnsList(const QList<int> cols);
    //! Возвращает в качестве дефолтных значений для #boolColumns "0", а для
    //! остальных - пустую строку
    virtual QVector<QString> defauldRowValue() const override;
};

#endif // CURRENTSTATEMODEL_H
