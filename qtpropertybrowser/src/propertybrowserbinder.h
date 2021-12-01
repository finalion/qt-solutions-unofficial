#ifndef PROPERTYBROWSERBINDER_H
#define PROPERTYBROWSERBINDER_H

#include <QObject>

class QtAbstractPropertyBrowser;
class QtVariantPropertyManager;

class PropertyBrowserBinder : public QObject
{
    Q_OBJECT
public:
    explicit PropertyBrowserBinder(QtAbstractPropertyBrowser *browser, QObject *parent = nullptr);

    void fillBrowser();

    QtAbstractPropertyBrowser *propertyBrowser() const;
    void bindWidget(QWidget *widget);

signals:

private:
    QtVariantPropertyManager *m_mutabelPropertyManager;
    QtVariantPropertyManager *m_volatilePropertyManager;
    QtAbstractPropertyBrowser *m_propertyBrowser;
    QWidget *m_widget;
};

#endif // PROPERTYBROWSERBINDER_H
