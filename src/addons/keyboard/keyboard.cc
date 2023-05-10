#include "keyboard.h"

#include <xkbcommon/xkbcommon.h>

#include <QDir>
#include <QDomDocument>
#include <QList>

#include "config.h"

struct Variant {
    QString name;
    QString shortDescription;
    QString description;
    QList<QString> languageList;
};

struct Layout {
    QString name;
    QString shortDescription;
    QString description;
    QList<QString> languageList;
    QList<Variant> variantList;
};

Keyboard::Keyboard(Dim *dim)
    : InputMethodAddon(dim)
    , ctx_(xkb_context_new(XKB_CONTEXT_NO_FLAGS)) {
    if (!ctx_) {
        throw std::runtime_error("Failed to create xkb context");
    }

    QDir dir(QStringLiteral(XKEYBOARDCONFIG_XKBBASE) + QDir::separator() + "rules");
    QString rules = dir.absoluteFilePath(QString("%1.xml").arg(DEFAULT_XKB_RULES));
    QString extraRules = dir.absoluteFilePath(QString("%1.extras.xml").arg(DEFAULT_XKB_RULES));
}

Keyboard::~Keyboard() {
}

QList<InputMethodEntry> Keyboard::getInputMethods() {
    return {};
}

static QList<QString> parseLanguageList(const QDomElement &languageListEle) {
    QList<QString> languageList;
    for (auto language = languageListEle.firstChildElement("iso639Id"); !language.isNull();
         language = language.nextSiblingElement("iso639Id")) {
        auto languageName = language.text();
        languageList.append(languageName);
    }

    return languageList;
}

static QList<Variant> parseVariantList(const QDomElement &variantListEle) {
    QList<Variant> list;
    for (auto variantEle = variantListEle.firstChildElement("variant"); !variantEle.isNull();
         variantEle = variantEle.nextSiblingElement("variant")) {
        auto configItemEle = variantEle.firstChildElement("configItem");

        Variant variant;
        variant.name = configItemEle.firstChildElement("name").text();
        variant.shortDescription = configItemEle.firstChildElement("shortDescription").text();
        variant.description = configItemEle.firstChildElement("description").text();
        variant.languageList = parseLanguageList(configItemEle.firstChildElement("languageList"));
        list.append(variant);
    }

    return list;
}

static QList<Layout> parseLayoutList(const QDomElement &layoutListEle) {
    QList<Layout> list;
    for (auto layoutEle = layoutListEle.firstChildElement("layout"); !layoutEle.isNull();
         layoutEle = layoutEle.nextSiblingElement("layout")) {
        auto configItemEle = layoutEle.firstChildElement("configItem");

        Layout layout;
        layout.name = configItemEle.firstChildElement("name").text();
        layout.shortDescription = configItemEle.firstChildElement("shortDescription").text();
        layout.description = configItemEle.firstChildElement("description").text();
        layout.languageList = parseLanguageList(configItemEle.firstChildElement("languageList"));
        layout.variantList = parseVariantList(layoutEle.firstChildElement("variantList"));

        list.append(layout);
    }

    return list;
}

void Keyboard::parseRule(const QString &file) {
    QFile xmlFile(file);
    if (!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QDomDocument xmlReader;
    xmlReader.setContent(&xmlFile);
    auto layoutListEle = xmlReader.documentElement().firstChildElement("layoutList");

    QList<Layout> layouts = parseLayoutList(layoutListEle);
}
