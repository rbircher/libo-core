# -*- Mode: makefile-gmake; tab-width: 4; indent-tabs-mode: t -*-
#*************************************************************************
#
# This file is part of the LibreOffice project.
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
#*************************************************************************

$(eval $(call gb_CppunitTest_CppunitTest,sw_apitests))

$(eval $(call gb_CppunitTest_use_external,sw_apitests,boost_headers))

$(eval $(call gb_CppunitTest_use_common_precompiled_header,sw_apitests))

$(eval $(call gb_CppunitTest_add_exception_objects,sw_apitests, \
    sw/qa/api/SwXBodyText \
    sw/qa/api/SwXBookmark \
    sw/qa/api/SwXDocumentIndex \
    sw/qa/api/SwXDocumentSettings \
    sw/qa/api/SwXFootnote \
    sw/qa/api/SwXFootnoteProperties \
    sw/qa/api/SwXFootnoteText \
    sw/qa/api/SwXFootnotes \
    sw/qa/api/SwXHeadFootText \
    sw/qa/api/SwXTextFrame \
    sw/qa/api/SwXTextField \
    sw/qa/api/SwXTextTable \
))

$(eval $(call gb_CppunitTest_use_libraries,sw_apitests, \
    basegfx \
    comphelper \
    cppu \
    cppuhelper \
    drawinglayer \
    docmodel \
    editeng \
    for \
    forui \
    i18nlangtag \
    msfilter \
    oox \
    sal \
    salhelper \
    sax \
    sb \
    sw \
    sfx \
    sot \
    svl \
    svt \
    svx \
    svxcore \
    subsequenttest \
    test \
    tl \
    tk \
    ucbhelper \
    unotest \
    utl \
    $(call gb_Helper_optional,SCRIPTING, \
        vbahelper) \
    vcl \
    xo \
))

$(eval $(call gb_CppunitTest_set_include,sw_apitests,\
    -I$(SRCDIR)/sw/inc \
    $$(INCLUDE) \
))

$(eval $(call gb_CppunitTest_use_api,sw_apitests,\
    offapi \
    udkapi \
))

$(eval $(call gb_CppunitTest_use_ure,sw_apitests))
$(eval $(call gb_CppunitTest_use_vcl,sw_apitests))
$(eval $(call gb_CppunitTest_use_configuration,sw_apitests))

$(eval $(call gb_CppunitTest_use_rdb,sw_apitests,services))

$(eval $(call gb_CppunitTest_use_uiconfigs,sw_apitests, \
    svx \
))

# vim: set noet sw=4 ts=4:
