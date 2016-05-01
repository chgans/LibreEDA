#ifndef CORE_H
#define CORE_H

class QSettings;

class Core
{

public:
    static Core *instance();
    static QSettings *settings();

private:
    Core();
    static Core *m_instance;
    static QSettings *m_settings;
};

#endif // CORE_H
