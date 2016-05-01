#ifndef DOCUMENTMANAGER_H
#define DOCUMENTMANAGER_H

#include "core/core_global.h"

#include <QObject>

class IDocument;

// TODO:
//  - split between GUI and non-GUI
//  -
class CORE_EXPORT DocumentManager : public QObject
{
    Q_OBJECT
public:
    static DocumentManager *instance();


    void addDocuments(QList<IDocument *> documents);
    void addDocument(IDocument *document);
    void removeDocument(IDocument *document);
    QList<IDocument *> documents();
    QList<IDocument *> modifiedDocuments();

    static bool saveDocument(IDocument *document, const QString &fileName = QString());
    static bool closeDocument(IDocument *document);

    static void addToRecentFiles(const QString &fileName/*, const QString &editorId = Id()*/);
    static void clearRecentFiles();
    static QList<QString> recentFiles();

    static QString defaultLocationForNewFiles();
    static void setDefaultLocationForNewFiles(const QString &location);

    static void saveSettings();
    static void loadSettings();

    // GUI related stuff
    static QStringList getOpenFileNames(const QString &filters,
                                        const QString &path = QString(),
                                        QString *selectedFilter = nullptr);
    static QString getSaveFileName(const QString &title, const QString &pathIn,
                                   const QString &filter = QString(), QString *selectedFilter = nullptr);
    static QString getSaveFileNameWithExtension(const QString &title, const QString &pathIn,
                                                const QString &filter);
    static QString getSaveAsFileName(const IDocument *document, const QString &filter = QString(),
                                     QString *selectedFilter = nullptr);

    static bool saveAllModifiedDocumentsSilently(bool *canceled = nullptr,
                                                 QList<IDocument *> *failedToClose = nullptr);
    static bool saveModifiedDocumentsSilently(const QList<IDocument *> &documents, bool *canceled = nullptr,
                                              QList<IDocument *> *failedToClose = nullptr);
    static bool saveModifiedDocumentSilently(IDocument *document, bool *canceled = nullptr,
                                             QList<IDocument *> *failedToClose = nullptr);

    static bool saveAllModifiedDocuments(const QString &message = QString(), bool *canceled = nullptr,
                                         const QString &alwaysSaveMessage = QString(),
                                         bool *alwaysSave = nullptr,
                                         QList<IDocument *> *failedToClose = nullptr);
    static bool saveModifiedDocuments(const QList<IDocument *> &documents,
                                      const QString &message = QString(), bool *canceled = nullptr,
                                      const QString &alwaysSaveMessage = QString(),
                                      bool *alwaysSave = nullptr,
                                      QList<IDocument *> *failedToClose = nullptr);
    static bool saveModifiedDocument(IDocument *document,
                                     const QString &message = QString(), bool *canceled = nullptr,
                                     const QString &alwaysSaveMessage = QString(),
                                     bool *alwaysSave = nullptr,
                                     QList<IDocument *> *failedToClose = nullptr);

    static QString fileDialogLastVisitedDirectory();
    static void setFileDialogLastVisitedDirectory(const QString &dir);
    // End of GUI related stuff

signals:

public slots:

private:
    explicit DocumentManager(QObject *parent = nullptr);
    static DocumentManager *m_instance;
    static QList<IDocument *> m_documents;
    static QList<QString> m_recentFiles;
    static QString m_fileDialogLastVisitedDirectory;
    static QString m_defaultLocationForNewFiles;
};

#endif // DOCUMENTMANAGER_H
