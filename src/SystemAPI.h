/*
2016 WanquanWu CS1501 HUST

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation;
either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program;
if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

*/

//本头文件内包含可供主函数调用的api函数的原型
#pragma once

typedef struct System_Data *SystemData;

//用于初始化系统
SystemData Initialize(void);
//用于进入录入模块
void RecordInputMode(SystemData systemData);
//用于进入查询模块
void QueryMode		(SystemData systemData);
//用于进入修改模块
void ModifyMode		(SystemData systemData);
//用于进入保存模块
void SaveMode		(SystemData systemData);

//用于进行选项选择的获取
long GetChoice(long bottom, long top);