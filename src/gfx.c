/*****************************************************************************
 * Copyright (c) 2014 Ted John, Peter Hill
 * OpenRCT2, an open source clone of Roller Coaster Tycoon 2.
 *
 * This file is part of OpenRCT2.
 *
 * OpenRCT2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <windows.h>
#include "addresses.h"
#include "gfx.h"
#include "rct2.h"
#include "strings.h"
#include "window.h"
#include "osinterface.h"

// HACK These were originally passed back through registers
int gLastDrawStringX;
int gLastDrawStringY;

uint8 _screenDirtyBlocks[5120];

static void gfx_draw_dirty_blocks(int x, int y, int columns, int rows);

/**
 * 
 *  rct2: 0x00678998
 */
void gfx_load_g1()
{
	HANDLE hFile;
	DWORD bytesRead;
	DWORD header[2];

	int i;
	int g1BufferSize;
	void* g1Buffer;

	rct_g1_element *g1Elements = RCT2_ADDRESS(RCT2_ADDRESS_G1_ELEMENTS, rct_g1_element);

	hFile = CreateFile(get_file_path(PATH_ID_G1), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_FLAG_RANDOM_ACCESS | FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {
		ReadFile(hFile, header, 8, &bytesRead, NULL);
		if (bytesRead == 8) {
			g1BufferSize = header[1];
			g1Buffer = rct2_malloc(g1BufferSize);
			ReadFile(hFile, g1Elements, 29294 * sizeof(rct_g1_element), &bytesRead, NULL);
			ReadFile(hFile, g1Buffer, g1BufferSize, &bytesRead, NULL);
			CloseHandle(hFile);

			for (i = 0; i < 29294; i++)
				g1Elements[i].offset += (int)g1Buffer;

			return;
		}
	}

	// exit with error
	fprintf(stderr, "Unable to load g1.dat");
	assert(0);
}

/**
 * Clears the screen with the specified colour.
 *  rct2: 0x00678A9F
 */
void gfx_clear(rct_drawpixelinfo *dpi, int colour)
{
	int y, w, h;
	char* ptr;

	w = dpi->width >> dpi->var_0F;
	h = dpi->height >> dpi->var_0F;

	ptr = dpi->bits;
	for (y = 0; y < h; y++) {
		memset(ptr, colour, w);
		ptr += w + dpi->pitch;
	}
}

void gfx_draw_pixel(rct_drawpixelinfo *dpi, int x, int y, int colour)
{
	gfx_fill_rect(dpi, x, y, x, y, colour);
}

/*
* Draws a horizontal line of specified colour to a buffer.
* rct2: 0x68474C
*/
void gfx_draw_line_on_buffer(rct_drawpixelinfo *dpi, char colour, int y, int x, int no_pixels)
{
	y -= dpi->y;

	//Check to make sure point is in the y range
	if (y < 0)return;
	if (y >= dpi->height)return;
	//Check to make sure we are drawing at least a pixel
	if (!no_pixels) return;

	no_pixels++;
	x -= dpi->x;

	//If x coord outside range leave
	if (x < 0){
		//Unless the number of pixels is enough to be in range
		no_pixels += x;
		if (no_pixels <= 0)return;
		//Resets starting point to 0 as we don't draw outside the range
		x = 0;
	}

	//Ensure that the end point of the line is within range
	if (x + no_pixels - dpi->width > 0){
		//If the end point has any pixels outside range
		//cut them off. If there are now no pixels return.
		no_pixels -= x + no_pixels - dpi->width;
		if (no_pixels <= 0)return;
	}

	char* bits_pointer;
	//Get the buffer we are drawing to and move to the first coordinate.
	bits_pointer = dpi->bits + y*(dpi->pitch + dpi->width) + x;

	//Draw the line to the specified colour
	for (; no_pixels > 0; --no_pixels, ++bits_pointer){
		*((uint8*)bits_pointer) = colour;
	}
}


/**
 * Draws a line on dpi if within dpi boundaries
 *  rct2: 0x00684466
 * dpi (edi)
 * x1 (ax)
 * y1 (bx)
 * x2 (cx)
 * y2 (dx)
 * colour (ebp)
 */
void gfx_draw_line(rct_drawpixelinfo *dpi, int x1, int y1, int x2, int y2, int colour)
{
	// Check to make sure the line is within the drawing area
	if ((x1 < dpi->x) && (x2 < dpi->x)){
		return;
	}

	if ((y1 < dpi->y) && (y2 < dpi->y)){
		return;
	}

	if ((x1 >(dpi->x + dpi->width)) && (x2 >(dpi->x + dpi->width))){
		return;
	}

	if ((y1 > (dpi->y + dpi->height)) && (y2 > (dpi->y + dpi->height))){
		return;
	}

	//Bresenhams algorithm

	//If vertical plot points upwards
	int steep = abs(y2 - y1) > abs(x2 - x1);
	if (steep){
		int temp_y2 = y2;
		int temp_x2 = x2;
		y2 = x1;
		x2 = y1;
		y1 = temp_x2;
		x1 = temp_y2;
	}

	//If line is right to left swap direction
	if (x1 > x2){
		int temp_y2 = y2;
		int temp_x2 = x2;
		y2 = y1;
		x2 = x1;
		y1 = temp_y2;
		x1 = temp_x2;
	}

	int delta_x = x2 - x1;
	int delta_y = abs(y2 - y1);
	int error = delta_x / 2;
	int y_step;
	int y = y1;

	//Direction of step
	if (y1 < y2)y_step = 1;
	else y_step = -1;

	for (int x = x1, x_start = x1, no_pixels = 1; x < x2; ++x,++no_pixels){
		//Vertical lines are drawn 1 pixel at a time
		if (steep)gfx_draw_line_on_buffer(dpi, colour, x, y, 1);

		error -= delta_y;
		if (error < 0){
			//Non vertical lines are drawn with as many pixels in a horizontal line as possible
			if (!steep)gfx_draw_line_on_buffer(dpi, colour, y, x_start, no_pixels);

			//Reset non vertical line vars
			x_start = x + 1;
			no_pixels = 1;
			y += y_step;
			error += delta_x;
		}

		//Catch the case of the last line
		if (x + 1 == x2 && !steep){
			gfx_draw_line_on_buffer(dpi, colour, y, x_start, no_pixels);
		}
	}
	return;
}

/**
 *
 *  rct2: 0x00678AD4
 * dpi (edi)
 * left (ax)
 * top (cx)
 * right (bx)
 * bottom (dx)
 * colour (ebp)
 */
void gfx_fill_rect(rct_drawpixelinfo *dpi, int left, int top, int right, int bottom, int colour)
{

	int left_, right_, top_, bottom_;
	rct_drawpixelinfo* dpi_;
	left_ = left;
	right_ = right;
	top_ = top;
	bottom_ = bottom;
	dpi_ = dpi;
  
	if ((left > right) || (top > bottom) || (dpi->x > right) || (left >= (dpi->x + dpi->width)) ||
		(bottom < dpi->y) || (top >= (dpi->y + dpi->height)))
		return;

	colour |= RCT2_GLOBAL(0x009ABD9C, uint32);

	if (!(colour & 0x1000000)) {
		if (!(colour & 0x8000000)) {
			left_ = left - dpi->x;
			if (left_ < 0)
				left_ = 0;

			right_ = right - dpi->x;
			right_++;
			if (right_ > dpi->width)
				right_ = dpi->width;

			right_ -= left_;
	
			top_ = top - dpi->y;
			if (top_ < 0)
				top_ = 0;

			bottom_ = bottom - dpi->y;
			bottom_++;

			if (bottom_ > dpi->height)
				bottom_ = dpi->height;

			bottom_ -= top_;

			if (!(colour & 0x2000000)) {
				if (!(colour & 0x4000000)) {
					uint8* pixel = (top_ * (dpi->width + dpi->pitch)) + left_ + dpi->bits;

					int length = dpi->width + dpi->pitch - right_;

					for (int i = 0; i < bottom_; ++i) {
						memset(pixel, (colour & 0xFF), right_);
						pixel += length + right_;
					}
				} else {
					// 00678B8A   00678E38
					char* esi;
					esi = (top_ * (dpi->width + dpi->pitch)) + left_ + dpi->bits;;

					int eax, ebp;
					eax = colour;
					ebp = dpi->width + dpi->pitch - right_;

					RCT2_GLOBAL(0x00EDF810, uint32) = ebp;
					RCT2_GLOBAL(0x009ABDB2, uint16) = bottom_;
					RCT2_GLOBAL(0x00EDF814, uint32) = right_;

					top_ = (top + dpi->y) & 0xf;
					right_ = (right + dpi_->x) &0xf;

					dpi_ = (rct_drawpixelinfo*)esi;

					esi = (char*)(eax >> 0x1C);
					esi = (char*)RCT2_GLOBAL(0x0097FEFC,uint32)[esi]; // or possibly uint8)[esi*4] ?

					for (; RCT2_GLOBAL(0x009ABDB2, uint16) > 0; RCT2_GLOBAL(0x009ABDB2, uint16)--) {
						// push    ebx
						// push    ecx
						ebp = *(esi + top_*2);
					     
						// mov     bp, [esi+top_*2];
						int ecx;
						ecx = RCT2_GLOBAL(0x00EDF814, uint32);

						for (int i = ecx; i >=0; --i) {
							if (!(ebp & (1 << right_)))
								dpi_->bits = (char*)(left_ & 0xFF);
		
							right_++;
							right_ = right_ & 0xF;
							dpi_++;
						}
						// pop     ecx
						// pop     ebx
						top_++;
						top_ = top_ &0xf;
						dpi_ += RCT2_GLOBAL(0x00EDF810, uint32);
					}
					return;
				}

			} else {
				// 00678B7E   00678C83
				if (dpi->pad_0E < 1) {
					// Location in screen buffer?
					uint8* pixel = top_ * (dpi->width + dpi->pitch) + left_ + dpi->bits;

					// Find colour in colour table?
					uint32 eax = RCT2_ADDRESS(0x0097FCBC, uint32)[(colour & 0xFF)];
					rct_g1_element* g1_element = &(RCT2_ADDRESS(RCT2_ADDRESS_G1_ELEMENTS, rct_g1_element)[eax]);

					int length = (dpi->width + dpi->pitch) - right_;

					// Fill the rectangle with the colours from the colour table
					for (int i = 0; i < bottom_; ++i) {
						for (int j = 0; j < right_; ++j) {
							*pixel = *((uint8*)(&g1_element->offset[*pixel]));
							pixel++;
						}
						pixel += length;
					}
				} else if (dpi->pad_0E > 1) {
					// 00678C8A    00678D57
					right_ = right;
				} else if (dpi->pad_0E == 1) {
					// 00678C88   00678CEE
					right = right;
				}
	
			}
		} else {
			// 00678B3A    00678EC9
			right_ = right;
      }
  } else {
    // 00678B2E    00678BE5
		// Cross hatching
		uint16 pattern = 0;

		left_ = left_ - dpi->x;
		if (left_ < 0) {
			pattern = pattern ^ left_;
			left_ = 0;
		}

		right_ = right_ - dpi->x;
		right_++;

		if (right_ > dpi->width)
			right_ = dpi-> width;
		
		right_ = right_ - left_;

		top_ = top - dpi->y;
		if (top_ < 0) {
			pattern = pattern ^ top_;
			top_ = 0;
		}

		bottom_ = bottom - dpi->y;
		bottom_++;

		if (bottom_ > dpi->height)
			bottom_ = dpi->height;

		bottom_ -= top_;

		uint8* pixel = (top_ * (dpi->width + dpi->pitch)) + left_ + dpi->bits;

		int length = dpi->width + dpi->pitch - right_;

		uint32 ecx;
		for (int i = 0; i < bottom_; ++i) {
			ecx = pattern;
			// Rotate right
			ecx = (ecx >> 1) | (ecx << (sizeof(ecx) * CHAR_BIT - 1));
			ecx = (ecx & 0xFFFF0000) | right_; 
			// Fill every other pixel with the colour
			for (; (ecx & 0xFFFF) > 0; ecx--) {
				ecx = ecx ^ 0x80000000;
				if ((int)ecx < 0) {
					*pixel = colour & 0xFF;
				}
				pixel++;
			}
			pattern = pattern ^ 1;
			pixel += length;
			
		}
	}

	// RCT2_CALLPROC_X(0x00678AD4, left, right, top, bottom, 0, dpi, colour);
}

/**
 *  Draw a rectangle, with optional border or fill
 *
 *  rct2: 0x006E6F81
 * dpi (edi)
 * left (ax)
 * top (cx)
 * right (bx)
 * bottom (dx)
 * colour (ebp)
 * flags (si)
 */
void gfx_fill_rect_inset(rct_drawpixelinfo* dpi, short left, short top, short right, short bottom, int colour, short flags)
{
	uint8 shadow, fill, hilight;

	// Flags
	int no_border, no_fill, pressed;

	no_border = 8;
	no_fill = 0x10;
	pressed = 0x20;

	if (colour & 0x180) {
		if (colour & 0x100) {
			colour = colour & 0x7F;
		} else {
			colour = RCT2_ADDRESS(0x009DEDF4,uint8)[colour];
		}

		colour = colour | 0x2000000; //Transparent

		if (flags & no_border) {
			gfx_fill_rect(dpi, left, top, bottom, right, colour);
		} else if (flags & pressed) {
			// Draw outline of box
			gfx_fill_rect(dpi, left, top, left, bottom, colour + 1);
			gfx_fill_rect(dpi, left, top, right, top, colour + 1);
			gfx_fill_rect(dpi, right, top, right, bottom, colour + 2);
			gfx_fill_rect(dpi, left, bottom, right, bottom, colour + 2);

			if (!(flags & no_fill)) {
				gfx_fill_rect(dpi, left+1, top+1, right-1, bottom-1, colour);
			}
		} else {
			// Draw outline of box
			gfx_fill_rect(dpi, left, top, left, bottom, colour + 2);
			gfx_fill_rect(dpi, left, top, right, top, colour + 2);
			gfx_fill_rect(dpi, right, top, right, bottom, colour + 1);
			gfx_fill_rect(dpi, left, bottom, right, bottom, colour + 1);

			if (!(flags & no_fill)) {
				gfx_fill_rect(dpi, left+1, top+1, right-1, bottom-1, colour);
			}
		}
	} else {
		if (flags & 0x80) {
			shadow	= RCT2_ADDRESS(0x0141FC46, uint8)[colour * 8];
			fill	= RCT2_ADDRESS(0x0141FC48, uint8)[colour * 8];
			hilight	= RCT2_ADDRESS(0x0141FC4A, uint8)[colour * 8];
		} else {
			shadow	= RCT2_ADDRESS(0x0141FC47, uint8)[colour * 8];
			fill	= RCT2_ADDRESS(0x0141FC49, uint8)[colour * 8];
			hilight	= RCT2_ADDRESS(0x0141FC4B, uint8)[colour * 8];
		}

		if (flags & no_border) {
			gfx_fill_rect(dpi, left, top, right, bottom, fill);
		} else if (flags & pressed) {
			// Draw outline of box
			gfx_fill_rect(dpi, left, top, left, bottom, shadow);
			gfx_fill_rect(dpi, left + 1, top, right, top, shadow);
			gfx_fill_rect(dpi, right, top + 1, right, bottom - 1, hilight);
			gfx_fill_rect(dpi, left + 1, bottom, right, bottom, hilight);

			if (!(flags & no_fill)) {
				if (!(flags & 0x40)) {
					if (flags & 0x04) {
						fill = RCT2_ADDRESS(0x0141FC49, uint8)[0];
					} else {
						fill = RCT2_ADDRESS(0x0141FC4A, uint8)[colour * 8];
					}
				}
				gfx_fill_rect(dpi, left+1, top+1, right-1, bottom-1, fill);
			}
		} else {
			// Draw outline of box
			gfx_fill_rect(dpi, left, top, left, bottom - 1, hilight);
			gfx_fill_rect(dpi, left + 1, top, right - 1, top, hilight);
			gfx_fill_rect(dpi, right, top, right, bottom - 1, shadow);
			gfx_fill_rect(dpi, left, bottom, right, bottom, shadow);

			if (!(flags & no_fill)) {
				if (flags & 0x04) {
					fill = RCT2_ADDRESS(0x0141FC49, uint8)[0];
				}
				gfx_fill_rect(dpi, left+1, top+1, right-1, bottom-1, fill);
			}
		}
	}
}

#define RCT2_Y_RELATED_GLOBAL_1 0x9E3D12 //uint16
#define RCT2_Y_RELATED_GLOBAL_2 0x9ABDAC //sint16
#define RCT2_X_RELATED_GLOBAL_1 0x9E3D10 //uint16
#define RCT2_X_RELATED_GLOBAL_2 0x9ABDA8 //sint16

void sub_0x67AA18(int* source_bits_pointer, int* dest_bits_pointer, rct_drawpixelinfo *dpi){
	if (RCT2_GLOBAL(0xEDF81C, uint32) & 0x2000000){
		return; //0x67AAB3
	}

	if (RCT2_GLOBAL(0xEDF81C, uint32) & 0x4000000){
		return; //0x67AFD8
	}

	int ebx = RCT2_GLOBAL(0xEDF808, uint32);
	ebx = RCT2_GLOBAL(ebx * 2 + source_bits_pointer,uint16);
	int ebp = (int)dest_bits_pointer;
	ebx += (int)source_bits_pointer;

StartLoop:
	ebx = ebx;
	int cx = RCT2_GLOBAL(ebx, uint16);
	RCT2_GLOBAL(0x9ABDB4, uint8) = cx & 0xFF;
	ebx += 2;
	cx &= 0xFF7F;
	int esi = ebx;
	int edx = (cx & 0xFF00) >> 8;
	ebx += cx;
	edx -= RCT2_GLOBAL(0xEDF80C, sint32);
	int edi = ebp;
	if (edx > 0){
		edi += edx;
	}
	else{
		esi -= edx;
		cx += edx & 0xFFFF;
		if (cx <= 0){
			goto TestLoop;
			//jump to 0x67AA97
		}
		edx &= 0xFFFF0000;
	}
	edx += cx;
	edx -= RCT2_GLOBAL(0x9ABDA8, sint16);
	if (edx > 0){
		cx -= edx;
		if (cx <= 0){
			goto TestLoop;
			//jump to 0x67AA97
		}
	}
	if (cx & 1){
		cx >>= 1;
		RCT2_GLOBAL(edi, uint8) = RCT2_GLOBAL(esi, uint8);
	}
	else cx >>= 1;

	if (cx & 1){
		cx >>= 1;
		RCT2_GLOBAL(edi, uint16) = RCT2_GLOBAL(esi, uint16);
	}
	else cx >>= 1;

	for (int i = cx; i > 0; --i, edi++, esi++){
		RCT2_GLOBAL(edi, uint16) = RCT2_GLOBAL(esi, uint16);
	}
TestLoop:
	if (!(RCT2_GLOBAL(0x9ABDB4, uint8) & 0x80)) goto StartLoop;
	edx = RCT2_GLOBAL(0x9ABDB0, sint16);
	ebp += edx;
	RCT2_GLOBAL(0x9ABDAC, sint16)--;
	if (RCT2_GLOBAL(0x9ABDAC, sint16))goto StartLoop;

}

/*
* rct2: 0x67A934 title screen bitmaps on buttons
* This function readies all the global vars for copying the sprite data onto the screen
* I think its only used for bitmaps onto buttons but i am not sure.
*/
void sub_0x67A934(rct_drawpixelinfo *dpi, int x, int y){

	int _edi = (int)dpi;
	sint16 translated_x = x, translated_y = y;
	char* bits_pointer;
	bits_pointer = dpi->bits;

	translated_y += RCT2_GLOBAL(RCT2_Y_RELATED_GLOBAL_1, uint16) - dpi->y;
	RCT2_GLOBAL(0xEDF808, uint32) = 0;
	RCT2_GLOBAL(RCT2_Y_RELATED_GLOBAL_2, sint16) = RCT2_GLOBAL(0x9E3D0E, sint16);

	if (translated_y < 0)
	{
		RCT2_GLOBAL(RCT2_Y_RELATED_GLOBAL_2, sint16) += translated_y;
		if (RCT2_GLOBAL(RCT2_Y_RELATED_GLOBAL_2, sint16) <= 0)return;
		RCT2_GLOBAL(0xEDF808, sint16) -= translated_y;
		translated_y = 0;
	}
	else{
		bits_pointer += (dpi->width + dpi->pitch)*translated_y;
	}

	translated_y += RCT2_GLOBAL(RCT2_Y_RELATED_GLOBAL_2, sint16) - dpi->height;
	if (translated_y > 0){
		RCT2_GLOBAL(RCT2_Y_RELATED_GLOBAL_2, sint16) -= translated_y;
		if (RCT2_GLOBAL(RCT2_Y_RELATED_GLOBAL_2, sint16) <= 0)return;
	}

	RCT2_GLOBAL(0xEDF80C, uint32) = 0;
	translated_x += RCT2_GLOBAL(RCT2_X_RELATED_GLOBAL_1, uint16) - dpi->x;

	RCT2_GLOBAL(RCT2_X_RELATED_GLOBAL_2, sint16) = RCT2_GLOBAL(0x9E3D0C, sint16);
	if (translated_x < 0){
		RCT2_GLOBAL(RCT2_X_RELATED_GLOBAL_2, sint16) += translated_x;
		if (RCT2_GLOBAL(RCT2_X_RELATED_GLOBAL_2, sint16) <= 0)return;
		RCT2_GLOBAL(0xEDF80C, sint16) -= translated_x;
		translated_x = 0;
	}
	else{
		bits_pointer += translated_x;
	}
	translated_x += RCT2_GLOBAL(RCT2_X_RELATED_GLOBAL_2, sint16);
	translated_x -= dpi->width;
	if (translated_x > 0){
		RCT2_GLOBAL(RCT2_X_RELATED_GLOBAL_2, sint16) -= translated_x;
		if (RCT2_GLOBAL(RCT2_X_RELATED_GLOBAL_2, sint16) <= 0)return;
	}

	RCT2_GLOBAL(0x9ABDB0, uint16) = dpi->width + dpi->pitch;
	
	// I dont think it uses ecx, edx but just in case
	//esi is the source and bits_pointer is the destination
	//sub_0x67AA18(RCT2_GLOBAL(0x9E3D08, int*), (int*)bits_pointer, dpi);

	RCT2_CALLPROC_X_EBPSAFE(0x67AA18, 0, 0, translated_x, translated_y, RCT2_GLOBAL(0x9E3D08, uint32), (int)bits_pointer, (int)dpi);
}

/**
 *
 *  rct2: 0x0067A28E
 * image_id (ebx)
 * x (cx)
 * y (dx)
 */
void gfx_draw_sprite(rct_drawpixelinfo *dpi, int image_id, int x, int y)
{
	//RCT2_CALLPROC_X(0x0067A28E, 0, image_id, x, y, 0, dpi, 0);
	//return;

	int eax = 0, ebx = image_id, ecx = x, edx = y, esi = 0, edi = (int)dpi, ebp = 0;

	RCT2_GLOBAL(0x00EDF81C, uint32) = image_id & 0xE0000000;
	eax = (image_id >> 26) & 0x7;
	//eax = RCT2_GLOBAL(0x009E3CE4 + eax*4, uint32);
	RCT2_GLOBAL(0x009E3CDC, uint32) = RCT2_GLOBAL(0x009E3CE4 + eax * 4, uint32);

	if (((image_id)& 0xE0000000) && !(image_id & (1 << 31))) {
		RCT2_CALLPROC_X(0x0067A28E, 0, image_id, x, y, 0, (int)dpi, 0);
		//
		return;//jump into 0x67a445
	}
	else if (((image_id)& 0xE0000000) && !(image_id & (1 << 29))){
		char* find = "FINDMEDUNCAN";
		RCT2_CALLPROC_X(0x0067A28E, 0, image_id, x, y, 0, (int)dpi, 0);
		return;//jump into 0x67a361
	}
	else if ((image_id)& 0xE0000000){
		RCT2_CALLPROC_X(0x0067A28E, 0, image_id, x, y, 0, (int)dpi, 0);		
		/*
		eax = image_id;
		RCT2_GLOBAL(0x9E3CDC, uint32) = 0;
		eax >>= 19;
		eax &= 0x1f;
		eax = RCT2_GLOBAL(eax * 4 + 0x97FCBC, uint32);
		eax <<= 4;
		eax = RCT2_GLOBAL(eax + RCT2_ADDRESS_G1_ELEMENTS, uint32);
		ebp = *((uint32*)eax + 0xF3);
		esi = *((uint32*)eax + 0xF7);
		RCT2_GLOBAL(0x9ABEFF, uint32) = ebp;
		RCT2_GLOBAL(0x9ABF03, uint32) = esi;
		ebp = *((uint32*)eax + 0xFB);
		eax = ebx;

		RCT2_GLOBAL(0x9ABF07, uint32) = ebp;
		eax >>= 24;
		eax &= 0x1f;
		eax = RCT2_GLOBAL(eax * 4 + 0x97FCBC, uint32);
		eax <<= 4;
		eax = RCT2_GLOBAL(eax + RCT2_ADDRESS_G1_ELEMENTS, uint32);
		ebp = *((uint32*)eax + 0xF3);
		esi = *((uint32*)eax + 0xF7);
		RCT2_GLOBAL(0x9ABED6, uint32) = ebp;
		RCT2_GLOBAL(0x9ABEDA, uint32) = esi;
		ebp = *((uint32*)eax + 0xFB);

		RCT2_GLOBAL(0x9ABDA4, uint32) = 0x009ABE0C;
		RCT2_GLOBAL(0x9ABEDE, uint32) = ebp;*/
		return;
	}

	ebx &= 0x7FFFF;
	ebx <<= 4;
	ebx += RCT2_ADDRESS_G1_ELEMENTS;
	if (dpi->pad_0E >= 1){
		if (dpi->pad_0E == 1){
			return;
			//jump into 0x67bd81
		}
		if (dpi->pad_0E >= 3){
			return;//jump into 0x67FAAE
		}
		//jump into 0x67DADA
		return;
	}
	eax = *((uint32*)ebx + 2);
	ebp = *((uint32*)ebx + 3);
	RCT2_GLOBAL(0x9E3D08, uint32) = *((uint32*)ebx); //offset to g1 bits?
	RCT2_GLOBAL(0x9E3D0C, uint32) = *((uint32*)ebx + 1);
	RCT2_GLOBAL(0x9E3D10, uint32) = *((uint32*)ebx + 2); //X-Y related unsigned? sets RCT2_X_RELATED_GLOBAL_1 and Y
	RCT2_GLOBAL(0x9E3D14, uint32) = *((uint32*)ebx + 3);
	if (RCT2_GLOBAL(0x9E3D14, uint32) & (1 << 2)){
		//Title screen bitmaps
		//RCT2_CALLPROC_X(0x0067A934, eax, ebx, x, y, 0, dpi, ebp);
		sub_0x67A934(dpi, x, y);
		return;
	}
	
	//dpi on stack
	int translated_x, translated_y;
	char* bits_pointer;

	ebp = (int)dpi;
	esi = RCT2_GLOBAL(0x9E3D08, uint32);
	RCT2_GLOBAL(0x9E3CE0, uint32) = 0;
	bits_pointer = dpi->bits;	

	RCT2_GLOBAL(RCT2_Y_RELATED_GLOBAL_2, sint16) = RCT2_GLOBAL(0x9E3D0E, sint16);

	translated_y = y + RCT2_GLOBAL(RCT2_Y_RELATED_GLOBAL_1, uint16) - dpi->y;


	if (translated_y < 0){
		RCT2_GLOBAL(RCT2_Y_RELATED_GLOBAL_2, sint16) += translated_y;
		if (RCT2_GLOBAL(RCT2_Y_RELATED_GLOBAL_2, sint16) <= 0){
			return;
		}
		translated_y = -translated_y;
		esi += translated_y * RCT2_GLOBAL(0x9E3D0C, sint16);
		RCT2_GLOBAL(0x9E3CE0, sint32) += translated_y * RCT2_GLOBAL(0x9E3D0C, sint16);
		translated_y = 0;
	} else {
		eax = (dpi->width + dpi->pitch) * translated_y;
		bits_pointer += eax;
	}

	translated_y += RCT2_GLOBAL(RCT2_Y_RELATED_GLOBAL_2, sint16) - dpi->height;;

	if (translated_y > 0){
		RCT2_GLOBAL(RCT2_Y_RELATED_GLOBAL_2, sint16) -= translated_y;
		if (RCT2_GLOBAL(RCT2_Y_RELATED_GLOBAL_2, sint16) <=0)
		{
			return;
		}
	}

	RCT2_GLOBAL(RCT2_X_RELATED_GLOBAL_2, sint16) = RCT2_GLOBAL(0x9E3D0C, sint16);
	eax = dpi->width + dpi->pitch - RCT2_GLOBAL(0x9E3D0C, sint16);

	RCT2_GLOBAL(0x9ABDAE, uint16) = 0;
	RCT2_GLOBAL(0x9ABDB0, sint16) = dpi->width + dpi->pitch - RCT2_GLOBAL(0x9E3D0C, sint16);
	translated_x = x + RCT2_GLOBAL(RCT2_X_RELATED_GLOBAL_1, uint16) - dpi->x;

	if (translated_x < 0){

		RCT2_GLOBAL(RCT2_X_RELATED_GLOBAL_2, sint16) += translated_x;
		if (RCT2_GLOBAL(RCT2_X_RELATED_GLOBAL_2, sint16) <= 0){
			return;
		}

		translated_x -= RCT2_GLOBAL(0x9ABDAE, sint16);
		esi -= translated_x;
		RCT2_GLOBAL(0x9E3CE0, sint32) -= translated_x;
		RCT2_GLOBAL(0x9ABDB0, sint16) -= translated_x;
		translated_x = 0;
	}

	bits_pointer += translated_x;
	translated_x += RCT2_GLOBAL(RCT2_X_RELATED_GLOBAL_2, sint16) - dpi->width;

	if (translated_x > 0){
		RCT2_GLOBAL(RCT2_X_RELATED_GLOBAL_2, sint16) -= translated_x;
		if (RCT2_GLOBAL(RCT2_X_RELATED_GLOBAL_2, sint16) <= 0){
			return;
		}
		RCT2_GLOBAL(0x9ABDAE, sint16) += translated_x;
		RCT2_GLOBAL(0x9ABDB0, sint16) += translated_x;
	}

	if (!(RCT2_GLOBAL(0x9E3D14, uint16) & 0x02)){
		eax = (RCT2_GLOBAL(RCT2_Y_RELATED_GLOBAL_2, uint8)) << 8;
		edx = RCT2_GLOBAL(0x9ABDAE, sint16);
		ebp = RCT2_GLOBAL(0x9ABDB0, sint16);
		ebx = RCT2_GLOBAL(0xEDF81C, uint32);
		ecx = 0xFFFF&translated_x;
		//ebx, esi, edi, ah used in 0x67a690
		RCT2_CALLPROC_X_EBPSAFE(0x67A690, eax, ebx, ecx, edx, esi, (int)bits_pointer, ebp);
		return;
	}
	//0x67A60A
	RCT2_CALLPROC_X(0x0067A28E, 0, image_id, x, y, 0, (int)dpi, 0);
	esi -= RCT2_GLOBAL(0x9E3D08, sint32);
	return;
}

/**
 *
 *  rct2: 0x00683854
 * a1 (ebx)
 * product (cl)
 */
void gfx_transpose_palette(int pal, unsigned char product)
{
	int eax, ebx, ebp;
	uint8* esi, *edi;

	ebx = pal * 16;
	esi = (uint8*)(*((int*)(RCT2_ADDRESS_G1_ELEMENTS + ebx)));
	ebp = *((short*)(0x009EBD2C + ebx));
	eax = *((short*)(0x009EBD30 + ebx)) * 4;
	edi = (uint8*)0x01424680 + eax;

	for (; ebp > 0; ebp--) {
		edi[0] = (esi[0] * product) >> 8;
		edi[1] = (esi[1] * product) >> 8;
		edi[2] = (esi[2] * product) >> 8;
		esi += 3;
		edi += 4;
	}
	osinterface_update_palette((char*)0x01424680, 10, 236);
}

/**
 * Draws i formatted text string centred at i specified position.
 *  rct2: 0x006C1D6C
 * dpi (edi)
 * format (bx)
 * x (cx)
 * y (dx)
 * colour (al)
 * args (esi)
 */
void gfx_draw_string_centred(rct_drawpixelinfo *dpi, int format, int x, int y, int colour, void *args)
{
	RCT2_CALLPROC_X(0x006C1D6C, colour, format, x, y, (int)args, (int)dpi, 0);
}

/**
 *
 *  rct2: 0x006ED7E5
 */
void gfx_invalidate_screen()
{
	int width = RCT2_GLOBAL(RCT2_ADDRESS_SCREEN_WIDTH, sint16);
	int height = RCT2_GLOBAL(RCT2_ADDRESS_SCREEN_HEIGHT, sint16);
	gfx_set_dirty_blocks(0, 0, width, height);
}

/**
 * 
 *  rct2: 0x006E732D
 * left (ax)
 * top (bx)
 * right (dx)
 * bottom (bp)
 */
void gfx_set_dirty_blocks(int left, int top, int right, int bottom)
{
	int x, y;
	uint8 *screenDirtyBlocks = RCT2_ADDRESS(0x00EDE408, uint8);

	left = max(left, 0);
	top = max(top, 0);
	right = min(right, RCT2_GLOBAL(RCT2_ADDRESS_SCREEN_WIDTH, sint16));
	bottom = min(bottom, RCT2_GLOBAL(RCT2_ADDRESS_SCREEN_HEIGHT, sint16));

	if (left >= right)
		return;
	if (top >= bottom)
		return;

	right--;
	bottom--;

	left >>= RCT2_GLOBAL(0x009ABDF0, sint8);
	right >>= RCT2_GLOBAL(0x009ABDF0, sint8);
	top >>= RCT2_GLOBAL(0x009ABDF1, sint8);
	bottom >>= RCT2_GLOBAL(0x009ABDF1, sint8);

	for (y = top; y <= bottom; y++)
		for (x = left; x <= right; x++)
			screenDirtyBlocks[y * RCT2_GLOBAL(RCT2_ADDRESS_DIRTY_BLOCK_COLUMNS, sint32) + x] = 0xFF;
}

/**
 *
 *  rct2: 0x006E73BE
 */
void gfx_draw_all_dirty_blocks()
{
	int x, y, xx, yy, columns, rows;
	uint8 *screenDirtyBlocks = RCT2_ADDRESS(0x00EDE408, uint8);

	for (x = 0; x < RCT2_GLOBAL(RCT2_ADDRESS_DIRTY_BLOCK_COLUMNS, sint32); x++) {
		for (y = 0; y < RCT2_GLOBAL(RCT2_ADDRESS_DIRTY_BLOCK_ROWS, sint32); y++) {
			if (screenDirtyBlocks[y * RCT2_GLOBAL(RCT2_ADDRESS_DIRTY_BLOCK_COLUMNS, sint32) + x] == 0)
				continue;

			// Determine columns
			for (xx = x; xx < RCT2_GLOBAL(RCT2_ADDRESS_DIRTY_BLOCK_COLUMNS, sint32); xx++)
				if (screenDirtyBlocks[y * RCT2_GLOBAL(RCT2_ADDRESS_DIRTY_BLOCK_COLUMNS, sint32) + xx] == 0)
					break;
			columns = xx - x;

			// Check rows
			for (yy = y; yy < RCT2_GLOBAL(RCT2_ADDRESS_DIRTY_BLOCK_ROWS, sint32); yy++)
				for (xx = x; xx < x + columns; xx++)
					if (screenDirtyBlocks[yy * RCT2_GLOBAL(RCT2_ADDRESS_DIRTY_BLOCK_COLUMNS, sint32) + xx] == 0)
						goto endRowCheck;
			
		endRowCheck:
			rows = yy - y;
			gfx_draw_dirty_blocks(x, y, columns, rows);
		}
	}
}

static void gfx_draw_dirty_blocks(int x, int y, int columns, int rows)
{
	int left, top, right, bottom;
	uint8 *screenDirtyBlocks = RCT2_ADDRESS(0x00EDE408, uint8);

	// Unset dirty blocks
	for (top = y; top < y + rows; top++)
		for (left = x; left < x + columns; left++)
			screenDirtyBlocks[top * RCT2_GLOBAL(RCT2_ADDRESS_DIRTY_BLOCK_COLUMNS, sint32) + left] = 0;

	// Determine region in pixels
	left = max(0, x * RCT2_GLOBAL(RCT2_ADDRESS_DIRTY_BLOCK_WIDTH, sint16));
	top = max(0, y * RCT2_GLOBAL(RCT2_ADDRESS_DIRTY_BLOCK_HEIGHT, sint16));
	right = min(RCT2_GLOBAL(RCT2_ADDRESS_SCREEN_WIDTH, sint16), left + (columns * RCT2_GLOBAL(RCT2_ADDRESS_DIRTY_BLOCK_WIDTH, sint16)));
	bottom = min(RCT2_GLOBAL(RCT2_ADDRESS_SCREEN_HEIGHT, sint16), top + (rows * RCT2_GLOBAL(RCT2_ADDRESS_DIRTY_BLOCK_HEIGHT, sint16)));
	if (right <= left || bottom <= top)
		return;

	// Draw region
	gfx_redraw_screen_rect(left, top, right, bottom);
}

/**
 * 
 *  rct2: 0x006E7499 
 * left (ax)
 * top (bx)
 * right (dx)
 * bottom (bp)
 */
void gfx_redraw_screen_rect(short left, short top, short right, short bottom)
{
	rct_window* w;
	rct_drawpixelinfo *screenDPI = RCT2_ADDRESS(RCT2_ADDRESS_SCREEN_DPI, rct_drawpixelinfo);
	rct_drawpixelinfo *windowDPI = RCT2_ADDRESS(RCT2_ADDRESS_WINDOW_DPI, rct_drawpixelinfo);

	// Unsure what this does
	RCT2_CALLPROC_X(0x00683326, left, top, right - 1, bottom - 1, 0, 0, 0);

	windowDPI->bits = screenDPI->bits + left + ((screenDPI->width + screenDPI->pitch) * top);
	windowDPI->x = left;
	windowDPI->y = top;
	windowDPI->width = right - left;
	windowDPI->height = bottom - top;
	windowDPI->pitch = screenDPI->width + screenDPI->pitch + left - right;

	for (w = RCT2_ADDRESS(RCT2_ADDRESS_WINDOW_LIST, rct_window); w < RCT2_GLOBAL(RCT2_ADDRESS_NEW_WINDOW_PTR, rct_window*); w++) {
		if (w->flags & WF_TRANSPARENT)
			continue;
		if (right <= w->x || bottom <= w->y)
			continue;
		if (left >= w->x + w->width || top >= w->y + w->height)
			continue;
		window_draw(w, left, top, right, bottom);
	}
}

/**
 * 
 *  rct2: 0x006C2321
 * buffer (esi)
 */
int gfx_get_string_width(char *buffer)
{
	int eax, ebx, ecx, edx, esi, edi, ebp;

	esi = (int)buffer;
	RCT2_CALLFUNC_X(0x006C2321, &eax, &ebx, &ecx, &edx, &esi, &edi, &ebp);

	return ecx & 0xFFFF;
}

/**
 * Draws i formatted text string left aligned at i specified position but clips
 * the text with an elipsis if the text width exceeds the specified width.
 *  rct2: 0x006C1B83
 * dpi (edi)
 * format (bx)
 * args (esi)
 * colour (al)
 * x (cx)
 * y (dx)
 * width (bp)
 */
void gfx_draw_string_left_clipped(rct_drawpixelinfo* dpi, int format, void* args, int colour, int x, int y, int width)
{
	RCT2_CALLPROC_X(0x006C1B83, colour, format, x, y, (int)args, (int)dpi, width);

	//char* buffer;

	//buffer = (char*)0x0141ED68;
	//format_string(buffer, format, args);
	//rctmem->current_font_sprite_base = 224;
	//clip_text(buffer, width);
	//gfx_draw_string(dpi, buffer, colour, x, y);
}

/**
 * Draws i formatted text string centred at i specified position but clips the
 * text with an elipsis if the text width exceeds the specified width.
 *  rct2: 0x006C1BBA
 * dpi (edi)
 * format (bx)
 * args (esi)
 * colour (al)
 * x (cx)
 * y (dx)
 * width (bp)
 */
void gfx_draw_string_centred_clipped(rct_drawpixelinfo *dpi, int format, void *args, int colour, int x, int y, int width)
{
	RCT2_CALLPROC_X(0x006C1BBA, colour, format, x, y, (int)args, (int)dpi, width);

	//char* buffer;
	//short text_width;

	//buffer = (char*)0x0141ED68;
	//format_string(buffer, format, args);
	//rctmem->current_font_sprite_base = 224;
	//text_width = clip_text(buffer, width);

	//// Draw the text centred
	//x -= (text_width - 1) / 2;
	//gfx_draw_string(dpi, buffer, colour, x, y);
}


/**
 * Draws i formatted text string right aligned.
 *  rct2: 0x006C1BFC
 * dpi (edi)
 * format (bx)
 * args (esi)
 * colour (al)
 * x (cx)
 * y (dx)
 */
void gfx_draw_string_right(rct_drawpixelinfo* dpi, int format, void* args, int colour, int x, int y)
{
	char* buffer;
	short text_width;

	buffer = (char*)0x0141ED68;
	format_string(buffer, format, args);

	// Measure text width
	text_width = gfx_get_string_width(buffer);

	// Draw the text right aligned
	x -= text_width;
	gfx_draw_string(dpi, buffer, colour, x, y);
}

/**
 * 
 *  rct2: 0x006C1E53
 * dpi (edi)
 * args (esi)
 * x (cx)
 * y (dx)
 * width (bp)
 * colour (al)
 * format (ebx)
 */
int gfx_draw_string_centred_wrapped(rct_drawpixelinfo *dpi, void *args, int x, int y, int width, int format, int colour)
{
	int eax, ebx, ecx, edx, esi, edi, ebp;

	eax = colour;
	ebx = format;
	ecx = x;
	edx = y;
	esi = (int)args;
	edi = (int)dpi;
	ebp = width;
	RCT2_CALLFUNC_X(0x006C1E53, &eax, &ebx, &ecx, &edx, &esi, &edi, &ebp);

	return (sint16)(edx & 0xFFFF) - y;
}

/**
 * 
 *  rct2: 0x006C2105
 * dpi (edi)
 * format (esi)
 * x (cx)
 * y (dx)
 * width (bp)
 * colour (bx)
 * unknown (al)
 */
int gfx_draw_string_left_wrapped(rct_drawpixelinfo *dpi, void *format, int x, int y, int width, int colour, int unknown)
{
	int eax, ebx, ecx, edx, esi, edi, ebp;

	eax = unknown;
	ebx = colour;
	ecx = x;
	edx = y;
	esi = (int)format;
	edi = (int)dpi;
	ebp = width;
	RCT2_CALLFUNC_X(0x006C2105, &eax, &ebx, &ecx, &edx, &esi, &edi, &ebp);

	return (sint16)(edx & 0xFFFF) - y;
}

/**
 * Draws i formatted text string.
 *  rct2: 0x006C1B2F
 * dpi (edi)
 * format (bx)
 * args (esi)
 * colour (al)
 * x (cx)
 * y (dx)
 */
void gfx_draw_string_left(rct_drawpixelinfo *dpi, int format, void *args, int colour, int x, int y)
{
	char* buffer;

	buffer = (char*)0x0141ED68;
	format_string(buffer, format, args);
	gfx_draw_string(dpi, buffer, colour, x, y);
}

/**
 * 
 *  rct2: 0x00682702
 * dpi (edi)
 * format (esi)
 * colour (al)
 * x (cx)
 * y (dx)
 */
void gfx_draw_string(rct_drawpixelinfo *dpi, char *format, int colour, int x, int y)
{
	int eax, ebx, ecx, edx, esi, edi, ebp;

	eax = colour;
	ebx = 0;
	ecx = x;
	edx = y;
	esi = (int)format;
	edi = (int)dpi;
	ebp = 0;
	RCT2_CALLFUNC_X(0x00682702, &eax, &ebx, &ecx, &edx, &esi, &edi, &ebp);

	gLastDrawStringX = ecx;
	gLastDrawStringY = edx;
}
