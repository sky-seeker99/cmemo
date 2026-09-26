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
//  TOOLKIT LIBRARY
//

#ifndef _T_TOOL_H_
#define _T_TOOL_H_

// テンプレート版 ----- 
template <class T> bool convStrNumCheck(char *str);
template <class T> T convStrNum     (char *str);
template <class T> T convStrNumSub  (char *str);
template <class T> T convStrNumSub_2(char *str);
template <class T> T hexc_long_t    (char *str,int len);
template <class T> T bitc_long_t    (char *str,int len);
template <class T> T decc_long_t    (char *str,int len);
double doubleConv(char *str);



#endif // _T_TOOL_H_
