/* Copyright (C) 2025 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * dispatch.hpp - Dispatcher for Standard Device Requests
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * https://opensource.org/licenses/MIT
 */

#pragma once
#include <usbplusplus/usbplusplus.hpp>
#if __cplusplus < 201703L
#error "Dispatcher requires c++17 or higher"
#endif

namespace usbplusplus {
namespace detail {
inline constexpr RequestType standard_device2host(Recipient_t recipient) {
    return RequestType(DataTransferDirection_t::Device_to_Host, RequestType_t::Standard, recipient);
}
inline constexpr RequestType standard_host2device(Recipient_t recipient) {
    return RequestType(DataTransferDirection_t::Host_to_device, RequestType_t::Standard, recipient);
}
struct any_host2device_recipient {
    constexpr bool operator==(RequestType request_type) const {
        return
            request_type == detail::standard_host2device(Recipient_t::Device) ||
            request_type == detail::standard_host2device(Recipient_t::Interface) ||
            request_type == detail::standard_host2device(Recipient_t::Endpoint);
    }
};

inline constexpr bool operator==(RequestType request_type, any_host2device_recipient any_recipient) {
    return any_recipient == request_type;
}

struct any_device2host_recipient {
    constexpr bool operator==(RequestType request_type) const {
        return
            request_type == detail::standard_device2host(Recipient_t::Device) ||
            request_type == detail::standard_device2host(Recipient_t::Interface) ||
            request_type == detail::standard_device2host(Recipient_t::Endpoint);
    }
};

inline constexpr bool operator==(RequestType request_type, any_device2host_recipient any_recipient) {
    return any_recipient == request_type;
}

}
namespace dispatch {
using usb1::RequestCode;

// Table 9-3. gives possible combinations of bmRequestType an bRequestType for Standard Device Requests
// Struct when implements those combinations in StandardDeviceRequest so that only one or two parameters
// are sufficient to express the intent

template<auto ... Params>
struct when;

template<>
struct when<RequestCode::CLEAR_FEATURE> {
    constexpr bool operator==(StandardDeviceRequest request) const {
        return request.bRequest == RequestCode::CLEAR_FEATURE &&
               request.bmRequestType == detail::any_host2device_recipient{};
    }
};

template<auto Recipient>
struct when<RequestCode::CLEAR_FEATURE, Recipient> {
    constexpr bool operator==(StandardDeviceRequest request) const {
        return request.bRequest == RequestCode::CLEAR_FEATURE &&
               request.bmRequestType == detail::standard_host2device(Recipient);
    }
};

template<>
struct when<RequestCode::GET_CONFIGURATION> {
    constexpr bool operator==(StandardDeviceRequest request) const {
        return request.bRequest == RequestCode::GET_CONFIGURATION &&
               request.bmRequestType == detail::standard_device2host(Recipient_t::Device);
    }
};

template<>
struct when<RequestCode::GET_DESCRIPTOR>{
    constexpr bool operator==(StandardDeviceRequest request) const {
        return request.bRequest == RequestCode::GET_DESCRIPTOR &&
               request.bmRequestType == detail::standard_device2host(Recipient_t::Device);
    }
};

template<>
struct when<RequestCode::GET_INTERFACE>{
    constexpr bool operator==(StandardDeviceRequest request) const {
        return request.bRequest == RequestCode::GET_INTERFACE &&
               request.bmRequestType == detail::standard_device2host(Recipient_t::Interface);
    }
};

template<>
struct when<RequestCode::GET_STATUS>{
    constexpr bool operator==(StandardDeviceRequest request) const {
        return request.bRequest == RequestCode::GET_STATUS &&
               request.bmRequestType == detail::any_device2host_recipient{};
    }
};

template<auto Recipient>
struct when<RequestCode::GET_STATUS, Recipient>{
    constexpr bool operator==(StandardDeviceRequest request) const {
        return request.bRequest == RequestCode::GET_STATUS &&
               request.bmRequestType == detail::standard_device2host(Recipient);
    }
};

template<>
struct when<RequestCode::SET_ADDRESS>{
    constexpr bool operator==(StandardDeviceRequest request) const {
        return request.bRequest == RequestCode::SET_ADDRESS &&
               request.bmRequestType == detail::standard_host2device(Recipient_t::Device);
    }
};

template<>
struct when<RequestCode::SET_CONFIGURATION>{
    constexpr bool operator==(StandardDeviceRequest request) const {
        return request.bRequest == RequestCode::SET_CONFIGURATION &&
               request.bmRequestType == detail::standard_host2device(Recipient_t::Device);
    }
};

template<>
struct when<RequestCode::SET_DESCRIPTOR>{
    constexpr bool operator==(StandardDeviceRequest request) const {
        return request.bRequest == RequestCode::SET_DESCRIPTOR &&
               request.bmRequestType == detail::standard_host2device(Recipient_t::Device);
    }
};

template<>
struct when<RequestCode::SET_FEATURE>{
    constexpr bool operator==(StandardDeviceRequest request) const {
        return request.bRequest == RequestCode::SET_FEATURE &&
               request.bmRequestType == detail::any_host2device_recipient{};
    }
};

template<auto Recipient>
struct when<RequestCode::SET_FEATURE, Recipient>{
    constexpr bool operator==(StandardDeviceRequest request) const {
        return request.bRequest == RequestCode::SET_FEATURE &&
               request.bmRequestType == detail::standard_host2device(Recipient);
    }
};

template<>
struct when<RequestCode::SET_INTERFACE>{
    constexpr bool operator==(StandardDeviceRequest request) const {
        return request.bRequest == RequestCode::SET_INTERFACE && (
               request.bmRequestType == detail::standard_host2device(Recipient_t::Interface));
    }
};

template<>
struct when<RequestCode::SYNCH_FRAME>{
    constexpr bool operator==(StandardDeviceRequest request) const {
        return request.bRequest == RequestCode::SYNCH_FRAME && (
               request.bmRequestType == detail::standard_device2host(Recipient_t::Endpoint));
    }
};

template<auto DescriptorType>
struct when<DescriptorType>{
    constexpr bool operator==(StandardDeviceRequest request) const {
        return request.descriptor_type() == DescriptorType &&
                request == when<RequestCode::GET_DESCRIPTOR>{};
    }
};

// Dispatches request, matching When conditions to Function
// Function is expected to return false only if the request is not intended for handling in its scope
// Otherwise It should return true, regardless of its execution success
template<auto Function, auto When>
struct to {
    template<typename Request, typename ... Params>
    bool operator()(Request request, Params&& ... params) const {
        if (When == request) {
            return Function(request, params...);
        }
        return false;
    }
};

// Dispatches request to one of the Items
template <typename ... Item>
struct dispatcher {
    template<typename Request, typename ... Params>
    auto operator()(Request request, Params&& ... params) const {
        return (Item{}(request, params...) || ...);
    }
};

} // namespace dispatch
} // namespace usbplusplus
