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

#ifndef DUMMY_ASYNC_EBOOK_H
#define DUMMY_ASYNC_EBOOK_H

#include <QThread>

#include "base_async_ebook.hpp"


class MetaEBook;

/**
 * @brief Implementation of BaseAsyncEBook interface.
 * 
 * This class implements all methods of the EBook and BaseAsyncEBook classes
 * with multi-threading support, but does not contain a separate thread.
 *
 * A derived class with a separate thread must override the following
 * asynchronous methods:
 * @li loadFile(const QString&, const LoadFileFunctor&) 
 * @li getTableOfContents(const GetTocFunctor&)
 * @li getIndex(const GetIndexFunctor&)
 * @li getFileContentAsString(const QUrl&, const GetTextFunctor&)
 * @li getFileContentAsBinary(const QUrl&, const GetBinFunctor&)
 */
class DummyAsyncEBook : public BaseAsyncEBook
{
public:
    DummyAsyncEBook();

    ~DummyAsyncEBook();

    bool loadFile(const QString& file) override;

    void loadFile(const QString& file, const LoadFileFunctor& func) override;

    void close() override;

    bool getTableOfContents(QList<EBookTocEntry>& toc) const override;

    void getTableOfContents(const GetTocFunctor& func) const override;

    bool getIndex(QList<EBookIndexEntry>& index) const override;

    void getIndex(const GetIndexFunctor& func) const override;

    bool getFileContentAsString(QString& str, const QUrl& url) const override;

    void getFileContentAsString(const QUrl& url, const GetTextFunctor& func) const override;

    bool getFileContentAsBinary(QByteArray& data, const QUrl& url) const override;

    void getFileContentAsBinary(const QUrl& url, const GetBinFunctor& func) const override;

    bool isLoaded() const override;

    QString title() const override;

    QUrl homeUrl() const override;

    bool hasFeature(EBook::Feature code) const override;

    bool enumerateFiles(QList<QUrl>& files) override;

    QString getTopicByUrl(const QUrl& url) override;

    QString currentEncoding() const override;

    bool setCurrentEncoding(const char* encoding) override;

    bool isSupportedUrl(const QUrl& url) override;

    QUrl pathToUrl(const QString& link) const override;

    QString urlToPath(const QUrl& url) const override;

protected:
    bool m_loaded = false;
    MetaEBook* m_metaEbook;

    bool setEbook(EBook* ebook);
};


/**
 * @brief Class for calling EBook functions in a separate thread.
 */
class MetaEBook : public QObject
{
    Q_OBJECT
public:
    MetaEBook();
    
    ~MetaEBook();
    
    inline bool isCurrentThread()
    {
        return this->thread() == QThread::currentThread();
    }

    inline bool isLoaded() const
    {
        return m_ebook != nullptr;
    }

    bool setEbook(EBook* ebook);

    inline EBook* ebook();

    QString title() const;

    QUrl homeUrl() const;

    bool hasFeature(EBook::Feature code) const;

    bool enumerateFiles(QList<QUrl>& files);

    QString getTopicByUrl(const QUrl& url);

    QString currentEncoding() const;

    bool setCurrentEncoding(const char* encoding);

    bool isSupportedUrl(const QUrl& url);

    QUrl pathToUrl(const QString& link) const;

    QString urlToPath(const QUrl& url) const;

protected:
    EBook* m_ebook;
};

#endif // DUMMY_ASYNC_EBOOK_H
