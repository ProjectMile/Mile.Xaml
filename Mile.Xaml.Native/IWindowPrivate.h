/*
 * PROJECT:   Mouri Internal Library Essentials
 * FILE:      IWindowPrivate.h
 * PURPOSE:   Definition for IWindowPrivate
 *
 * LICENSE:   The MIT License
 *
 * DEVELOPER: MouriNaruto (KurikoMouri@outlook.jp)
 */

#pragma once

#ifndef IWINDOWPRIVATE
#define IWINDOWPRIVATE

#include <inspectable.h>

// Enable XamlHost transparency by using a undocumented API.
// Reference: https://github.com/ADeltaX/IWindowPrivate

MIDL_INTERFACE("06636C29-5A17-458D-8EA2-2422D997A922")
IWindowPrivate : public IInspectable
{
public:
    virtual HRESULT STDMETHODCALLTYPE get_TransparentBackground(
        _Out_ boolean* value) = 0;

    virtual HRESULT STDMETHODCALLTYPE put_TransparentBackground(
        _In_ boolean value) = 0;
};

#endif // !IWINDOWPRIVATE
