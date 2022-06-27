/**
 *  Simple Slot
 *
 *  Copyright (C) 2006-2018 Teru Kamogashira
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "freeverb/slot.hpp"
#include "freeverb/fv3_type_float.h"
#include "freeverb/fv3_ns_start.h"

FV3_(slot)::FV3_(slot)()
{
  size = ch = 0;
  data = NULL; L = R = NULL;
}

FV3_(slot)::~FV3_(slot)()
{
  free();
}

void FV3_(slot)::alloc(long nsize, long nch)
{
  if(nsize <= 0||nch <= 0) return;
  free();
  data = new fv3_float_t*[nch];
  for(long t = 0;t < nch;t ++)
    {
      data[t] = (fv3_float_t*)FV3_(utils)::aligned_malloc(sizeof(fv3_float_t)*nsize, FV3_PTR_ALIGN_BYTE);
    }
  size = nsize; ch = nch;
  L = this->c(0); R = this->c(1);
  mute();
}

fv3_float_t * FV3_(slot)::c(long nch)
{
  if(ch == 0||size == 0||data == NULL) return NULL;
  if(nch < ch) return data[nch];
  return data[0];
}

void FV3_(slot)::free()
{
  if(size > 0&&ch > 0&&data != NULL)
    {
      for(long t = 0;t < ch;t ++) FV3_(utils)::aligned_free(data[t]);
      delete[] data;
    }
  size = ch = 0;
  data = NULL; L = R = NULL;
}

fv3_float_t ** FV3_(slot)::getArray()
{
  return data;
}

void FV3_(slot)::mute()
{
  if(ch == 0||size == 0||data == NULL) return;
  for(long t = 0;t < ch;t ++) FV3_(utils)::mute(data[t], size);
}

void FV3_(slot)::mute(long limit)
{
  if(ch == 0||size == 0||data == NULL||limit < 0) return;
  if(limit > size) limit = size;
  for(long t = 0;t < ch;t ++) FV3_(utils)::mute(data[t], limit);
}

void FV3_(slot)::mute(long offset, long limit)
{
  if(ch == 0||size == 0||data == NULL||offset < 0||limit < 0) return;
  if(offset > size) offset = size;
  if(offset+limit > size) limit = size-offset;
  for(long t = 0;t < ch;t ++) FV3_(utils)::mute(data[t]+offset, limit);
}

#include "freeverb/fv3_ns_end.h"
