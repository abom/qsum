#include "sum.h"

#define bufSize 16*1024

QString Sum::toSize(qint64 size)
{
    //FIXME: rewrite using the value of 1024^n directly
    QString sSize;
    qreal res;
    if(size < 1024)
        sSize = tr("%1 B").arg(size);
    else if(size > 1024 && size < qPow(1024, 2))
    {
        res = (qreal) qRound((size / 1024) * 100) / 100;
        sSize = tr("%1 KB").arg(res);
    }
    else if(size > qPow(1024, 2) && size < qPow(1024, 3))
    {
        res = (qreal) qRound((size / qPow(1024, 2)) * 100) / 100;
        sSize = tr("%1 MB").arg(res);
    }
    else if(size > qPow(1024, 3) && size < qPow(1024, 4))
    {
        res = (qreal) qRound((size / qPow(1024, 3)) * 100) / 100;
        sSize = tr("%1 GB").arg(res);
    }
    return sSize;
}

QString Sum::toTime(int secs)
{
    return QTime().addSecs(secs).toString("hh:mm:ss"); //.zzz");
}

Sum::Sum(const QString &filename)
{
    m_stop = false;
    hashSizes[md5] = 16;
    hashSizes[sha1] = 20;
    hashSizes[sha224] = 28;
    hashSizes[sha256] = 32;
    hashSizes[sha384] = 48;
    hashSizes[sha512] = 64;

    if(!filename.isEmpty()) setFilename(filename);
}

void Sum::setFilename(const QString &filename)
{
    QFileInfo info(filename);
    if(info.exists())
    {
        m_filename = filename;
        m_size = info.size();
    }
}

QString Sum::filename() const
{
    return m_filename;
}

void Sum::setAlgo(unsigned int algo)
{
    m_algo = static_cast<Algorithm>(algo);
}

void Sum::stop(bool st)
{
    m_stop = st;
}

bool Sum::stopped() const
{
    return m_stop || m_result == "stopped";
}

void Sum::update_md5(unsigned char *buf, size_t size)
{
    md5_update(&md5_ctx, buf, size);
}

void Sum::update_sha1(unsigned char *buf, size_t size)
{
    sha1_update(&sha1_ctx, buf, size);
}

void Sum::update_sha2(unsigned char *buf, size_t size)
{
    sha2_update(&sha2_ctx, buf, size);
}

void Sum::update_sha4(unsigned char *buf, size_t size)
{
    sha4_update(&sha4_ctx, buf, size);
}

bool Sum::init_algo()
{
    switch(m_algo)
    {
    case md5:
        md5_starts(&md5_ctx);
        update_func = &Sum::update_md5;
        break;
    case sha1:
        sha1_starts(&sha1_ctx);
        update_func = &Sum::update_sha1;
        break;
    case sha224:
        sha2_starts(&sha2_ctx, 1);
        update_func = &Sum::update_sha2;
        break;
    case sha256:
        sha2_starts(&sha2_ctx, 0);
        update_func = &Sum::update_sha2;
        break;
    case sha384:
        sha4_starts(&sha4_ctx, 1);
        update_func = &Sum::update_sha4;
        break;
    case sha512:
        sha4_starts(&sha4_ctx, 0);
        update_func = &Sum::update_sha4;
        break;
    default: return false; break;
    }

    emit started();
    startTime = QTime::currentTime();
}

bool Sum::update_algo(QFile &input)
{
    char buf[bufSize];
    qint64 size;

    size = input.read(buf, bufSize);
    (this->*update_func)((unsigned char*) buf, size);
    m_currentPos = input.pos();
    emit hashChanged();
}

bool Sum::finish_algo()
{
    unsigned char out[hashSizes[m_algo]];

    switch(m_algo)
    {
    case md5: md5_finish(&md5_ctx, out); break;
    case sha1: sha1_finish(&sha1_ctx, out); break;
    case sha224:
    case sha256: sha2_finish(&sha2_ctx, out); break;
    default: sha4_finish(&sha4_ctx, out); break;
    }

    if(m_stop)
    {
        m_result = "stopped";
        stop(false);
    }
    else
         m_result = QString(QByteArray((const char*) out, hashSizes[m_algo]).toHex());

    emit finished();
}

bool Sum::sum()
{
    QFile inFile(m_filename);


    if(inFile.open(QIODevice::ReadOnly))
    {
        if(!init_algo()) return false;
        while(!inFile.atEnd())
        {
            qApp->processEvents();
            if(m_stop) break;
            update_algo(inFile);
        }
        inFile.close();
    } else return false;
    finish_algo();
    return true;
}

QString Sum::result()
{
    return m_result;
}

qint64 Sum::size()
{
    return m_size;
}

qint64 Sum::currentPos()
{
    return m_currentPos;
}

int Sum::elapsed()
{
    return startTime.secsTo(QTime::currentTime());
}

int Sum::estimated()
{
    return (size() - m_currentPos) / readRate();
}

float Sum::readRate()
{
    return (1.0 * m_currentPos / elapsed());
}
