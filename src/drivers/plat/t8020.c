/* 
 * pongoOS - https://checkra.in
 * 
 * Copyright (C) 2019-2023 checkra1n team
 *
 * This file is part of pongoOS.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 */
#include <pongo.h>

static bool t8020_probe(struct hal_platform_driver* device_driver, struct hal_platform* device) {
    if (device->cpid == 0x8020) {
        return true;
    }
    return false;
}

static struct usb_regs t8020_usb_regs = {
    .reg1 = 0x3b0802a8,
    .reg2 = 0x3b0802b0,
    .reg3 = 0x3b0802a8,
    .otg_irq = 451
};

static bool t8020_get_platform_value(const char* name, void* value, size_t* size) {
    if (strcmp(name, "usb_regs") == 0 && *size == sizeof(struct usb_regs)) {
        memcpy(value, &t8020_usb_regs, sizeof(struct usb_regs));
        return true;
    }
    return false;
}

static struct hal_platform_driver t8020_plat = {
    .name = "Apple A12 (T8020)",
    .context = NULL,
    .probe = t8020_probe,
    .get_platform_value = t8020_get_platform_value
};

static void t8020_init(struct driver* driver) {
    hal_register_platform_driver(&t8020_plat);
}

REGISTER_DRIVER(t8020, t8020_init, NULL, DRIVER_FLAGS_PLATFORM);
