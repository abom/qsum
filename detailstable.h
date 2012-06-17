#ifndef DETAILSTABLE_H
#define DETAILSTABLE_H

#include <QTableWidget>
#include <QHeaderView>
#include "sum.h"

class DetailsTable : public QTableWidget
{
    Q_OBJECT

public slots:
    void setFilename(const QString &filename);
    void setSize(qint64 size);

public:
    DetailsTable(QWidget *parent = 0);
    ~DetailsTable();

    //void updateDetails(qint64 pos, float speed, int elapsed, int estimated);
    void updateDetails(Sum *sumlib);
private:
    QString m_filename;
};

#endif // DETAILSTABLE_H
