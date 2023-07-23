// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/*
 * bit7z - A C++ static library to interface with the 7-zip shared libraries.
 * Copyright (c) 2014-2023 Riccardo Ostani - All Rights Reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <algorithm>
#include <numeric>

#include "bitarchivereader.hpp"

#include <7zip/PropID.h>

using namespace bit7z;

BitArchiveReader::BitArchiveReader( const Bit7zLibrary& lib,
                                    const tstring& in_archive,
                                    const BitInFormat& format,
                                    const tstring& password )
    : BitAbstractArchiveOpener( lib, format, password ), BitInputArchive( *this, in_archive ) {}

BitArchiveReader::BitArchiveReader( const Bit7zLibrary& lib,
                                    const std::vector< byte_t >& in_archive,
                                    const BitInFormat& format,
                                    const tstring& password )
    : BitAbstractArchiveOpener( lib, format, password ), BitInputArchive( *this, in_archive ) {}

BitArchiveReader::BitArchiveReader( const Bit7zLibrary& lib,
                                    std::istream& in_archive,
                                    const BitInFormat& format,
                                    const tstring& password )
    : BitAbstractArchiveOpener( lib, format, password ), BitInputArchive( *this, in_archive ) {}

auto BitArchiveReader::archiveProperties() const -> map< BitProperty, BitPropVariant > {
    map< BitProperty, BitPropVariant > result;
    for ( uint32_t i = kpidNoProperty; i <= kpidCopyLink; ++i ) {
        // Yeah, I know, I cast property twice (here and in archiveProperty), but the code is easier to read!
        const auto property = static_cast< BitProperty >( i );
        const BitPropVariant property_value = archiveProperty( property );
        if ( !property_value.isEmpty() ) {
            result[ property ] = property_value;
        }
    }
    return result;
}

auto BitArchiveReader::items() const -> vector< BitArchiveItemInfo > {
    vector< BitArchiveItemInfo > result;
    for ( uint32_t i = 0; i < itemsCount(); ++i ) {
        BitArchiveItemInfo item( i );
        for ( uint32_t j = kpidNoProperty; j <= kpidCopyLink; ++j ) {
            // Yeah, I know, I cast property twice (here and in itemProperty), but the code is easier to read!
            const auto property = static_cast< BitProperty >( j );
            const auto property_value = itemProperty( i, property );
            if ( !property_value.isEmpty() ) {
                item.setProperty( property, property_value );
            }
        }
        result.push_back( item );
    }
    return result;
}

auto BitArchiveReader::foldersCount() const -> uint32_t {
    return std::count_if( cbegin(), cend(), []( const BitArchiveItem& item ) {
        return item.isDir();
    } );
}

auto BitArchiveReader::filesCount() const -> uint32_t {
    return itemsCount() - foldersCount(); // I'm lazy :)
}

auto BitArchiveReader::size() const -> uint64_t {
    return std::accumulate( cbegin(), cend(), 0ull, []( uint64_t accumulator, const BitArchiveItem& item ) {
        return item.isDir() ? accumulator : accumulator + item.size();
    } );
}

auto BitArchiveReader::packSize() const -> uint64_t {
    return std::accumulate( cbegin(), cend(), 0ull, []( uint64_t accumulator, const BitArchiveItem& item ) {
        return item.isDir() ? accumulator : accumulator + item.packSize();
    } );
}

auto BitArchiveReader::hasEncryptedItems() const -> bool {
    /* Note: simple encryption (i.e., not including the archive headers) can be detected only reading
     *       the properties of the files in the archive, so we search for any encrypted file inside the archive! */
    return std::any_of( cbegin(), cend(), []( const BitArchiveItem& item ) {
        return !item.isDir() && item.isEncrypted();
    } );
}

auto BitArchiveReader::isMultiVolume() const -> bool {
    if ( extractionFormat() == BitFormat::Split ) {
        return true;
    }
    const BitPropVariant is_multi_volume = archiveProperty( BitProperty::IsVolume );
    return is_multi_volume.isBool() && is_multi_volume.getBool();
}

auto BitArchiveReader::isSolid() const -> bool {
    const BitPropVariant is_solid = archiveProperty( BitProperty::Solid );
    return is_solid.isBool() && is_solid.getBool();
}

auto BitArchiveReader::volumesCount() const -> uint32_t {
    const BitPropVariant volumes_count = archiveProperty( BitProperty::NumVolumes );
    return volumes_count.isEmpty() ? 1 : volumes_count.getUInt32();
}