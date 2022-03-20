/*

 * Copyright (c) 2003 Shigeru Kasuya (sky_seeker99@users.sourceforge.jp)
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
//
// csv file read class ŒÝŠ·
//

#ifndef _ZCSVFILE_H_
#define _ZCSVFILE_H_

#include "xarea.h"
#include "xread64.h"
#include "xcsvi_anl.h"


// # Temporary Class ------------------
class CsvInf_T{
  private:
    char *buff;
  public:
    CsvAnl_C  *csvi;  
    Read64_C  *in_fp;
    bool       pushFlag;
	int        line;
	sChar     *file_name;

	CsvInf_T(char *file){
	  in_fp = new Read64_C(file,"csv",1024);
	  csvi  = new CsvAnl_C;
	  pushFlag = false;
	  line     = 0;
	  file_name = new sChar(file);
	}

    ~CsvInf_T(){
      delete csvi;
	  delete in_fp;
	  delete file_name;
    }
    int CsvError_Rd();
    int CsvFgets();
    int CsvSelSu_Rd();
    char *CsvSel_Rd(int no);
    char **CsvSel_Rd();
    void sp_push_set(bool flag){pushFlag = flag;}

    void CsvWrite(FILE *out_fp,char *str);

	void CsvError(char *msg,int clm);
    void CsvError2(char *msg,int clm);
    void CsvClmMsg(sChar *str,int clm);
    char *xFgetsBuff_Rd();

};



#endif // _ZCSVFILE_H_
