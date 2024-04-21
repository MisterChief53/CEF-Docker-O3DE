// Copyright (c) 2023 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the CEF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//
// $hash=e5131e9cf438a098db3ceec47c2a666f703b6d10$
//

#ifndef CEF_LIBCEF_DLL_CTOCPP_STREAM_WRITER_CTOCPP_H_
#define CEF_LIBCEF_DLL_CTOCPP_STREAM_WRITER_CTOCPP_H_
#pragma once

#if !defined(WRAPPING_CEF_SHARED)
#error This file can be included wrapper-side only
#endif

#include "include/capi/cef_stream_capi.h"
#include "include/cef_stream.h"
#include "libcef_dll/ctocpp/ctocpp_ref_counted.h"

// Wrap a C structure with a C++ class.
// This class may be instantiated and accessed wrapper-side only.
class CefStreamWriterCToCpp : public CefCToCppRefCounted<CefStreamWriterCToCpp,
                                                         CefStreamWriter,
                                                         cef_stream_writer_t> {
 public:
  CefStreamWriterCToCpp();
  virtual ~CefStreamWriterCToCpp();

  // CefStreamWriter methods.
  size_t Write(const void* ptr, size_t size, size_t n) override;
  int Seek(int64_t offset, int whence) override;
  int64_t Tell() override;
  int Flush() override;
  bool MayBlock() override;
};

#endif  // CEF_LIBCEF_DLL_CTOCPP_STREAM_WRITER_CTOCPP_H_