/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This file incorporates work covered by the following license notice:
 *
 *   Licensed to the Apache Software Foundation (ASF) under one or more
 *   contributor license agreements. See the NOTICE file distributed
 *   with this work for additional information regarding copyright
 *   ownership. The ASF licenses this file to you under the Apache
 *   License, Version 2.0 (the "License"); you may not use this file
 *   except in compliance with the License. You may obtain a copy of
 *   the License at http://www.apache.org/licenses/LICENSE-2.0 .
 */

#include <tools/color.hxx>
#include <vcl/alpha.hxx>

#include <bitmap/BitmapWriteAccess.hxx>
#include <salinst.hxx>
#include <svdata.hxx>
#include <salbmp.hxx>
#include <sal/log.hxx>

AlphaMask::AlphaMask() = default;

AlphaMask::AlphaMask( const Bitmap& rBitmap ) :
    Bitmap( rBitmap )
{
    if( !rBitmap.IsEmpty() )
        Convert( BmpConversion::N8BitNoConversion );
}

AlphaMask::AlphaMask( const AlphaMask& ) = default;

AlphaMask::AlphaMask( AlphaMask&& ) = default;

AlphaMask::AlphaMask( const Size& rSizePixel, const sal_uInt8* pEraseTransparency )
    : Bitmap(rSizePixel, vcl::PixelFormat::N8_BPP, &Bitmap::GetGreyPalette(256))
{
    if( pEraseTransparency )
        Bitmap::Erase( Color( *pEraseTransparency, *pEraseTransparency, *pEraseTransparency ) );
}

AlphaMask::~AlphaMask() = default;

AlphaMask& AlphaMask::operator=( const Bitmap& rBitmap )
{
    *static_cast<Bitmap*>(this) = rBitmap;

    if( !rBitmap.IsEmpty() )
        Convert( BmpConversion::N8BitNoConversion );

    return *this;
}

const Bitmap& AlphaMask::ImplGetBitmap() const
{
    return *this;
}

Bitmap const & AlphaMask::GetBitmap() const
{
    return ImplGetBitmap();
}

void AlphaMask::Erase( sal_uInt8 cTransparency )
{
    Bitmap::Erase( Color( cTransparency, cTransparency, cTransparency ) );
}

void AlphaMask::BlendWith(const AlphaMask& rOther)
{
    std::shared_ptr<SalBitmap> xImpBmp(ImplGetSVData()->mpDefInst->CreateSalBitmap());
    if (xImpBmp->Create(*ImplGetSalBitmap()) && xImpBmp->AlphaBlendWith(*rOther.ImplGetSalBitmap()))
    {
        ImplSetSalBitmap(xImpBmp);
        return;
    }
    Bitmap::ScopedReadAccess pOtherAcc(const_cast<AlphaMask&>(rOther));
    AlphaScopedWriteAccess pAcc(*this);
    assert (pOtherAcc && pAcc && pOtherAcc->GetBitCount() == 8 && pAcc->GetBitCount() == 8 && "cannot BlendWith this combination");
    if (!(pOtherAcc && pAcc && pOtherAcc->GetBitCount() == 8 && pAcc->GetBitCount() == 8))
    {
        SAL_WARN("vcl", "cannot BlendWith this combination");
        return;
    }

    const tools::Long nHeight = std::min(pOtherAcc->Height(), pAcc->Height());
    const tools::Long nWidth = std::min(pOtherAcc->Width(), pAcc->Width());
    for (tools::Long y = 0; y < nHeight; ++y)
    {
        Scanline scanline = pAcc->GetScanline( y );
        ConstScanline otherScanline = pOtherAcc->GetScanline( y );
        for (tools::Long x = 0; x < nWidth; ++x)
        {
            // Use sal_uInt16 for following multiplication
            const sal_uInt16 nGrey1 = *scanline;
            const sal_uInt16 nGrey2 = *otherScanline;
            *scanline = static_cast<sal_uInt8>(nGrey1 + nGrey2 - nGrey1 * nGrey2 / 255);
            ++scanline;
            ++otherScanline;
        }
    }
}

bool AlphaMask::hasAlpha() const
{
    // no content, no alpha
    if(IsEmpty())
        return false;

    ScopedReadAccess pAcc(const_cast<AlphaMask&>(*this));
    const tools::Long nHeight(pAcc->Height());
    const tools::Long nWidth(pAcc->Width());

    // no content, no alpha
    if(0 == nHeight || 0 == nWidth)
        return false;

    for (tools::Long y = 0; y < nHeight; ++y)
    {
        for (tools::Long x = 0; x < nWidth; ++x)
        {
            if (0 != pAcc->GetColor(y, x).GetRed())
            {
                return true;
            }
        }
    }

    return false;
}

void AlphaMask::ReleaseAccess( BitmapReadAccess* pAccess )
{
    if( pAccess )
    {
        Bitmap::ReleaseAccess( pAccess );
        Convert( BmpConversion::N8BitNoConversion );
    }
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
