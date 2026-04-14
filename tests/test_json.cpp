#include <QtTest/QtTest>
#include <QApplication>
#include <QTemporaryDir>
#include "VarTypes.h"
#include "VarInt.h"
#include "VarDouble.h"
#include "VarBool.h"
#include "VarString.h"
#include "VarList.h"
#include "VarJSON.h"

using namespace VarTypes;

class TestJson : public QObject
{
    Q_OBJECT

private slots:
    void writeAndReadRoundTrip()
    {
        QTemporaryDir tmpDir;
        QVERIFY(tmpDir.isValid());
        QString jsonPath = tmpDir.path() + "/test_settings.json";

        // Build a tree
        VarListPtr root(new VarList("root"));
        VarIntPtr vi(new VarInt("my_int", 42));
        VarDoublePtr vd(new VarDouble("my_double", 3.14));
        VarBoolPtr vb(new VarBool("my_bool", true));
        VarStringPtr vs(new VarString("my_string", "hello"));
        root->addChild(vi);
        root->addChild(vd);
        root->addChild(vb);
        root->addChild(vs);

        std::vector<VarPtr> world;
        world.push_back(root);

        // Write JSON
        VarJSON::write(world, jsonPath.toStdString());

        // Verify file exists
        QVERIFY(QFile::exists(jsonPath));

        // Build a new tree with different defaults, then read
        VarListPtr root2(new VarList("root"));
        VarIntPtr vi2(new VarInt("my_int", 0));
        VarDoublePtr vd2(new VarDouble("my_double", 0.0));
        VarBoolPtr vb2(new VarBool("my_bool", false));
        VarStringPtr vs2(new VarString("my_string", ""));
        root2->addChild(vi2);
        root2->addChild(vd2);
        root2->addChild(vb2);
        root2->addChild(vs2);

        std::vector<VarPtr> world2;
        world2.push_back(root2);

        world2 = VarJSON::read(world2, jsonPath.toStdString());

        // Verify values were restored
        QCOMPARE(vi2->getInt(), 42);
        QVERIFY(qFuzzyCompare(vd2->getDouble(), 3.14));
        QCOMPARE(vb2->getBool(), true);
        QCOMPARE(vs2->getString(), std::string("hello"));
    }

    void getJsonString()
    {
        VarListPtr root(new VarList("test"));
        VarIntPtr vi(new VarInt("val", 99));
        root->addChild(vi);

        std::vector<VarPtr> world;
        world.push_back(root);

        std::string json = VarJSON::getJSON(world);
        QVERIFY(!json.empty());
        QVERIFY(json.find("VarJSON") != std::string::npos);
        QVERIFY(json.find("test") != std::string::npos);
        QVERIFY(json.find("99") != std::string::npos);
    }

    void emptyRead()
    {
        QTemporaryDir tmpDir;
        QVERIFY(tmpDir.isValid());
        QString jsonPath = tmpDir.path() + "/nonexistent.json";

        std::vector<VarPtr> empty;
        std::vector<VarPtr> result = VarJSON::read(empty, jsonPath.toStdString());
        QCOMPARE((int)result.size(), 0);
    }
};

QTEST_MAIN(TestJson)
#include "test_json.moc"
