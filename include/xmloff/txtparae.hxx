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

#ifndef INCLUDED_XMLOFF_TXTPARAE_HXX
#define INCLUDED_XMLOFF_TXTPARAE_HXX

#include <sal/config.h>
#include <rtl/ref.hxx>
#include <xmloff/dllapi.h>
#include <rtl/ustring.hxx>
#include <com/sun/star/uno/Reference.h>
#include <xmloff/maptype.hxx>
#include <xmloff/styleexp.hxx>
#include <xmloff/xmltoken.hxx>
#include <xmloff/SinglePropertySetInfoCache.hxx>
#include <xmloff/XMLTextListAutoStylePool.hxx>
#include <o3tl/sorted_vector.hxx>
#include <o3tl/span.hxx>
#include <memory>
#include <vector>

class XMLTextListsHelper;
class SvXMLExport;
class SvXMLAutoStylePoolP;
class XMLTextFieldExport;
class XMLTextNumRuleInfo;
class XMLSectionExport;
class XMLIndexMarkExport;
class XMLRedlineExport;
class MultiPropertySetHelper;
enum class XMLShapeExportFlags;
class SvXMLExportPropertyMapper;

namespace com::sun::star
{
    namespace beans { class XPropertySet; class XPropertyState;
                      class XPropertySetInfo; }
    namespace container { class XEnumeration; class XIndexAccess; class XNameReplace; }
    namespace drawing { class XShape; }
    namespace text { class XTextContent; class XTextRange; class XText;
                     class XFootnote; class XTextFrame; class XTextSection;
                     class XTextField; }
}

namespace xmloff
{
    class OFormLayerXMLExport;
    class BoundFrameSets;

}

namespace basegfx
{
    class B2DPoint;
}

enum class TextPNS
{
    ODF,
    EXTENSION
};

class XMLOFF_DLLPUBLIC XMLTextParagraphExport : public XMLStyleExport
{
    struct Impl;
    std::unique_ptr<Impl> m_xImpl;

//  SvXMLExport& rExport;
    SvXMLAutoStylePoolP& m_rAutoStylePool;
    rtl::Reference < SvXMLExportPropertyMapper > m_xParaPropMapper;
    rtl::Reference < SvXMLExportPropertyMapper > m_xTextPropMapper;
    rtl::Reference < SvXMLExportPropertyMapper > m_xFramePropMapper;
    rtl::Reference < SvXMLExportPropertyMapper > m_xAutoFramePropMapper;
    rtl::Reference < SvXMLExportPropertyMapper > m_xSectionPropMapper;
    rtl::Reference < SvXMLExportPropertyMapper > m_xRubyPropMapper;

    const ::std::unique_ptr< ::xmloff::BoundFrameSets > m_pBoundFrameSets;
    std::unique_ptr<XMLTextFieldExport>          m_pFieldExport;
    std::vector<OUString>                        maListElements;
    XMLTextListAutoStylePool                     maListAutoPool;
    std::unique_ptr<XMLSectionExport>            m_pSectionExport;
    std::unique_ptr<XMLIndexMarkExport>          m_pIndexMarkExport;

    /// may be NULL (if no redlines should be exported; e.g. in block mode)
    std::unique_ptr<XMLRedlineExport> m_pRedlineExport;

    bool                        m_bProgress;

    bool                        m_bBlock;

    // keep track of open rubies
    OUString                    m_sOpenRubyText;
    OUString                    m_sOpenRubyCharStyle;
    bool                        m_bOpenRuby;

    XMLTextListsHelper* mpTextListsHelper;
    ::std::vector< std::unique_ptr<XMLTextListsHelper> > maTextListsHelperStack;

    struct DocumentListNodes;
    std::unique_ptr<DocumentListNodes> mpDocumentListNodes;

    o3tl::sorted_vector<css::uno::Reference<css::text::XTextFrame>> maFrameRecurseGuard;
    o3tl::sorted_vector<css::uno::Reference<css::drawing::XShape>> maShapeRecurseGuard;

    bool mbCollected;

    enum class FrameType { Text, Graphic, Embedded, Shape };
public:

    enum FieldmarkType { NONE, TEXT, CHECK }; // Used for simulating fieldmarks in OpenDocument 1.n Strict (for n <= 2). CHECK currently ignored.


    void exportTextRangeSpan(
            const css::uno::Reference< css::text::XTextRange > & rTextRange,
            css::uno::Reference< css::beans::XPropertySet > const & xPropSet,
            css::uno::Reference < css::beans::XPropertySetInfo > & xPropSetInfo,
            const bool bIsUICharStyle,
            const bool bHasAutoStyle,
            const OUString& sStyle,
            bool& rPrevCharIsSpace,
            FieldmarkType& openFieldMark);

private:

    // Implement Title/Description Elements UI (#i73249#)
    static constexpr OUStringLiteral gsAnchorCharStyleName = u"AnchorCharStyleName";
    static constexpr OUStringLiteral gsBeginNotice = u"BeginNotice";
    static constexpr OUStringLiteral gsCategory = u"Category";
    static constexpr OUStringLiteral gsCharStyleName = u"CharStyleName";
    static constexpr OUStringLiteral gsCharStyleNames = u"CharStyleNames";
    static constexpr OUStringLiteral gsEndNotice = u"EndNotice";
    static constexpr OUStringLiteral gsFootnote = u"Footnote";
    static constexpr OUStringLiteral gsFootnoteCounting = u"FootnoteCounting";
    static constexpr OUStringLiteral gsNumberingType = u"NumberingType";
    static constexpr OUStringLiteral gsPageDescName = u"PageDescName";
    static constexpr OUStringLiteral gsPageStyleName = u"PageStyleName";
    static constexpr OUStringLiteral gsParaStyleName = u"ParaStyleName";
    static constexpr OUStringLiteral gsPositionEndOfDoc = u"PositionEndOfDoc";
    static constexpr OUStringLiteral gsPrefix = u"Prefix";
    static constexpr OUStringLiteral gsReferenceId = u"ReferenceId";
    static constexpr OUStringLiteral gsStartAt = u"StartAt";
    static constexpr OUStringLiteral gsSuffix = u"Suffix";
    static constexpr OUStringLiteral gsTextEndnoteService = u"com.sun.star.text.Endnote";
    static constexpr OUStringLiteral gsTextSection = u"TextSection";

protected:
    static constexpr OUStringLiteral gsFrameStyleName = u"FrameStyleName";
    SinglePropertySetInfoCache m_aCharStyleNamesPropInfoCache;

    SvXMLAutoStylePoolP& GetAutoStylePool() { return m_rAutoStylePool; }
    const SvXMLAutoStylePoolP& GetAutoStylePool() const { return m_rAutoStylePool; }

public:
    const rtl::Reference < SvXMLExportPropertyMapper >& GetParaPropMapper() const
    {
        return m_xParaPropMapper;
    }

    const rtl::Reference < SvXMLExportPropertyMapper >& GetTextPropMapper() const
    {
        return m_xTextPropMapper;
    }

    const rtl::Reference < SvXMLExportPropertyMapper >& GetAutoFramePropMapper() const
    {
        return m_xAutoFramePropMapper;
    }
    const rtl::Reference < SvXMLExportPropertyMapper >& GetSectionPropMapper() const
    {
        return m_xSectionPropMapper;
    }
    const rtl::Reference < SvXMLExportPropertyMapper >& GetRubyPropMapper() const
    {
        return m_xRubyPropMapper;
    }

    OUString FindTextStyle(
            const css::uno::Reference< css::beans::XPropertySet > & rPropSet,
            bool& rbHasCharStyle,
            bool& rbHasAutoStyle,
            const XMLPropertyState** pAddState = nullptr) const;

    void exportTextRangeEnumeration(
        const css::uno::Reference< css::container::XEnumeration > & rRangeEnum,
        bool bAutoStyles, bool bProgress, bool & rPrevCharIsSpace);

protected:

    XMLShapeExportFlags addTextFrameAttributes(
        const css::uno::Reference< css::beans::XPropertySet >& rPropSet,
        bool bShape,
        basegfx::B2DPoint* pCenter = nullptr,
        OUString *pMinHeightValue = nullptr,
        OUString *pMinWidthValue = nullptr );

    virtual void exportStyleAttributes(
        const css::uno::Reference< css::style::XStyle > & rStyle ) override;

    void exportPageFrames( bool bProgress );
    void exportFrameFrames( bool bAutoStyles, bool bProgress,
            const css::uno::Reference< css::text::XTextFrame >& rParentTxtFrame );

    void exportNumStyles( bool bUsed );

    void exportText(
        const css::uno::Reference <
            css::text::XText > & rText,
        bool bAutoStyles, bool bProgress, bool bExportParagraph, TextPNS eExtensionNS = TextPNS::ODF );

    void exportText(
        const css::uno::Reference< css::text::XText > & rText,
        const css::uno::Reference< css::text::XTextSection > & rBaseSection,
        bool bAutoStyles, bool bProgress, bool bExportParagraph );

    void exportTextContentEnumeration(
        const css::uno::Reference< css::container::XEnumeration > & rContentEnum,
        bool bAutoStyles,
        const css::uno::Reference< css::text::XTextSection > & rBaseSection,
        bool bProgress,
        bool bExportParagraph = true,
        const css::uno::Reference< css::beans::XPropertySet > *pRangePropSet = nullptr,
        TextPNS eExtensionNS = TextPNS::ODF);
    void exportParagraph(
        const css::uno::Reference< css::text::XTextContent > & rTextContent,
        bool bAutoStyles, bool bProgress,
        bool bExportParagraph,
        MultiPropertySetHelper& rPropSetHelper,
        TextPNS eExtensionNS);

    virtual void exportTable(
        const css::uno::Reference< css::text::XTextContent > & rTextContent,
        bool bAutoStyles, bool bProgress );

    void exportTextField(
        const css::uno::Reference< css::text::XTextRange > & rTextRange,
        bool bAutoStyles, bool bProgress, bool * pPrevCharIsSpace);

    void exportTextField(
        const css::uno::Reference< css::text::XTextField> & xTextField,
        const bool bAutoStyles, const bool bProgress,
        const bool bRecursive, bool * pPrevCharIsSpace);

    void exportAnyTextFrame(
        const css::uno::Reference< css::text::XTextContent > & rTextContent,
        FrameType eTxpe,
        bool bAutoStyles, bool bProgress, bool bExportContent,
        const css::uno::Reference< css::beans::XPropertySet > *pRangePropSet );
    void _exportTextFrame(
        const css::uno::Reference< css::beans::XPropertySet > & rPropSet,
        const css::uno::Reference< css::beans::XPropertySetInfo > & rPropSetInfo,
        bool bProgress );
    inline void exportTextFrame(
        const css::uno::Reference< css::text::XTextContent > & rTextContent,
        bool bAutoStyles, bool bProgress, bool bExportContent,
        const css::uno::Reference< css::beans::XPropertySet > *pRangePropSet = nullptr );
    inline void exportShape(
        const css::uno::Reference< css::text::XTextContent > & rTextContent,
        bool bAutoStyles,
        const css::uno::Reference< css::beans::XPropertySet > *pRangePropSet = nullptr  );

    void exportContour(
        const css::uno::Reference< css::beans::XPropertySet > & rPropSet,
        const css::uno::Reference< css::beans::XPropertySetInfo > & rPropSetInfo );
    void _exportTextGraphic(
        const css::uno::Reference< css::beans::XPropertySet > & rPropSet,
        const css::uno::Reference< css::beans::XPropertySetInfo > & rPropSetInfo );
    inline void exportTextGraphic(
        const css::uno::Reference< css::text::XTextContent > & rTextContent,
        bool bAutoStyles,
        const css::uno::Reference< css::beans::XPropertySet > *pRangePropSet = nullptr  );

    virtual void _collectTextEmbeddedAutoStyles(
        const css::uno::Reference< css::beans::XPropertySet > & rPropSet );
    virtual void _exportTextEmbedded(
        const css::uno::Reference< css::beans::XPropertySet > & rPropSet,
        const css::uno::Reference< css::beans::XPropertySetInfo > & rPropSetInfo );
    inline void exportTextEmbedded(
        const css::uno::Reference< css::text::XTextContent > & rTextContent,
        bool bAutoStyles,
        const css::uno::Reference< css::beans::XPropertySet > *pRangePropSet = nullptr  );

    /// export a footnote and styles
    void exportTextFootnote(
        const css::uno::Reference< css::beans::XPropertySet > & rPropSet,
        const OUString& sString,
        bool bAutoStyles, bool bProgress );

    /// helper for exportTextFootnote
    void exportTextFootnoteHelper(
        const css::uno::Reference< css::text::XFootnote > & rPropSet,
        const css::uno::Reference< css::text::XText> & rText,
        const OUString& sString,
        bool bAutoStyles,
        bool bIsEndnote, bool bProgress );

    /// export footnote and endnote configuration elements
    void exportTextFootnoteConfiguration();

    void exportTextFootnoteConfigurationHelper(
        const css::uno::Reference< css::beans::XPropertySet> & rFootnoteSupplier,
        bool bIsEndnote);

    void exportTextMark(
        const css::uno::Reference< css::beans::XPropertySet> & xPropSet,
        const OUString& rProperty,
        const enum ::xmloff::token::XMLTokenEnum pElements[],
        bool bAutoStyles);

    void exportSoftPageBreak();

    void exportTextLineBreak(const css::uno::Reference<css::beans::XPropertySet>& xPropSet);

    void exportTextRange(
        const css::uno::Reference< css::text::XTextRange > & rTextRange,
        bool bAutoStyles,
        bool& rPrevCharWasSpace,
        FieldmarkType& openFieldmarkType );

    void exportListChange( const XMLTextNumRuleInfo& rPrvInfo,
                           const XMLTextNumRuleInfo& rNextInfo );

    /// check if current section or current list has changed;
    /// calls exportListChange as appropriate
    void exportListAndSectionChange(
        css::uno::Reference< css::text::XTextSection > & rOldSection,
        const css::uno::Reference< css::text::XTextSection > & rNewSection,
        const XMLTextNumRuleInfo& rOldList,
        const XMLTextNumRuleInfo& rNewList,
        bool bAutoStyles );

    /// overload for exportListAndSectionChange;
    /// takes new content rather than new section.
    void exportListAndSectionChange(
        css::uno::Reference< css::text::XTextSection > & rOldSection,
        const css::uno::Reference< css::text::XTextContent > & rNewContent,
        const XMLTextNumRuleInfo& rOldList,
        const XMLTextNumRuleInfo& rNewList,
        bool bAutoStyles );
    void exportListAndSectionChange(
        css::uno::Reference< css::text::XTextSection > & rOldSection,
        MultiPropertySetHelper& rPropSetHelper,
        sal_Int16 nTextSectionId,
        const css::uno::Reference< css::text::XTextContent > & rNewContent,
        const XMLTextNumRuleInfo& rOldList,
        const XMLTextNumRuleInfo& rNewList,
        bool bAutoStyles );

    /// export a ruby
    void exportRuby(
        const css::uno::Reference< css::beans::XPropertySet> & rPortionPropSet,
        bool bAutoStyles );

    /// export a text:meta
    void exportMeta(
        const css::uno::Reference< css::beans::XPropertySet> & i_xPortion,
        bool i_bAutoStyles, bool i_isProgress, bool & rPrevCharIsSpace);

    /// Exports a <loext:content-control> element.
    void ExportContentControl(const css::uno::Reference<css::beans::XPropertySet>& xPortion,
                              bool bAutoStyles, bool isProgress, bool& rPrevCharIsSpace);

    bool isAutoStylesCollected() const { return mbCollected; }

    virtual void exportTableAutoStyles();

public:

    XMLTextParagraphExport(
            SvXMLExport& rExp,
               SvXMLAutoStylePoolP & rASP
                          );
    virtual ~XMLTextParagraphExport() override;

    /// add autostyle for specified family
    void Add(
        XmlStyleFamily nFamily,
        MultiPropertySetHelper& rPropSetHelper,
        const css::uno::Reference< css::beans::XPropertySet > & rPropSet );
    void Add(
        XmlStyleFamily nFamily,
        const css::uno::Reference< css::beans::XPropertySet > & rPropSet,
        o3tl::span<const XMLPropertyState> aAddStates = {}, bool bDontSeek = false );

    /// find style name for specified family and parent
    OUString Find(
        XmlStyleFamily nFamily,
        const css::uno::Reference< css::beans::XPropertySet > & rPropSet,
        const OUString& rParent,
        const o3tl::span<const XMLPropertyState> aAddStates = {} ) const;

    static SvXMLExportPropertyMapper *CreateShapeExtPropMapper(
                                                SvXMLExport& rExport );
    static SvXMLExportPropertyMapper *CreateCharExtPropMapper(
                                                SvXMLExport& rExport);
    static SvXMLExportPropertyMapper *CreateParaExtPropMapper(
                                                SvXMLExport& rExport);
    static SvXMLExportPropertyMapper *CreateParaDefaultExtPropMapper(
                                                SvXMLExport& rExport);

    // This methods exports all (or all used) styles
    void exportTextStyles( bool bUsed, bool bProg );

    /// This method exports (text field) declarations etc.
    void exportTextDeclarations();

    /// export the (text field) declarations for a particular XText
    void exportTextDeclarations(
        const css::uno::Reference< css::text::XText > & rText );

    /// export all declarations
    void exportUsedDeclarations();

    /// Export the list of change information (enclosed by <tracked-changes>)
    /// (or the necessary automatic styles)
    void exportTrackedChanges(bool bAutoStyle);

    /// Export the list of change information (enclosed by <tracked-changes>)
    /// (or the necessary automatic styles)
    void exportTrackedChanges(const css::uno::Reference< css::text::XText > & rText,
                              bool bAutoStyle );

    /// Record tracked changes for this particular XText
    /// (empty reference stop recording)
    /// This should be used if tracked changes for e.g. footers are to
    /// be exported separately via the exportTrackedChanges(bool,
    /// Reference<XText>) method.
    void recordTrackedChangesForXText(
        const css::uno::Reference< css::text::XText > & rText );


    /// Stop recording tracked changes.
    /// This is the same as calling recordTrackedChanges(...) with an
    /// empty reference.
    void recordTrackedChangesNoXText();


    // This method exports the given OUString
    void exportCharacterData(
        const OUString& rText,
        bool& rPrevCharWasSpace);

    // This method collects all automatic styles for the given XText
    void collectTextAutoStyles(
        const css::uno::Reference< css::text::XText > & rText,
        bool bIsProgress = false,
        bool bExportParagraph = true )
    {
        exportText( rText, true, bIsProgress, bExportParagraph );
    }

    void collectTextAutoStyles(
        const css::uno::Reference< css::text::XText > & rText,
        const css::uno::Reference< css::text::XTextSection > & rBaseSection,
        bool bIsProgress )
    {
        exportText( rText, rBaseSection, true, bIsProgress, true/*bExportParagraph*/ );
    }

    // It the model implements the xAutoStylesSupplier interface, the automatic
    // styles can exported without iterating over the text portions
    void collectTextAutoStylesOptimized( bool bIsProgress );

    // This method exports all automatic styles that have been collected.
    void exportTextAutoStyles();

    void exportEvents( const css::uno::Reference< css::beans::XPropertySet > & rPropSet );

    // Implement Title/Description Elements UI (#i73249#)
    void exportTitleAndDescription( const css::uno::Reference< css::beans::XPropertySet > & rPropSet,
                                    const css::uno::Reference< css::beans::XPropertySetInfo > & rPropSetInfo );

    // This method exports the given XText
    void exportText(
        const css::uno::Reference< css::text::XText > & rText,
        bool bIsProgress = false,
        bool bExportParagraph = true, TextPNS eExtensionNS = TextPNS::ODF)
    {
        exportText( rText, false, bIsProgress, bExportParagraph, eExtensionNS );
    }

    void exportText(
        const css::uno::Reference< css::text::XText > & rText,
        const css::uno::Reference< css::text::XTextSection > & rBaseSection,
        bool bIsProgress)
    {
        exportText( rText, rBaseSection, false, bIsProgress, true/*bExportParagraph*/ );
    }

    void exportFramesBoundToPage( bool bIsProgress )
    {
        exportPageFrames( bIsProgress );
    }
    inline const XMLTextListAutoStylePool& GetListAutoStylePool() const;

    void SetBlockMode( bool bSet ) { m_bBlock = bSet; }
    bool IsBlockMode() const { return m_bBlock; }


    const rtl::Reference < SvXMLExportPropertyMapper >& GetParagraphPropertyMapper() const
    {
        return m_xParaPropMapper;
    }


    /** exclude form controls which are in mute sections.
     *
     * This method is necessary to prevent the form layer export from exporting
     * control models whose controls are not represented in the document.  To
     * achieve this, this method iterates over all shapes, checks to see if
     * they are control shapes, and if so, whether they should be exported or
     * not. If not, the form layer export will be notified accordingly.
     *
     * The reason this method is located here is that it needs to access the
     * XMLSectionExport, which is only available here.
     */
    void PreventExportOfControlsInMuteSections(
        const css::uno::Reference< css::container::XIndexAccess> & rShapes,
        const rtl::Reference<xmloff::OFormLayerXMLExport>& xFormExport );

    SinglePropertySetInfoCache& GetCharStyleNamesPropInfoCache() { return m_aCharStyleNamesPropInfoCache; }

    void PushNewTextListsHelper();

    void PopTextListsHelper();

private:
    bool ShouldSkipListId(const css::uno::Reference<css::text::XTextContent>& xTextContent);
    bool ExportListId() const;

        XMLTextParagraphExport(XMLTextParagraphExport const &) = delete;

};

inline const XMLTextListAutoStylePool&
    XMLTextParagraphExport::GetListAutoStylePool() const
{
    return maListAutoPool;
}

inline void XMLTextParagraphExport::exportTextFrame(
        const css::uno::Reference< css::text::XTextContent > & rTextContent,
        bool bAutoStyles, bool bIsProgress, bool bExportContent,
        const css::uno::Reference< css::beans::XPropertySet > *pRangePropSet)
{
    exportAnyTextFrame( rTextContent, FrameType::Text, bAutoStyles, bIsProgress,
                        bExportContent, pRangePropSet );
}

inline void XMLTextParagraphExport::exportTextGraphic(
        const css::uno::Reference< css::text::XTextContent > & rTextContent,
        bool bAutoStyles,
        const css::uno::Reference< css::beans::XPropertySet > *pRangePropSet )
{
    exportAnyTextFrame( rTextContent, FrameType::Graphic, bAutoStyles, false,
                        true, pRangePropSet );
}

inline void XMLTextParagraphExport::exportTextEmbedded(
        const css::uno::Reference< css::text::XTextContent > & rTextContent,
        bool bAutoStyles,
        const css::uno::Reference< css::beans::XPropertySet > *pRangePropSet )
{
    exportAnyTextFrame( rTextContent, FrameType::Embedded, bAutoStyles, false,
                        true, pRangePropSet );
}

inline void XMLTextParagraphExport::exportShape(
        const css::uno::Reference< css::text::XTextContent > & rTextContent,
        bool bAutoStyles,
        const css::uno::Reference< css::beans::XPropertySet > *pRangePropSet )
{
    exportAnyTextFrame( rTextContent, FrameType::Shape, bAutoStyles, false,
                        true, pRangePropSet );
}

#endif

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
