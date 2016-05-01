#ifndef FILESYSTEMNAVIGATIONVIEWFACTORY_H
#define FILESYSTEMNAVIGATIONVIEWFACTORY_H

#include "core/navigationview/inavigationviewfactory.h"

class FileSystemNavigationViewFactory: public INavigationViewFactory
{
    Q_OBJECT

public:
    explicit FileSystemNavigationViewFactory(QObject *parent = nullptr);
    ~FileSystemNavigationViewFactory();

    // INavigationViewFactory interface
public:
    virtual NavigationView *createView();
};

#endif // FILESYSTEMNAVIGATIONVIEWFACTORY_H
