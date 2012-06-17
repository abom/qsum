#include "detailstable.h"

DetailsTable::DetailsTable(QWidget *parent)
    : QTableWidget(6, 1, parent)
{
    horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    verticalHeader()->setResizeMode(QHeaderView::Stretch);
    verticalHeader()->setDefaultSectionSize(70);

    QStringList verticalHeaderLabels;
    verticalHeaderLabels << tr("Filename:") <<  tr("Size:") << tr("Reading speed:")
                 << tr("Reading:") << tr("Elapsed:") <<  tr("Estimated:");

    setVerticalHeaderLabels(verticalHeaderLabels);
    setHorizontalHeaderLabels(QStringList() << tr("Details"));

    for(int i=0; i<6; i++)
        setItem(i, 0, new QTableWidgetItem(""));

    setSelectionBehavior(QAbstractItemView::SelectRows);
}

DetailsTable::~DetailsTable()
{

}

void DetailsTable::setFilename(const QString &filename)
{
    m_filename = filename;
    item(0, 0)->setText(filename.split("\\").last());
}

void DetailsTable::setSize(qint64 size)
{
    item(1, 0)->setText(Sum::toSize(size));
}

//void DetailsTable::updateDetails(qint64 pos, float speed, int elapsed, int estimated)
void DetailsTable::updateDetails(Sum *sumlib)
{
//    item(2, 0)->setText(Sum::toSize((qint64) speed) + tr("/sec"));
//    item(3, 0)->setText(Sum::toSize(pos));
//    item(4, 0)->setText(Sum::toTime(elapsed));
//    item(5, 0)->setText(Sum::toTime(estimated));
    item(2, 0)->setText(Sum::toSize((qint64) sumlib->readRate()) + tr("/sec"));
    item(3, 0)->setText(Sum::toSize(sumlib->currentPos()));
    item(4, 0)->setText(Sum::toTime(sumlib->elapsed()));
    item(5, 0)->setText(Sum::toTime(sumlib->estimated()));
}
