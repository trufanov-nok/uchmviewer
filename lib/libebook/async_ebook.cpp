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

#include <QApplication>
#include <QObject>

#include "async_ebook_impl.hpp"


AsyncEBook::AsyncEBook()
{
    m_mainThread = new QThread();
    m_mainThread->start();
    m_metaEbook->moveToThread(m_mainThread);
    m_contentsThread = new QThread();
    m_contentsThread->start();
    m_indexThread = new QThread();
    m_indexThread->start();
}

AsyncEBook::~AsyncEBook()
{
    m_metaEbook->lock();
    m_mainThread->exit();
    m_contentsThread->exit();
    m_indexThread->exit();
    m_mainThread->wait();
    m_contentsThread->wait();
    m_indexThread->wait();
    delete m_mainThread;
    delete m_contentsThread;
    delete m_indexThread;
}

void AsyncEBook::loadFile(const QString& file, const LoadFileFunctor& func)
{
    LoadFileWorker* worker = new LoadFileWorker();
    worker->moveToThread(m_mainThread);
    LoadFileClient* client = new LoadFileClient(this, worker, func);
    client->query(file);
}

void AsyncEBook::getTableOfContents(const GetTocFunctor& func) const
{
    GetTocWorker* worker = new GetTocWorker(m_metaEbook);
    worker->moveToThread(m_contentsThread);
    GetTocClient* client = new GetTocClient(m_metaEbook, worker, func);
    client->query();
}

void AsyncEBook::getIndex(const GetIndexFunctor& func) const
{
    GetIndexWorker* worker = new GetIndexWorker(m_metaEbook);
    worker->moveToThread(m_indexThread);
    GetIndexClient* client = new GetIndexClient(m_metaEbook, worker, func);
    client->query();
}

void AsyncEBook::getFileContentAsString(const QUrl& url, const GetTextFunctor& func) const
{
    GetTextWorker* worker = new GetTextWorker(m_metaEbook);
    worker->moveToThread(m_mainThread);
    GetTextClient* client = new GetTextClient(m_metaEbook, worker, func);
    client->query(url);
}

void AsyncEBook::getFileContentAsBinary(const QUrl& url, const GetBinFunctor& func) const
{
    GetBinWorker* worker = new GetBinWorker(m_metaEbook);
    worker->moveToThread(m_mainThread);
    GetBinClient* client = new GetBinClient(m_metaEbook, worker, func);
    client->query(url);
}
