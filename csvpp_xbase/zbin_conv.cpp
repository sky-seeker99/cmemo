/*

 * Copyright (c) 2010 Shigeru Kasuya (sky_seeker99@users.sourceforge.jp)
 *
 *    This source code is free software; you can redistribute it
 *    and/or modify it in source code form under the terms of the GNU
 *    General Public License as published by the Free Software
 *    Foundation; either version 2 of the License, or (at your option)
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

 
#include "zbin_conv.h"

//---------------------------
// atai to binary data
// 8'b10  -> 00000010(01000000)
// 8'h10  -> 00010000(00001000)
// 8'10   -> 00001010(01010000)
//---------------------------
char *binConv_C::conv(char *in)
  {
  int    size;
  int    i,len;
  long   w_long;

  size = -1;
  len  = strlen(in);
  for(i=0;i<len;++i)
    {
    if (*(in+i) == '\'')
      {
      *(in+i) = 0x00;
      size = SujiConvEx(in);
      *(in+i) = '\'';
      break;
      }
    }
  if (size < 1){return(NULL);}

  if (*(in+i+1) == 'h')
    {
    bitc_ex(in+i+2,strlen(in+i+2));
    rtn_cd->beta(size,'0');
    w     ->beta(strlen(in+i+2)*4,'0');
    bitc_hextobin(w->c_str(),in+i+2,strlen(in+i+2)*4);
    bit_dif_cpy(rtn_cd->c_str(),w->c_str(),size,strlen(in+i+2)*4);
    bitc_ex(in+i+2,strlen(in+i+2));
    return(rtn_cd->c_str());
    }

  if (*(in+i+1) == 'b')
    {
    bitc_ex(in+i+2,strlen(in+i+2));
    rtn_cd->beta(size,'0');
    bit_dif_cpy(rtn_cd->c_str(),in+i+2,size,strlen(in+i+2));
    bitc_ex(in+i+2,strlen(in+i+2));
    return(rtn_cd->c_str());
    }

  w_long = SujiConvEx(in+i+1);
  if (w_long == -1){return(NULL);}

  rtn_cd->beta(size,'0');
  long_bitc(rtn_cd->c_str(),w_long,size);
  return(rtn_cd->c_str());
  }



