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

#ifndef BASE_ASYNC_EBOOK_H
#define BASE_ASYNC_EBOOK_H

#include <functional>

#include <QByteArray>
#include <QList>
#include <QString>
#include <QUrl>

#include "ebook.h"


class BaseAsyncEBook;

typedef std::function<void(bool success)>                               LoadFileFunctor;
typedef std::function<void(bool success, QList<EBookTocEntry> toc)>     GetTocFunctor;
typedef std::function<void(bool success, QList<EBookIndexEntry> index)> GetIndexFunctor;
typedef std::function<void(bool success, QString text)>                 GetTextFunctor;
typedef std::function<void(bool success, QByteArray data)>              GetBinFunctor;

/**
 * @brief Interface for asynchronous operations with EBook
 *
 * Important:
 * @li The load(const QString&) function not work and allways return @c false.
 * @li Use the non-static loadFile(const QString&) function to load e-book.
 * @li If there is not a loaded e-book and isLoaded() return @c false,
 *     functions returns empty result or @c false.
 */
class BaseAsyncEBook : public EBook
{
public:
    /**
     * @brief Attempts to load e-book.
     *
     * @details If the e-book was successfully loaded, the internal flag is set
     * to @c true and the isLoaded() function returns @c true. The previous
     * e-book, if any, is deleted.
     *
     * If the e-book could not be loaded, the internal flag is not changed and
     * the previous e-book is not deleted.
     *
     * @param file The path to an e-book file.
     * @return True if the e-book was successfully loaded.
     */
    virtual bool loadFile(const QString& file) = 0;

    /**
     * @brief Attempts to load e-book asynchronously.
     *
     * @details If the e-book was successfully loaded, the internal flag is set
     * to @c true and the isLoaded() function returns @c true. The previous
     * e-book, if any, is deleted.
     *
     * If the e-book could not be loaded, the internal flag is not changed and
     * the previous e-book is not deleted.
     *
     * @param file The path to an e-book file.
     * @param func Callback function to receive result.
     */
    virtual void loadFile(const QString& file, const LoadFileFunctor& func) = 0;

    /**
     * @brief Whether the e-book has been loaded.
     *
     * @details The function returns the state of the internal flag.
     *
     * If the e-book was successfully loaded, the internal flag is set to
     * @c true. If the close() function was called, the internal flag is reset
     * to @c false.
     *
     * @return True, if there is a loaded e-book.
     * @see loadFile @see close
     */
    virtual bool isLoaded() const = 0;

    virtual void getTableOfContents(const GetTocFunctor& func) const = 0;

    virtual void getIndex(const GetIndexFunctor& func) const  = 0;

    virtual void getFileContentAsString(const QUrl& url, const GetTextFunctor& func) const  = 0;

    virtual void getFileContentAsBinary(const QUrl& url, const GetBinFunctor& func) const  = 0;

    // Overriding synchronous methods from EBook.
    bool getTableOfContents(QList<EBookTocEntry>& toc) const override = 0;

    bool getIndex(QList<EBookIndexEntry>& index) const override = 0;

    bool getFileContentAsString(QString& str, const QUrl& url) const override = 0;

    bool getFileContentAsBinary(QByteArray& data, const QUrl& url) const override = 0;

protected:
    /**
     * This class (and its derived classes) are not an implementation of any e-book
     * format and should not load files directly.
     *
     * @param file Not used.
     * @return @c false.
     */
    bool load(const QString& file) override final
    {
        Q_UNUSED(file);
        return false;
    }
};

#endif // BASE_ASYNC_EBOOK_H
