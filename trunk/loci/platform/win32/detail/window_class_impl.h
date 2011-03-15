#ifndef LOCI_PLATFORM_WIN32_DETAIL_WINDOW_CLASS_IMPL_H_
#define LOCI_PLATFORM_WIN32_DETAIL_WINDOW_CLASS_IMPL_H_

/**
 * Pimpl implementation class for window_class's.
 * Defines data members for window_class's and any functions that require accessing
 * those variables more than once to ensure only one indirection is ever paid.
 *
 * @file   window_class_impl.h
 * @author David Gill
 * @date   21/08/2009
 */

#include <stdexcept>

#include "loci/platform/win32/windows_common.h"
#include "loci/platform/win32/window_class.h"

namespace loci {
namespace platform {
namespace win32
{
    class window_class::window_class_impl
    {
    public:
        window_class_impl(::ATOM atom, window_class::module_type hinstance)
            : atom(static_cast<window_class::handle>(MAKEINTATOM(atom)))
            , hinstance(hinstance)
        {
            if (!atom)
            {
                throw std::runtime_error(
                    "window_class_impl error: Registration failure, the "
                    "received atom was null.");
            }
        }

        ~window_class_impl()
        {
            ::UnregisterClass(atom, hinstance);
        }

        window_class::handle atom;
        window_class::module_type hinstance;
    };
} // namespace win32
} // namespace platform
} // namespace loci


#endif // LOCI_PLATFORM_WIN32_DETAIL_WINDOW_CLASS_IMPL_H_
