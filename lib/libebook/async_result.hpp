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

#include <atomic>

#include <QAbstractEventDispatcher>

/**
 * @brief Class template similar to @c atomic with an additional wait() method.
 */
template< class T >
class AsyncResult {
    std::atomic<T> value;
    std::atomic<bool> flag;

public:
    AsyncResult()
    {
        flag.store(true);
    }

    ~AsyncResult(){}

    T load(std::memory_order order = std::memory_order_seq_cst) const
    {
        return value.load(order);
    }

    void store(T newValue, std::memory_order order = std::memory_order_seq_cst)
    {
        value.store(newValue, order);
        flag.store(false);
    }

    T operator=(T newValue)
    {
        store(newValue);
        return newValue;
    }

    /**
     * @brief Wait for the result to be ready.
     */
    void wait() const
    {
        QAbstractEventDispatcher* dispatcher = QAbstractEventDispatcher::instance(0);

        while (flag.load()) {
            dispatcher->processEvents(QEventLoop::AllEvents);
        }
    }
};
