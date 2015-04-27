#ifndef DOCUMENTMANAGER_H
#define DOCUMENTMANAGER_H

#include <QObject>

class DocumentManager : public QObject
{
    Q_OBJECT
public:
    static DocumentManager *instance();


    static QStringList getOpenFileNames(const QString &filters,
                                        const QString &path = QString(),
                                        QString *selectedFilter = 0);

    static void addToRecentFiles(const QString &fileName/*, const QString &editorId = Id()*/);
    static void clearRecentFiles();
    static QList<QString> recentFiles();

    static QString fileDialogLastVisitedDirectory();
    static void setFileDialogLastVisitedDirectory(const QString &dir);

    static QString defaultLocationForNewFiles();
    static void setDefaultLocationForNewFiles(const QString &location);

    static void saveSettings();
    static void loadSettings();

signals:

public slots:

private:
    explicit DocumentManager(QObject *parent = 0);
    static DocumentManager *m_instance;
    static QList<QString> m_recentFiles;
    static QString m_fileDialogLastVisitedDirectory;
    static QString m_defaultLocationForNewFiles;
};

#endif // DOCUMENTMANAGER_H
