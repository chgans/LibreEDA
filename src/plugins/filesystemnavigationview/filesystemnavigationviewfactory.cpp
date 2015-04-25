#include "filesystemnavigationviewfactory.h"
#include "filesystemnavigationwidget.h"

FileSystemNavigationViewFactory::FileSystemNavigationViewFactory(QObject *parent):
    INavigationViewFactory(parent)
{
    setDisplayName("File System");

}

FileSystemNavigationViewFactory::~FileSystemNavigationViewFactory()
{

}



NavigationView *FileSystemNavigationViewFactory::createView()
{
    NavigationView *view = new NavigationView();
    view->widget = new FileSystemNavigationWidget();
    return view;
}
