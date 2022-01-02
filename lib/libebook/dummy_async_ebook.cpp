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

#include <QThread>
#include <QMetaObject>
#include <QMetaType>

#include "async_result.hpp"
#include "dummy_async_ebook.hpp"

DummyAsyncEBook::DummyAsyncEBook()
{
    qRegisterMetaType<QList<EBookTocEntry>>("QList<EBookTocEntry>");
    qRegisterMetaType<QList<EBookIndexEntry>>("QList<EBookIndexEntry>");
    m_metaEbook = new MetaEBook();
}

DummyAsyncEBook::~DummyAsyncEBook()
{
    delete m_metaEbook;
}

bool DummyAsyncEBook::loadFile(const QString& file)
{
    AsyncResult<bool> result;
    loadFile(file, [&](bool success) {
        result = success;
    });

    result.wait();
    return result.load();
}

void DummyAsyncEBook::loadFile(const QString& file, const LoadFileFunctor& func)
{
    bool result = false;
    EBook* ebook = EBook::loadFile(file);

    if (ebook) {
        result = setEbook(ebook);
    }

    func(result);
}

bool DummyAsyncEBook::isLoaded() const
{
    return m_loaded;
}

void DummyAsyncEBook::close()
{
    setEbook(nullptr);
}

bool DummyAsyncEBook::getTableOfContents(QList<EBookTocEntry>& toc) const
{
    AsyncResult<bool> result;
    getTableOfContents([&](bool success, QList<EBookTocEntry> data) {
        result = success;
        toc = data;
    });

    result.wait();
    return result.load();
}

void DummyAsyncEBook::getTableOfContents(const GetTocFunctor& func) const
{
    bool success = false;
    QList<EBookTocEntry> toc;

    if (isLoaded()) {
        success = m_metaEbook->ebook()->getTableOfContents(toc);
    }

    func(success, toc);
}

bool DummyAsyncEBook::getIndex(QList<EBookIndexEntry>& index) const
{
    AsyncResult<bool> result;
    getIndex([&](bool success, QList<EBookIndexEntry> data) {
        result = success;
        index = data;
    });

    result.wait();
    return result.load();
}

void DummyAsyncEBook::getIndex(const GetIndexFunctor& func) const
{
    bool success = false;
    QList<EBookIndexEntry> index;

    if (isLoaded()) {
        success = m_metaEbook->ebook()->getIndex(index);
    }

    func(success, index);
}

bool DummyAsyncEBook::getFileContentAsString(QString& str, const QUrl& url) const
{
    AsyncResult<bool> result;
    getFileContentAsString(url, [&](bool success, QString data) {
        result = success;
        str = data;
    });

    result.wait();
    return result.load();
}

void DummyAsyncEBook::getFileContentAsString(const QUrl& url, const GetTextFunctor& func) const
{
    bool success = false;
    QString text;

    if (isLoaded()) {
        success = m_metaEbook->ebook()->getFileContentAsString(text, url);
    }

    func(success, text);
}

bool DummyAsyncEBook::getFileContentAsBinary(QByteArray& data, const QUrl& url) const
{
    AsyncResult<bool> result;
    getFileContentAsBinary(url, [&](bool success, QByteArray bin) {
        result = success;
        data = bin;
    });

    result.wait();
    return result.load();
}

void DummyAsyncEBook::getFileContentAsBinary(const QUrl& url, const GetBinFunctor& func) const
{
    bool success = false;
    QByteArray bin;

    if (isLoaded()) {
        success = m_metaEbook->ebook()->getFileContentAsBinary(bin, url);
    }

    func(success, bin);
}


/*
 * Overriding synchronous EBook methods.
 * If the calling thread and the metaobject thread are the same, the EBook
 * method is called directly. If the threads are different, the EBook method
 * is called via QMetaObject::invokeMethod with the current thread locked.
 *****************************************************************************/

QString DummyAsyncEBook::title() const
{
    if (m_metaEbook->isCurrentThread()) {
        return m_metaEbook->title();
    }

    QString result;
    QMetaObject::invokeMethod(m_metaEbook,
                              "title",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(QString, result));
    return result;
}

QUrl DummyAsyncEBook::homeUrl() const
{
    if (m_metaEbook->isCurrentThread()) {
        return m_metaEbook->homeUrl();
    }

    QUrl result;
    QMetaObject::invokeMethod(m_metaEbook,
                              "homeUrl",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(QUrl, result));
    return result;
}

bool DummyAsyncEBook::hasFeature(EBook::Feature code) const
{
    if (m_metaEbook->isCurrentThread()) {
        return m_metaEbook->hasFeature(code);
    }

    bool result;
    QMetaObject::invokeMethod(m_metaEbook,
                              "hasFeature",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(bool, result),
                              Q_ARG(EBook::Feature, code));
    return result;
}

bool DummyAsyncEBook::enumerateFiles(QList<QUrl>& files)
{
    if (m_metaEbook->isCurrentThread()) {
        return m_metaEbook->enumerateFiles(files);
    }

    bool result;
    QMetaObject::invokeMethod(m_metaEbook,
                              "enumerateFiles",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(bool, result),
                              Q_ARG(QList<QUrl>&, files));
    return result;
}

QString DummyAsyncEBook::getTopicByUrl(const QUrl& url)
{
    if (m_metaEbook->isCurrentThread()) {
        return m_metaEbook->getTopicByUrl(url);
    }

    QString result;
    QMetaObject::invokeMethod(m_metaEbook,
                              "getTopicByUrl",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(QString, result),
                              Q_ARG(const QUrl&, url));
    return result;
}

QString DummyAsyncEBook::currentEncoding() const
{
    if (m_metaEbook->isCurrentThread()) {
        return m_metaEbook->currentEncoding();
    }

    QString result;
    QMetaObject::invokeMethod(m_metaEbook,
                              "currentEncoding",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(QString, result));
    return result;
}

bool DummyAsyncEBook::setCurrentEncoding(const char* encoding)
{
    if (m_metaEbook->isCurrentThread()) {
        return m_metaEbook->setCurrentEncoding(encoding);
    }

    bool result;
    QMetaObject::invokeMethod(m_metaEbook,
                              "setCurrentEncoding",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(bool, result),
                              Q_ARG(const char*, encoding));
    return result;
}

bool DummyAsyncEBook::isSupportedUrl(const QUrl& url)
{
    if (m_metaEbook->isCurrentThread()) {
        return m_metaEbook->isSupportedUrl(url);
    }

    bool result;
    QMetaObject::invokeMethod(m_metaEbook,
                              "isSupportedUrl",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(bool, result),
                              Q_ARG(const QUrl&, url));
    return result;
}

QUrl DummyAsyncEBook::pathToUrl(const QString& link) const
{
    if (m_metaEbook->isCurrentThread()) {
        return m_metaEbook->pathToUrl(link);
    }

    QUrl result;
    QMetaObject::invokeMethod(m_metaEbook,
                              "pathToUrl",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(QUrl, result),
                              Q_ARG(const QString&, link));
    return result;
}

QString DummyAsyncEBook::urlToPath(const QUrl& url) const
{
    if (m_metaEbook->isCurrentThread()) {
        return m_metaEbook->urlToPath(url);
    }

    QString result;
    QMetaObject::invokeMethod(m_metaEbook,
                              "urlToPath",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(QString, result),
                              Q_ARG(const QUrl&, url));
    return result;
}

EBook* DummyAsyncEBook::ebook() const
{
    return m_metaEbook->ebook();
}

bool DummyAsyncEBook::setEbook(EBook* ebook)
{
    bool result;

    if (m_metaEbook->isCurrentThread()) {
        result = m_metaEbook->setEbook(ebook);
    } else {
        QMetaObject::invokeMethod(m_metaEbook,
                                  "setEbook",
                                  Qt::BlockingQueuedConnection,
                                  Q_RETURN_ARG(bool, result),
                                  Q_ARG(EBook*, ebook));
    }

    m_loaded = result;
    return result;
}


/*
 * MetaEBook
 ********************************************/

MetaEBook::MetaEBook()
{
    m_ebook = nullptr;
}

MetaEBook::~MetaEBook()
{
    if (m_ebook != nullptr) {
        delete m_ebook;
    }
}

bool MetaEBook::setEbook(EBook* ebook)
{
    if (m_ebook != nullptr) {
        delete m_ebook;
    }

    m_ebook = ebook;
    return m_ebook != nullptr;
}

QString MetaEBook::title() const
{
    if (isLoaded()) {
        return m_ebook->title();
    }  else {
        return QString();
    }
}

QUrl MetaEBook::homeUrl() const
{
    if (isLoaded()) {
        return m_ebook->homeUrl();
    }  else {
        return QUrl();
    }
}

bool MetaEBook::hasFeature(EBook::Feature code) const
{
    if (isLoaded()) {
        return m_ebook->hasFeature(code);
    }  else {
        return false;
    }
}

bool MetaEBook::enumerateFiles(QList<QUrl>& files)
{
    if (isLoaded()) {
        return m_ebook->enumerateFiles(files);
    }  else {
        return false;
    }
}

QString MetaEBook::getTopicByUrl(const QUrl& url)
{
    if (isLoaded()) {
        return m_ebook->getTopicByUrl(url);
    }  else {
        return QString();
    }
}

QString MetaEBook::currentEncoding() const
{
    if (isLoaded()) {
        return m_ebook->currentEncoding();
    }  else {
        return QString();
    }
}

bool MetaEBook::setCurrentEncoding(const char* encoding)
{
    if (isLoaded()) {
        return m_ebook->setCurrentEncoding(encoding);
    }  else {
        return false;
    }
}

bool MetaEBook::isSupportedUrl(const QUrl& url)
{
    if (isLoaded()) {
        return m_ebook->isSupportedUrl(url);
    }  else {
        return false;
    }
}

QUrl MetaEBook::pathToUrl(const QString& link) const
{
    if (isLoaded()) {
        return m_ebook->pathToUrl(link);
    }  else {
        return QUrl();
    }
}

QString MetaEBook::urlToPath(const QUrl& url) const
{
    if (isLoaded()) {
        return m_ebook->urlToPath(url);
    }  else {
        return QString();
    }
}
