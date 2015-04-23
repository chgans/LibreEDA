#ifndef FILESYSTEMNAVIGATIONVIEWFACTORY_H
#define FILESYSTEMNAVIGATIONVIEWFACTORY_H

#include "core/navigationview/inavigationviewfactory.h"

class FileSystemNavigationViewFactory: public INavigationViewFactory
{
public:
    FileSystemNavigationViewFactory(QObject *parent = nullptr);
    ~FileSystemNavigationViewFactory();

    // INavigationViewFactory interface
public:
    virtual NavigationView *createView();
};

#endif // FILESYSTEMNAVIGATIONVIEWFACTORY_H
