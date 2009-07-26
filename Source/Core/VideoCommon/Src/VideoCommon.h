// Copyright (C) 2003-2009 Dolphin Project.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2.0.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 2.0 for more details.

// A copy of the GPL 2.0 should have been included with the program.
// If not, see http://www.gnu.org/licenses/

// Official SVN repository and contact information can be found at
// http://code.google.com/p/dolphin-emu/

#ifndef _VIDEOCOMMON_H
#define _VIDEOCOMMON_H

#include "Common.h"
#include "MathUtil.h"
#include "pluginspecs_video.h"

#if defined(_MSC_VER) && !defined(__x86_64__) && !defined(_M_X64)
void * memcpy_amd(void *dest, const void *src, size_t n);
unsigned char memcmp_mmx(const void* src1, const void* src2, int cmpsize);
#define memcpy_gc memcpy_amd
#define memcmp_gc memcmp_mmx
#else
#define memcpy_gc memcpy
#define memcmp_gc memcmp
#endif

// These are accurate (disregarding AA modes).
enum
{
	EFB_WIDTH = 640,
	EFB_HEIGHT = 528,
};

enum
{
	// XFB width is decided by EFB copy operation. The VI can do horizontal
	// scaling (TODO: emulate).
	MAX_XFB_WIDTH = EFB_WIDTH,

	// Although EFB height is 528, 574-line XFB's can be created either with
	// vertical scaling by the EFB copy operation or copying to multiple XFB's
	// that are next to each other in memory (TODO: handle that situation).
	MAX_XFB_HEIGHT = 574
};

// If this is enabled, bounding boxes will be computed for everything drawn.
// This can theoretically have a big speed hit in some geom heavy games. Needs more work.
// Helps some effects in Paper Mario (but they aren't quite right yet).
// May help Super Mario Galaxy?
// Do testing to figure out if the speed hit is bad?
// #define BBOX_SUPPORT

extern SVideoInitialize g_VideoInitialize;

//////////////////////////////////////////////////////////////////////////
inline u8 *Memory_GetPtr(u32 _uAddress)
{
	return g_VideoInitialize.pGetMemoryPointer(_uAddress);
}

inline u8 Memory_Read_U8(u32 _uAddress)
{
	return *(u8*)g_VideoInitialize.pGetMemoryPointer(_uAddress);
}

inline u16 Memory_Read_U16(u32 _uAddress)
{
	return Common::swap16(*(u16*)g_VideoInitialize.pGetMemoryPointer(_uAddress));
}

inline u32 Memory_Read_U32(u32 _uAddress)
{
	return Common::swap32(*(u32*)g_VideoInitialize.pGetMemoryPointer(_uAddress));
}
//////////////////////////////////////////////////////////////////////////
inline u8* Memory_Read_U8_Ptr(u32 _uAddress)
{
	return (u8*)g_VideoInitialize.pGetMemoryPointer(_uAddress);
}

inline u16* Memory_Read_U16_Unswapped_Ptr(u32 _uAddress)
{
	return (u16*)g_VideoInitialize.pGetMemoryPointer(_uAddress);
}

inline u32* Memory_Read_U32_Unswapped_Ptr(u32 _uAddress)
{
	return (u32*)g_VideoInitialize.pGetMemoryPointer(_uAddress);
}
//////////////////////////////////////////////////////////////////////////

inline float Memory_Read_Float(u32 _uAddress)
{
	union {u32 i; float f;} temp;
	temp.i = Memory_Read_U32(_uAddress);
	return temp.f;
}

// This structure should only be used to represent a rectangle in EFB
// coordinates, where the origin is at the upper left and the frame dimensions
// are 640 x 528.
struct EFBRectangle : public MathUtil::Rectangle<int>
{};

// Logging
// ŻŻŻŻŻŻŻŻŻŻ
void HandleGLError();



#ifdef _WIN32
#define PRIM_LOG(...) {DEBUG_LOG(VIDEO, __VA_ARGS__)}
#else
#define PRIM_LOG(...) {DEBUG_LOG(VIDEO, ##__VA_ARGS__)}
#endif

#define LOG_VTX() DEBUG_LOG(VIDEO, "vtx: %f %f %f, ", ((float*)VertexManager::s_pCurBufferPointer)[0], ((float*)VertexManager::s_pCurBufferPointer)[1], ((float*)VertexManager::s_pCurBufferPointer)[2]);


#endif  // _VIDEOCOMMON_H
