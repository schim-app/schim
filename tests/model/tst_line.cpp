#include <QtTest/QtTest>

#include "model/line.h"

class TestLine : public QObject
{
    Q_OBJECT
private slots:
    void testOperatorEqual()
    {
        Line l1(0, 10, 20.5, 30.1);
        Line *l2 = static_cast<Line*>(l1.clone());
        QVERIFY(l1 == *l2);
        QVERIFY(!(l1 != *l2));
        l1.setPos({-1, -1});
        QVERIFY(l1 != *l2);
        QVERIFY(!(l1 == *l2));
        delete l2;
    }
};

QTEST_MAIN(TestLine)
#include "tst_line.moc"
