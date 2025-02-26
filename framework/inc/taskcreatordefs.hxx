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

#pragma once

#include <rtl/ustring.hxx>

/// [XFrame] if it's set, it will be used as parent frame for the new created frame.
inline constexpr OUStringLiteral ARGUMENT_PARENTFRAME = u"ParentFrame"; // XFrame

/** [OUString] if it's not a special name (beginning with "_" ... which are not allowed here!)
               it will be set as the API name of the new created frame.
 */
inline constexpr OUStringLiteral ARGUMENT_FRAMENAME = u"FrameName"; // OUString

/// [bool] If it's set to true we will make the new created frame visible.
inline constexpr OUStringLiteral ARGUMENT_MAKEVISIBLE = u"MakeVisible"; // bool

/** [bool] If not "ContainerWindow" property is set it force creation of a
               top level window as new container window.
 */
inline constexpr OUStringLiteral ARGUMENT_CREATETOPWINDOW = u"CreateTopWindow"; // bool

/// [Rectangle] Place the new created frame on this place and resize the container window.
inline constexpr OUStringLiteral ARGUMENT_POSSIZE = u"PosSize"; // Rectangle

/// [XWindow] an outside created window, used as container window of the new created frame.
inline constexpr OUStringLiteral ARGUMENT_CONTAINERWINDOW = u"ContainerWindow"; // XWindow

/** [bool] enable/disable special mode, where this frame will be part of
               the persistent window state feature suitable for any office module window
 */
inline constexpr OUStringLiteral ARGUMENT_SUPPORTPERSISTENTWINDOWSTATE
    = u"SupportPersistentWindowState"; // bool

/** [bool] enable/disable special mode, where the title bar of our
               the new created frame will be updated automatically.
               Default = ON !
 */
inline constexpr OUStringLiteral ARGUMENT_ENABLE_TITLEBARUPDATE = u"EnableTitleBarUpdate"; // bool

/** [bool] enable/disable if the frame is explicitly requested to be hidden
               Default = OFF !
 */
inline constexpr OUStringLiteral ARGUMENT_HIDDENFORCONVERSION = u"HiddenForConversion"; // bool

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
