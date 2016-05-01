#ifndef IDOCUMENT_H
#define IDOCUMENT_H

#include "core/core_global.h"

#include <QObject>
class QPainter;

class CORE_EXPORT IDocument : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool modified READ isModified NOTIFY modifiedChanged)

public:
    explicit IDocument(QObject *parent = nullptr);
    ~IDocument();

    void setId(const QString &id);
    QString id() const;

    virtual bool save(QString *errorString, const QString &fileName = QString()) = 0;
    virtual void render(QPainter *painter) = 0;

    const QString filePath() const;
    virtual void setFilePath(const QString &filePath);
    QString displayName() const;
    void setDisplayName(const QString &name);
    bool isModified() const;

signals:
    void displayNameChanged(const QString &name);
    void filePathChanged(const QString &oldName, const QString &newName);
    void modifiedChanged(bool arg);

public slots:
    void setModified(bool modified);

private:
    QString m_id;
    QString m_filePath;
    QString m_displayName;
    bool m_modified;
};

#endif // IDOCUMENT_H
