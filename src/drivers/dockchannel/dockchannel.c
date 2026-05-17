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

static uint64_t gDockchannelBase;
extern uint32_t gLogoBitmap[32];

#define rDATA_TX8       (*(volatile uint32_t*)(gDockchannelBase + 0x4004))
#define rDATA_TX_FREE   (*(volatile uint32_t*)(gDockchannelBase + 0x4014))

static inline void put_dockchannel_modifier(const char* str) {
    while (*str) dockchannel_putc(*str++);
}

void dockchannel_init(void)
{
    if (socnum != 0x8012)
        return;

    dt_node_t *node = dt_get("/arm-io/dockchannel-uart");
    gDockchannelBase = gIOBase + dt_node_u64(node, "reg", 0);

    char reorder[6] = {'1','3','2','6','4','5'};
    char modifier[] = {'\x1b', '[', '4', '1', ';', '1', 'm', 0};
    int cnt = 0;
    for (int y=0; y < 32; y++) {
        uint32_t b = gLogoBitmap[y];
        for (int x=0; x < 32; x++) {
            if (b & (1 << (x))) {
                modifier[3] = reorder[((cnt) % 6)];
                put_dockchannel_modifier(modifier);
            }
            dockchannel_putc(' ');
            dockchannel_putc(' ');
            if (b & (1 << (x))) {
                put_dockchannel_modifier("\x1b[0m");
            }
            cnt = (x+1) + y;
        }
        dockchannel_putc('\n');
    }
}

void dockchannel_putc(char c)
{
    if (c == '\n') dockchannel_putc('\r');
    if (!gDockchannelBase) return;
    while (rDATA_TX_FREE == 0) {}
    rDATA_TX8 = (unsigned int)(c);
    return;
}
