#pragma once

#include "i2c.h"
#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_i2c.h"
#include <cstring>
#include <stdio.h>

#define USE_FLASH 0

#if USE_FLASH

namespace impl { // symply stable tuple
    #pragma pack(push)
template <class... Ts>
struct tuple;

template <class Head, class... Tail>
struct tuple<Head, Tail...> {
    Head head;
    tuple<Tail...> tail;

    tuple(const Head& head, const Tail&... tail)
        : head {head}
        , tail {tail...} { }

    template <class... Ts>
    auto operator=(const tuple<Ts...>& t) {
        printf("%s\n", __FUNCTION__);
        head = t.head;
        tail = t.tail;
        return (*this);
    }
};

template <>
struct tuple<> { };

inline auto operator==(const tuple<>&, const tuple<>&) {
    return true;
};

template <class Head1, class... Tail1, class Head2, class... Tail2>
auto operator==(const tuple<Head1, Tail1...>& t1, const tuple<Head2, Tail2...>& t2) {
    return sizeof...(Tail1) == sizeof...(Tail2) && t1.head == t2.head && t1.tail == t2.tail;
};
    #pragma pack(pop)
} // namespace impl

#endif

class Flash {
    const uint32_t page;
    uint8_t* const pageAddress_;
    mutable HAL_StatusTypeDef flashstatus {HAL_OK};

#if USE_FLASH
    struct Unlocker {
        const Flash* const f;
        Unlocker(const Flash* f)
            : f {f} {
            printf("%s\n", __FUNCTION__);
            f->flashstatus = HAL_FLASH_Unlock();
        }
        ~Unlocker() {
            printf("%s\n", __FUNCTION__);
            f->flashstatus = HAL_FLASH_Lock();
        }
    };

#endif

    struct Proxy {

        static constexpr uint8_t I2C_DEVICE_ADDRESS = 0x50;

        //        const Flash* const f;
        const uint32_t i;

        operator uint8_t() const {
            uint8_t val{};
            uint16_t address = (I2C_DEVICE_ADDRESS | ((i >> 8) & 0x7)) << 1;
            auto status = HAL_I2C_Mem_Read(&hi2c1, address, i & 0xFF, I2C_MEMADD_SIZE_8BIT, &val, 1, 100);
            return val;
        }

        uint8_t operator=(uint8_t val) {
            uint16_t address = (I2C_DEVICE_ADDRESS | ((i >> 8) & 0x7)) << 1;
            auto status = HAL_I2C_Mem_Write(&hi2c1, address, i & 0xFF, I2C_MEMADD_SIZE_8BIT, &val, 1, 100);
            HAL_Delay(6);
            return val;
        }

#if USE_FLASH
        template <typename T>
        operator T() const {
            return ((T*)f->pageAddress_)[i];
        }
        template <typename T>
        T operator=(T val) {
            do {
                uint8_t buf[PAGESIZE];
                memcpy(buf, f->pageAddress_, PAGESIZE);

                T* buf_t = ((T*)buf);
                bool erace {(buf_t[i] & val) != val};
                buf_t[i] = val;

                if (erace) {
                    Unlocker unlocker {f};
                    if (f->flashstatus != HAL_OK)
                        break;
                    f->erasePage();
                    if (f->flashstatus != HAL_OK)
                        break;
                }
                Unlocker unlocker {f};
                if (f->flashstatus != HAL_OK)
                    break;
                for (uint32_t err {}, ctr {}; ctr < (PAGESIZE / 8); ctr += 8) {
                    if (erace || *(uint64_t*)(f->pageAddress_ + ctr) != *(uint64_t*)(buf + ctr)) {
                        f->flashstatus = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, uint32_t(f->pageAddress_ + ctr), *reinterpret_cast<uint64_t*>(buf + ctr));
                        if (f->flashstatus != HAL_OK)
                            err = HAL_FLASH_GetError();
                    }
                }
            } while (0);

            return ((T*)f->pageAddress_)[i];
        }
#endif
    };

public:
    Flash(uint32_t page)
        : page {page}
        , pageAddress_ {(uint8_t* const)(FLASH_BASE + FLASH_PAGE_SIZE * page)} { }

    Proxy operator[](uint32_t i) const { return {/*this,*/ i}; }

#if USE_FLASH
    HAL_StatusTypeDef erasePage() const {
        Unlocker unlocker {this};
        FLASH_EraseInitTypeDef s_eraseinit {
            .TypeErase = FLASH_TYPEERASE_PAGES,
            //            .Banks = 1,
            .Page = page,
            .NbPages = 1,
        };
        uint32_t page_error {};
        return HAL_FLASHEx_Erase(&s_eraseinit, &page_error);
    }

    template <class... Ts>
    bool read(Ts&... val) {
        using namespace impl;
        tuple<Ts&...>(val...) = *reinterpret_cast<tuple<Ts...>*>(pageAddress_);
        return true;
    }

    template <class... Ts>
    bool write(const Ts&... val) {
        Unlocker unlocker {this};
        if (flashstatus != HAL_OK)
            return {};

        using namespace impl;

        if (tuple<const Ts&...>(val...) != *reinterpret_cast<tuple<Ts...>*>(pageAddress_))
            if (!erasePage())
                return {};

        constexpr size_t size {sizeof(tuple<Ts...>) / 2 + 1};

        volatile union {
            tuple<Ts...> tu;
            uint16_t buf[size];
        } u {.tu = {val...}};

        for (uint32_t ctr {}; auto val : u.buf) {
            flashstatus = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, pageAddress_ + ctr, val); // half-word (16-bit)
            if (flashstatus != HAL_OK)
                return {};
            ctr += sizeof(uint64_t);
        }
        printf("head %x\n", reinterpret_cast<tuple<Ts...>*>(pageAddress_)->head);
        return true;
    }

#endif
};
