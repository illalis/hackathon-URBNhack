#ifndef SERVICE_H
#define SERVICE_H

#include <QString>
#include <QFile>
#include <QTextStream>

class Service {
public:
    //! строки ошибок для выбрасвания exceptions
    static const QString    ErrCreateDir,
                            ErrCreateFile,
                            ErrOpenFile,
                            ErrRemoveFile,
                            ErrCopyFile,
                            ErrUnknownModel,
                            ErrUnknownAction,
                            ErrInvalidData,
                            ErrInvalidInput,
                            ErrInvalidCell;
    /*!
     * \brief Создает пустой файл с именем filePath
     */
    static void createEmptyFile(const QString filePath);
    /*!
     * \brief перезаписывает(если не существует, то создает) файл dest
     *        файлом source
     * \details если переданы одинаковые аргументы, то функция не делает ничего
     */
    static void overwriteFile(const QString source, const QString dest);
    /*!
     * \brief Парсит файл в QStingList построчно,
     *          удаляя ";" на конце (если она есть)
     */
    static QStringList csvToList (const QString filename);
    /*!
     * \brief Парсит файл в QList<QStringList> построчно, удаляя ";" на конце
     *          (если она есть), игнорирует пустые строки
     */
    static QList<QStringList> csvToListList(const QString filename, QByteArray codec /*= "UTF-8"*/);
    /*!
      \see csvToListList(filename)
     */
    static QList<QStringList> csvToListList(QTextStream &in);
    //! выполняет преобразование QList<int> в QStringList
    static QStringList listIntToStringList(const QList<int> list);
    /*!
     * \brief делает QList<int> из строки, в которой знечения разделены sep,
     *      для пустой строки возвращает пустой лист
     */
    static QList<int> strToListInt(const QString str, const QString sep);
};

#endif // SERVICE_H
