#include <QtTest/QtTest>
#include <QApplication>
#include "VarTypes.h"
#include "VarTypesInstance.h"
#include "VarInt.h"
#include "VarDouble.h"
#include "VarBool.h"
#include "VarString.h"
#include "VarList.h"
#include "VarStringEnum.h"
#include "VarSelection.h"

using namespace VarTypes;

class TestVarTypes : public QObject
{
    Q_OBJECT

private slots:
    void varIntCreation()
    {
        VarIntPtr vi(new VarInt("test_int", 10, 0, 100));
        QCOMPARE(vi->getInt(), 10);
        QCOMPARE(vi->getName(), std::string("test_int"));
    }

    void varIntMinMax()
    {
        VarIntPtr vi(new VarInt("clamped", 50, 0, 100));
        vi->setInt(200);
        QCOMPARE(vi->getInt(), 100);
        vi->setInt(-10);
        QCOMPARE(vi->getInt(), 0);
    }

    void varDoubleCreation()
    {
        VarDoublePtr vd(new VarDouble("test_double", 1.5));
        QVERIFY(qFuzzyCompare(vd->getDouble(), 1.5));
    }

    void varBoolCreation()
    {
        VarBoolPtr vb(new VarBool("test_bool", true));
        QCOMPARE(vb->getBool(), true);
    }

    void varStringCreation()
    {
        VarStringPtr vs(new VarString("test_string", "hello"));
        QCOMPARE(vs->getString(), std::string("hello"));
    }

    void varListAddChildren()
    {
        VarListPtr vl(new VarList("root"));
        VarIntPtr vi(new VarInt("child_int"));
        vl->addChild(vi);
        auto children = vl->getChildren();
        QCOMPARE((int)children.size(), 1);
        QCOMPARE(children[0]->getName(), std::string("child_int"));
    }

    void varStringEnumSelection()
    {
        VarStringEnumPtr vse(new VarStringEnum("enum", "b"));
        vse->addItem("a");
        vse->addItem("b");
        vse->addItem("c");
        QCOMPARE(vse->getString(), std::string("b"));
    }

    void varResetToDefault()
    {
        VarIntPtr vi(new VarInt("def_test", 42));
        vi->setInt(99);
        QCOMPARE(vi->getInt(), 99);
        vi->resetToDefault();
        QCOMPARE(vi->getInt(), 42);
    }

    void varTypesFactory()
    {
        VarTypesFactoryPtr factory = VarTypesInstance::getFactory();
        QVERIFY(factory != nullptr);
        QVERIFY(factory->isRegisteredType("VarInt"));
        QVERIFY(factory->isRegisteredType("VarDouble"));
        QVERIFY(factory->isRegisteredType("VarBool"));
        QVERIFY(factory->isRegisteredType("VarString"));
        QVERIFY(factory->isRegisteredType("VarList"));
    }
};

QTEST_MAIN(TestVarTypes)
#include "test_vartypes.moc"
