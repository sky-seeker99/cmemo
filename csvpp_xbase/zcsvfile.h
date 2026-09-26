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
