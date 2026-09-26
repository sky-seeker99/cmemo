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
// link information class
//

#ifndef _XLINK_H_
#define _XLINK_H_


class link_C{
  public:
    void            *g_psl;
    void            *g_ppf;
    HRESULT          hRes;
    bool             okFlg;
    WIN32_FIND_DATA  *wfd;
    char             dest_link[MAX_PATH];
    bool             linkCheckFlg;

    link_C();
    ~link_C();

    char *link_C::Get(char *file);
    bool link_C::dirHantei();
};  

#endif // _XLINK_H_

