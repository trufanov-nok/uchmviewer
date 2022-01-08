/*
 *  uChmViewer - a CHM and EPUB file viewer with broad language support
 *  Copyright (C) 2021 Nick Egorrov, nicegorov@yandex.ru
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ASYNC_EBOOK_IMPL_H
#define ASYNC_EBOOK_IMPL_H

#include <QObject>

#include "ebook_chm.h"
#include "ebook_epub.h"
#include "async_ebook.hpp"

/*
 * LoadFile
 **************************************************************/

class LoadFileWorker : public QObject
{
    Q_OBJECT

public slots:
    void query(const QString& archiveName)
    {
        response(EBook::loadFile(archiveName));
        deleteLater();
    }

signals:
    void response(EBook* ebook);
};

class LoadFileClient : public QObject
{
    Q_OBJECT

public:
    LoadFileClient(AsyncEBook* asyncEbook, LoadFileWorker* worker, const LoadFileFunctor& func) :
        m_asyncEbook(asyncEbook), m_func(func)
    {
        QObject::connect(this, &LoadFileClient::query, worker, &LoadFileWorker::query);
        QObject::connect(worker, &LoadFileWorker::response, this, &LoadFileClient::response);
    }

signals:
    void query(const QString& archiveName);

public slots:
    void response(EBook* ebook)
    {
        m_func(m_asyncEbook->setEbook(ebook));
        deleteLater();
    }

protected:
    AsyncEBook* m_asyncEbook;
    LoadFileFunctor m_func;
};

/*
 * getTableOfContents
 **************************************************************/

class GetTocWorker : public QObject
{
    Q_OBJECT

public:
    GetTocWorker(const MetaEBook* Ebook) : m_ebook(Ebook) {}

public slots:
    void query()
    {
        QList<EBookTocEntry> toc;
        bool status =  m_ebook->getTableOfContents(toc);
        response(status, toc);
        deleteLater();
    }

signals:
    void response(bool success, QList<EBookTocEntry> toc);

protected:
    const MetaEBook* m_ebook;
};

class GetTocClient : public QObject
{
    Q_OBJECT

public:
    GetTocClient(MetaEBook* Ebook, GetTocWorker* worker, const GetTocFunctor& func) :
         m_ebook(Ebook), m_func(func)
    {
        QObject::connect(this, &GetTocClient::query, worker, &GetTocWorker::query);
        QObject::connect(worker, &GetTocWorker::response, this, &GetTocClient::response);
    }

signals:
    void query();

public slots:
    void response(bool success, QList<EBookTocEntry> toc)
    {
        m_func(success, toc);
        deleteLater();
    }

protected:
    MetaEBook* m_ebook;
    GetTocFunctor m_func;
};

/*
 * getIndex
 **************************************************************/

class GetIndexWorker : public QObject
{
    Q_OBJECT

public:
    GetIndexWorker(const MetaEBook* Ebook) : m_ebook(Ebook) {}

public slots:
    void query()
    {
        QList<EBookIndexEntry> index;
        bool status = m_ebook->getIndex(index);
        response(status, index);
        deleteLater();
    }

signals:
    void response(bool success, QList<EBookIndexEntry> toc);

protected:
    const MetaEBook* m_ebook;
};

class GetIndexClient : public QObject
{
    Q_OBJECT

public:
    GetIndexClient(MetaEBook* Ebook, GetIndexWorker* worker, const GetIndexFunctor& func) :
        m_ebook(Ebook), m_func(func)
    {
        QObject::connect(this, &GetIndexClient::query, worker, &GetIndexWorker::query);
        QObject::connect(worker, &GetIndexWorker::response, this, &GetIndexClient::response);
    }

signals:
    void query();

public slots:
    void response(bool success, QList<EBookIndexEntry> index)
    {
        m_func(success, index);
        deleteLater();
    }

protected:
    MetaEBook* m_ebook;
    GetIndexFunctor m_func;
};

/*
 * getFileContentAsString
 **************************************************************/

class GetTextWorker : public QObject
{
    Q_OBJECT

public:
    GetTextWorker(const MetaEBook* Ebook) : m_ebook(Ebook) {}

public slots:
    void query(const QUrl url)
    {
        QString text;
        bool status = m_ebook->getFileContentAsString(text, url);
        response(status, text);
        deleteLater();
    }

signals:
    void response(bool success, QString text);

protected:
    const MetaEBook* m_ebook;
};

class GetTextClient : public QObject
{
    Q_OBJECT

public:
    GetTextClient(MetaEBook* Ebook, GetTextWorker* worker, const GetTextFunctor& func) :
        m_ebook(Ebook), m_func(func)
    {
        QObject::connect(this, &GetTextClient::query, worker, &GetTextWorker::query);
        QObject::connect(worker, &GetTextWorker::response, this, &GetTextClient::response);
    }

signals:
    void query(const QUrl url);

public slots:
    void response(bool success, QString text)
    {
        m_func(success, text);
        deleteLater();
    }

protected:
    MetaEBook* m_ebook;
    GetTextFunctor m_func;
};

/*
 * getFileContentAsBinary
 **************************************************************/

class GetBinWorker : public QObject
{
    Q_OBJECT

public:
    GetBinWorker(const MetaEBook* Ebook) : m_ebook(Ebook) {}

public slots:
    void query(const QUrl url)
    {
        QByteArray bin;
        bool status = m_ebook->getFileContentAsBinary(bin, url);
        response(status, bin);
        deleteLater();
    }

signals:
    void response(bool success, QByteArray bin);

protected:
    const MetaEBook* m_ebook;
};

class GetBinClient : public QObject
{
    Q_OBJECT

public:
    GetBinClient(MetaEBook* Ebook, GetBinWorker* worker, const GetBinFunctor& func) :
        m_ebook(Ebook), m_func(func)
    {
        QObject::connect(this, &GetBinClient::query, worker, &GetBinWorker::query);
        QObject::connect(worker, &GetBinWorker::response, this, &GetBinClient::response);
    }

signals:
    void query(const QUrl url);

public slots:
    void response(bool success, QByteArray bin)
    {
        m_func(success, bin);
        deleteLater();
    }

protected:
    MetaEBook* m_ebook;
    GetBinFunctor m_func;
};

#endif // ASYNC_EBOOK_IMPL_H
