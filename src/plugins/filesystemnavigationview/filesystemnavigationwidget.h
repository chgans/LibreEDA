#ifndef FILESYSTEMNAVIGATIONWIDGET_H
#define FILESYSTEMNAVIGATIONWIDGET_H

#include <QWidget>

class QLabel;
class QListView;
class QFileSystemModel;

class FileSystemNavigationWidget: public QWidget
{
    Q_OBJECT

public:
    explicit FileSystemNavigationWidget(QWidget *parent = nullptr);
    ~FileSystemNavigationWidget();

    bool setCurrentDirectory(const QString &directory);
    QString currentDirectory() const;
    void setCurrentTitle(QString dirName, const QString &fullPath);

private slots:
    void openItem(const QModelIndex &index);

private:
    QLabel *m_title;
    QListView *m_dirView;
    QFileSystemModel *m_fileSystemModel;
};

#endif // FILESYSTEMNAVIGATIONWIDGET_H
