#include "propertybrowserbinder.h"
#include <QMetaProperty>
#include "qtvariantproperty.h"
#include <QDebug>

PropertyBrowserBinder::PropertyBrowserBinder(QtAbstractPropertyBrowser *browser, QObject *parent)
    : QObject(parent), m_propertyBrowser(browser), m_widget(Q_NULLPTR)
{
    m_mutabelPropertyManager = new QtVariantPropertyManager(m_propertyBrowser);
    m_volatilePropertyManager = new QtVariantPropertyManager(m_propertyBrowser);
}

void PropertyBrowserBinder::fillBrowser()
{
    if(!m_widget)
        return;

    QtVariantEditorFactory *factory = new QtVariantEditorFactory(m_propertyBrowser);

    const QMetaObject *mo = m_widget->metaObject();
    int propCnt = mo->propertyCount();
    for(int i=0; i<propCnt; ++i)
    {
        QMetaProperty mp = mo->property(i);
        QtVariantProperty *property = m_volatilePropertyManager->addProperty(mp.type(), mp.name());
        // make sure the type is supported, property is not zero.
        if(property)
        {
            const QVariant value = m_widget->property(mp.name());
            property->setValue(value);
//            qWarning()<<mp.type()<<mp.name()<<widget->property(mp.name());
            m_propertyBrowser->addProperty(property);
        }
    }
    m_propertyBrowser->setFactoryForManager(m_volatilePropertyManager, factory);

    QObject::connect(m_volatilePropertyManager, &QtVariantPropertyManager::valueChanged, [=](QtProperty *property, const QVariant &v){
        QByteArray ba = property->propertyName().toLocal8Bit();
        const char *name = ba.constData();
//        qDebug()<<name<<v;
        m_widget->setProperty(name, v);
    });
}

QtAbstractPropertyBrowser *PropertyBrowserBinder::propertyBrowser() const
{
    return m_propertyBrowser;
}

void PropertyBrowserBinder::bindWidget(QWidget *widget)
{
    m_widget = widget;
}
