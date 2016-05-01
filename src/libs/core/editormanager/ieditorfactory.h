#ifndef IEDITORFACTORY_H
#define IEDITORFACTORY_H

#include "core/core_global.h"

#include <QObject>

class IEditor;

class CORE_EXPORT IEditorFactory : public QObject
{
    Q_OBJECT

public:
    explicit IEditorFactory(QObject *parent = nullptr);
    ~IEditorFactory();

    QString displayName() const;
    void setDisplayName(const QString &displayName);

    QString id() const;
    void setId(const QString &id);

    virtual IEditor *createEditor() = 0;

    QString fileExtension() const;
    void setFileExtension(const QString &fileExtension);
private:
    QString m_id;
    QString m_displayName;
    QString m_fileExtension;
};

#endif // IEDITORFACTORY_H
