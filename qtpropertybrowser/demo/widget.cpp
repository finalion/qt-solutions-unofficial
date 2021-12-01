#include "widget.h"
#include "ui_widget.h"
#include <QMetaProperty>
#include <QDebug>
#include "propertybrowserbinder.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    PropertyBrowserBinder *binder = new PropertyBrowserBinder(ui->treePropBrowser);
    binder->bindWidget(ui->pushButton);
    binder->fillBrowser();

//    QtVariantPropertyManager *manager2 = new QtVariantPropertyManager(ui->treePropBrowser);
//    QtVariantEditorFactory *factory = new QtVariantEditorFactory(ui->treePropBrowser);

//    QtProperty *groupProp = manager2->addProperty(QtVariantPropertyManager::groupTypeId(), "group1");

//    QtVariantProperty *property = manager2->addProperty(QVariant::String, "test3");
//    property->setValue("hellovr");
//    groupProp->addSubProperty(property);

//    ui->treePropBrowser->addProperty(groupProp);
//    ui->treePropBrowser->setFactoryForManager(manager2, factory);
//    ui->treePropBrowser->unsetFactoryForManager(manager2);
}

Widget::~Widget()
{
    delete ui;
}

