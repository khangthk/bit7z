/*
 * bit7z - A C++ static library to interface with the 7-zip shared libraries.
 * Copyright (c) 2014-2022 Riccardo Ostani - All Rights Reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <bitfs.hpp>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifdef _WIN32
#include <array>
#include <Windows.h>
#elif defined( __APPLE__ )
#include <libproc.h> // for proc_pidpath and PROC_PIDPATHINFO_MAXSIZE
#include <unistd.h> // for getpid
#endif

#include <internal/fs.hpp>

namespace bit7z { // NOLINT(modernize-concat-nested-namespaces)
namespace test {
namespace filesystem {

inline auto exe_path() -> fs::path {
#ifdef _WIN32
    std::array< wchar_t, MAX_PATH > path{ 0 };
    GetModuleFileNameW( nullptr, path.data(), MAX_PATH );
    return path.data();
#elif defined( __APPLE__ )
    std::array< char, PROC_PIDPATHINFO_MAXSIZE > result{ 0 };
    ssize_t result_size = proc_pidpath( getpid(), result.data(), result.size() );
    return result_size > 0 ? std::string( result.data(), result_size ) : "";
#else
    std::error_code error;
    const fs::path result = fs::read_symlink( "/proc/self/exe", error );
    return error ? "" : result;
#endif
}

#ifdef BIT7Z_TESTS_FILESYSTEM

constexpr auto test_data_dir = BIT7Z_TESTS_DATA_DIR;
constexpr auto test_filesystem_dir = BIT7Z_TESTS_DATA_DIR "/test_filesystem";

inline auto current_dir() -> fs::path {
    std::error_code ec;
    return fs::current_path( ec );
}

inline auto set_current_dir( const fs::path& dir ) -> bool {
    std::error_code ec;
    fs::current_path( dir, ec );
    return !ec;
}

#endif

} // namespace filesystem
} // namespace test
} // namespace bit7z

#endif //FILESYSTEM_HPP
