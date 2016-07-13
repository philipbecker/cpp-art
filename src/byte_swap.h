#ifndef ART_BYTE_SWAP_H
#define ART_BYTE_SWAP_H


namespace art
{
/**

    template<typename _Tp>
    struct byte_swap;

    template<>
    struct byte_swap<uint16_t> {
        uint16_t operator()(uint16_t __val) const noexcept {
            return (__val << 8) | (__val >> 8);
        }
    };

    template<>
    struct byte_swap<int16_t> {
        int16_t operator()(int16_t __val) const noexcept {
            return (__val << 8) | ((__val >> 8) & 0xFF);
        }
    };

    template<>
    struct byte_swap<uint32_t> {
        uint32_t operator()(uint32_t __val) const noexcept {
            __val = ((__val << 8) & 0xFF00FF00) | ((__val >> 8) & 0xFF00FF);
            return (__val << 16) | (__val >> 16);
        }
    };

    template<>
    struct byte_swap<int32_t> {
        int32_t operator()(int32_t __val) const noexcept {
            __val = ((__val << 8) & 0xFF00FF00) | ((__val >> 8) & 0xFF00FF);
            return (__val << 16) | ((__val >> 16) & 0xFFFF);
        }
    };

    template<>
    struct byte_swap<uint64_t> {
        uint64_t operator()(uint64_t __val) const noexcept {
            __val = ((__val << 8) & 0xFF00FF00FF00FF00ULL) | ((__val >> 8) & 0x00FF00FF00FF00FFULL);
            __val = ((__val << 16) & 0xFFFF0000FFFF0000ULL) | ((__val >> 16) & 0x0000FFFF0000FFFFULL);
            return (__val << 32) | (__val >> 32);
        }
    };

    template<>
    struct byte_swap<int64_t> {
        int64_t operator()(int64_t __val) const noexcept {
            __val = ((__val << 8) & 0xFF00FF00FF00FF00ULL) | ((__val >> 8) & 0x00FF00FF00FF00FFULL);
            __val = ((__val << 16) & 0xFFFF0000FFFF0000ULL) | ((__val >> 16) & 0x0000FFFF0000FFFFULL);
            return (__val << 32) | ((__val >> 32) & 0xFFFFFFFFULL);
        }
    };
*/
}
#endif //ART_BYTE_SWAP_H
