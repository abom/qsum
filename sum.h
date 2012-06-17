#ifndef SUM_H
#define SUM_H

#include <QObject>
#include <QTime>
#include <QCryptographicHash>
#include <QFileInfo>
#include <QtCore/qmath.h>
#include <QApplication>
#include <QMap>
#include "polarssl/config.h"
#include "polarssl/md5.h"
#include "polarssl/sha1.h"
#include "polarssl/sha2.h"
#include "polarssl/sha4.h"

class Sum : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString filename READ filename WRITE setFilename)

public:
    Sum(const QString &filename = "");
    QString filename() const;
    bool stopped() const;
    static QString toSize(qint64 size);
    static QString toTime(int secs);

private:
    QString m_filename;
    md5_context md5_ctx;
    sha1_context sha1_ctx;
    sha2_context sha2_ctx;
    sha4_context sha4_ctx;
    bool m_stop;
    QTime startTime;
    QString m_result;
    qint64 m_size;
    qint64 m_currentPos;

    enum Algorithm{ md5, sha1, sha224, sha256, sha384, sha512 };
    Algorithm m_algo;

    QMap<Algorithm, int> hashSizes;

    void (Sum::*update_func)(unsigned char*, size_t);

    void update_md5(unsigned char *buf, size_t size);
    void update_sha1(unsigned char *buf, size_t size);
    void update_sha2(unsigned char *buf, size_t size);
    void update_sha4(unsigned char *buf, size_t size);

    bool init_algo();
    bool update_algo(QFile &input);
    bool finish_algo();

public slots:
    bool sum();
    float readRate();
    int elapsed();
    int estimated();
    QString result();
    void setFilename(const QString &filename);
    void setAlgo(unsigned int algo);
    void stop(bool st);
    qint64 size();
    qint64 currentPos();

signals:
    void started();
    void hashChanged();
    void finished();

};
#endif // SUM_H
