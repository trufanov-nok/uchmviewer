
TEMPLATE = lib
TARGET = ebook
CONFIG *= c++11 warn_on staticlib
QT += widgets

HEADERS += \
    bitfiddle.h \
    ebook_chm.h \
    ebook_epub.h \
    ebook.h \
    ebook_chm_encoding.h \
    ebook_search.h \
    helper_entitydecoder.h \
    helper_search_index.h \
    helperxmlhandler_epubcontainer.h \
    helperxmlhandler_epubcontent.h \
    helperxmlhandler_epubtoc.h \
    base_async_ebook.hpp \
    async_result.hpp \
    dummy_async_ebook.hpp

SOURCES += \
    ebook_chm.cpp \
    ebook_epub.cpp \
    ebook.cpp \
    ebook_chm_encoding.cpp \
    ebook_search.cpp \
    helper_entitydecoder.cpp \
    helper_search_index.cpp \
    helperxmlhandler_epubcontainer.cpp \
    helperxmlhandler_epubcontent.cpp \
    helperxmlhandler_epubtoc.cpp \
    dummy_async_ebook.cpp

!isEmpty(USE_STATIC_CHMLIB) {
    INCLUDEPATH *= ../CHMLib/src
} else {
    !isEmpty(CHMLIB_ROOT): INCLUDEPATH *= "$$CHMLIB_ROOT/include"
}

!isEmpty(LIBZIP_ROOT): INCLUDEPATH *= "$$LIBZIP_ROOT/include"

!isEmpty(USE_ASYNCEBOOK) {
    DEFINES += USE_ASYNCEBOOK
    HEADERS += async_ebook.hpp async_ebook_impl.hpp
    SOURCES += async_ebook.cpp
}
