#include <QtTest/QtTest>
#include "VarVal.h"
#include "VarStringVal.h"
#include "VarIntVal.h"
#include "VarDoubleVal.h"
#include "VarBoolVal.h"

using namespace VarTypes;

class TestVarVal : public QObject
{
    Q_OBJECT

private slots:
    void intValDefaultIsZero()
    {
        VarIntVal v;
        QCOMPARE(v.getInt(), 0);
    }

    void intValSetGet()
    {
        VarIntVal v;
        v.setInt(42);
        QCOMPARE(v.getInt(), 42);
    }

    void intValString()
    {
        VarIntVal v;
        v.setInt(123);
        QCOMPARE(v.getString(), std::string("123"));
    }

    void doubleValDefaultIsZero()
    {
        VarDoubleVal v;
        QCOMPARE(v.getDouble(), 0.0);
    }

    void doubleValSetGet()
    {
        VarDoubleVal v;
        v.setDouble(3.14);
        QCOMPARE(v.getDouble(), 3.14);
    }

    void boolValDefaultIsFalse()
    {
        VarBoolVal v;
        QCOMPARE(v.getBool(), false);
    }

    void boolValSetGet()
    {
        VarBoolVal v;
        v.setBool(true);
        QCOMPARE(v.getBool(), true);
    }

    void boolValString()
    {
        VarBoolVal v;
        v.setBool(true);
        QCOMPARE(v.getString(), std::string("true"));
        v.setBool(false);
        QCOMPARE(v.getString(), std::string("false"));
    }

    void stringValSetGet()
    {
        VarStringVal v;
        v.setString("hello");
        QCOMPARE(v.getString(), std::string("hello"));
    }

    void intValClone()
    {
        VarIntVal v;
        v.setInt(99);
        VarVal *c = v.clone();
        QCOMPARE(c->getString(), v.getString());
        delete c;
    }
};

QTEST_MAIN(TestVarVal)
#include "test_varval.moc"
