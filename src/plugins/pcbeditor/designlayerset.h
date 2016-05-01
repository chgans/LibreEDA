#ifndef DESIGNLAYERSET_H
#define DESIGNLAYERSET_H

#include <QObject>
#include <QList>
#include <QString>

class QSettings;

class DesignLayerSet: public QObject
{
    Q_OBJECT

    Q_ENUMS(Type)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString customName READ customName WRITE setCustomName NOTIFY customNameChanged)
    Q_PROPERTY(QString effectiveName READ effectiveName NOTIFY effectiveNameChanged STORED false)
public:
    explicit DesignLayerSet(QObject *parent = nullptr);
    ~DesignLayerSet();

    QString name() const;
    void setName(const QString &name);
    QString customName() const;
    void setCustomName(const QString &name);
    QString effectiveName() const;
    bool isSystem() const;
    void setIsSystem(bool isSystem);
    QList<int> layers();

    void loadFromSettings(QSettings &settings);
    void saveToSettings(QSettings &settings) const;

    void add(int index);
    void add(const QList<int> &indexes);
    void remove(int index);
    void remove(const QList<int> &indexes);

signals:
    void layersChanged(const QList<int> &indexes);
    void nameChanged(const QString &name);
    void customNameChanged(const QString &name);
    void effectiveNameChanged(const QString &name);

private:
    QList<int> m_layers;
    bool m_isSystem;
    QString m_name;
    QString m_customName;
};

//Q_DECLARE_METATYPE(DesignLayerSet::Type)

#endif // DESIGNLAYERSET_H
