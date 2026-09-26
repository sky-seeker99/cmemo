/*
 * Copyright 2026 Shigeru Kasuya
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by me or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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



