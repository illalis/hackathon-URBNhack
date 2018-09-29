#ifndef STRINGTABLEMODEL_H
#define STRINGTABLEMODEL_H

#include <QAbstractTableModel>

/*!
 * \brief Класс реализует интерфейс QAbstractTableModel в виде простого списка
 *      строк с некоторыми удобными функциями
 * \details Класс отвечает только за реализацию простейшего хранилища без
 *      каких-либо проверок или специальных методов, чтобы миеть возможность
 *      расширять его по-разному (для этого структуры для хранения данных
 *      сделаны protected, а не private.
 *      Реализована функция def
 */
class StringTableModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit StringTableModel();
    explicit StringTableModel(const QStringList& labels);
    explicit StringTableModel(const QVector< QVector< QString > >& data);
    /*!
     * \brief Создает модель с загловками labels и данными data
     * \details размер каждого внутреннего QVector должен быть равен размеру labels
     */
    explicit StringTableModel(const QStringList& labels,
                              const QVector< QVector< QString > >& data);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    /*!
     * \note позволяет только менять существующее поле в модели (только внутри текущего размера)
     *      (добавить новые не получится, пользоваться insertRows для этого)
     */
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool setHeaderData(int section, Qt::Orientation orientation = Qt::Horizontal, const QVariant &value = QVariant(), int role = Qt::EditRole) override;
    //! Использует ::defautRowValue для новых строк
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

    // custom features
    /*!
     * \brief возвращает элемент в ячейке row, col
     * \details перегруженная функция для удобства пользования
     */
    QString data(const int row, const int col) const;
    //! Перегруженная функиця, позволяет получать данные по названию толбца
    QString data(const int row, const QString colName) const;
    /*!
     * \brief устанавливает элемент в ячейке row, col
     * \details перегруженная функция для удобства пользования,
     *      также не позволяет добавить новые ячейки
     */
    bool setData(const int row, const int col, const QString value);
    //! Перегруженная функиця, позволяет писать данные по названию столбца
    bool setData(int row, const QString colName, const QString value);
    /*!
     * \brief возвращает список горизонтальных заголовков
     */
    inline const QStringList getHeaders() const { return headers; }
    /*!
     * \brief устанавливает горизонтальные заголовки модели
     */
    bool setHeaderData(const QStringList newHeaders);
    /*!
     * \brief вставляет в модель данные из data с позиции position
     * \details вставляет ровно columnCount() столбцов из data (если в data
     *      больше столбцов, то обрезает, если меньше, то дополняет пустыми),
     *      возвращает true, если вставка успешна
     */
    bool insertRows(int position, QList<QStringList> data);
    /*!
     * \brief заполняет поля с указанными индексами значением value
     * \details не проверяет валидность индексов, отправляет только один сигнал
     */
    void fillIndexes(const QModelIndexList indexes, const QString value);
    /*!
     * \brief заполняет весь столбец значением value
     * \details не проверяет валидность индексов, отправляет только один сигнал
     */
    void fillColumn(const int col, const QString value);
    /*!
     * \brief возвращает номер колонки с именем label
     * \details при отутствии такого названия кидается exception
     */
    int columnOf(const QString label) const;
    /*!
     * \brief выдает номер строки в которой в столбце column впервые встретилось
     *          значение value, при отсутствии возвращает -1
     */
    int rowOf(const QString value, const int column = 0) const;
    /*!
     * \brief Парсит строку модели в QStringList
     * \param row номер строки в модели
     */
    QStringList rowToList(const int row) const;
    /*!
     * \see rowToList(const int row)
     */
    QStringList colToList(const int col) const;
protected:
    /*!
     * \brief проверяет валидность индекса и что он лежит в текущих границах
     */
    bool indexInBounds(const QModelIndex& index) const;
protected: // TODO совсем не хорошо делать их не private. надо бы написать тут
           // функцию для массового редактирования, а потом уже ей пользоваться
    //! горизонтальные заголовки модели
    QStringList headers;
    //! структура для хранения данных
    QVector< QVector<QString> > rows;
private:
    //! функция сделана для метода ::insertRows(), который добавляет новые
    //! строки в модель, чтобы эти строки могли быть кастомизованы
    virtual QVector<QString> defauldRowValue() const;
};

#endif // STRINGTABLEMODEL_H
