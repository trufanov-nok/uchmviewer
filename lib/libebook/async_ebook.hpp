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

#ifndef ASYNC_EBOOK_H
#define ASYNC_EBOOK_H

#include <QThread>

#include "dummy_async_ebook.hpp"

class AsyncEBook : public DummyAsyncEBook
{
public:
    AsyncEBook();

    ~AsyncEBook();

    void loadFile(const QString& file, const LoadFileFunctor& func) override;

    void getTableOfContents( const GetTocFunctor& func) const override;

    void getIndex(const GetIndexFunctor& func) const override;

    void getFileContentAsString(const QUrl& url, const GetTextFunctor& func) const override;

    void getFileContentAsBinary(const QUrl& url, const GetBinFunctor& func) const override;

protected:
    friend class LoadFileClient;

    QThread*    m_mainThread;
    QThread*    m_contentsThread;
    QThread*    m_indexThread;
};

#endif // ASYNC_EBOOK_H
