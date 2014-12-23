TEMPLATE = lib
TARGET   = bit7z
VERSION  = 1.0
CONFIG  += staticlib
CONFIG  -= app_bundle
CONFIG  -= qt

SOURCES += lib/7zSDK/CPP/Windows/Error.cpp \
           lib/7zSDK/CPP/Windows/DLL.cpp \
           lib/7zSDK/CPP/Windows/FileIO.cpp \
           lib/7zSDK/CPP/Windows/FileDir.cpp \
           lib/7zSDK/CPP/Windows/FileName.cpp \
           lib/7zSDK/CPP/Windows/FileFind.cpp \
           lib/7zSDK/CPP/Windows/COM.cpp \
           lib/7zSDK/CPP/Windows/PropVariant.cpp \
           lib/7zSDK/CPP/7zip/Common/FileStreams.cpp \
           lib/7zSDK/CPP/Common/IntToString.cpp \
           lib/7zSDK/CPP/Common/MyVector.cpp \
           src/bitextractor.cpp \
           src/bitcompressor.cpp \
           src/bit7zlibrary.cpp \
           src/bitexception.cpp \
           src/bitguids.cpp \
           src/extractcallback.cpp \
           src/opencallback.cpp \
           src/updatecallback.cpp \
           src/fsitem.cpp \
           src/fsindexer.cpp \
           src/fsutil.cpp \
           src/callback.cpp \
           src/bitformat.cpp

INCLUDEPATH += lib/7zSDK/CPP/

CONFIG  += embed_manifest_dll

QMAKE_CFLAGS_WARN_ON -= -W3
QMAKE_CFLAGS_WARN_ON += -W4

DEFINES += _UNICODE

HEADERS += include/bitcompressor.hpp \
           include/bit7zlibrary.hpp \
           include/bitexception.hpp \
           include/bitguids.hpp \
           include/bitextractor.hpp \
           include/extractcallback.hpp \
           include/opencallback.hpp \
           include/updatecallback.hpp \
           include/fsitem.hpp \
           include/fsindexer.hpp \
           include/fsutil.hpp \
           include/bit7z.hpp \
           include/callback.hpp \
           include/bitformat.hpp \
           include/bitcompressionlevel.hpp

contains(QMAKE_HOST.arch, x86_64) {
    QMAKE_LFLAGS         += /MACHINE:X64
    PLATFORM = x64
} else {
    QMAKE_LFLAGS         += /MACHINE:X86
    PLATFORM = x86
}

CONFIG(debug, debug|release) {
    BUILD = debug
} else {
    BUILD = release
}

DESTDIR  = $$PWD/bin/$${PLATFORM}/
OBJECTS_DIR = $$PWD/build/$${PLATFORM}/$${BUILD}/.obj
RCC_DIR  = $$PWD/build/$${PLATFORM}/$${BUILD}/.rcc
