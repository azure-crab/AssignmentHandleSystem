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

#include <stdio.h>
#include <stdlib.h>
#include "SystemAPI.h"

void Guidence(void);
void About(void);

int main(void)
{
	SystemData systemData = Initialize();
	Guidence();
	for (;;)
	{
		printf("作业管理系统主选单\n\n请输入你想要使用的功能模块的编号并回车来选择该模块，也可用0来查看各功能简单说明：");
		int choice = GetChoice(0, 6);
		switch (choice) {
		case 1: RecordInputMode(systemData);	break;
		case 2: QueryMode(systemData);			break;
		case 3: ModifyMode(systemData);			break;
		case 4: SaveMode(systemData);			break;
		case 5: About();						break;
		case 6: return;
		case 0: 
			Guidence();						
			system("pause");
			break;
		}
		system("cls");
	}
}

void Guidence(void)
{
	printf("欢迎使用本学生课程作业成绩信息管理系统。\n"
		"本系统有以下功能可选：\n"
		"1：进入录入模块（包括载入存档，添加各项数据）\n"
		"2：进入查询模块（包括查询作业成绩，查询作业成绩总计信息）\n"
		"3：进入修改模块（包括修改各项信息，删除数据项等）\n"
		"4：进入保存模块（将数据存档）\n"
		"5：关于（包括使用帮助）\t6：退出\n"
		"输入数字（0-6）并回车来选择一个选项。\n");
}

void About(void)
{	
	system("cls");
	printf("学生作业成绩信息管理系统v1.1.0 by 吴万泉\n\n"
		"本程序遵从GPLv2协议开源，任何基于本程序或引用本程序部分的产品，请注意授权问题。"
		"任何引用本程序源码的产品或出版物，不得用于商业用途。\n\n"
		"    欢迎使用本作业成绩管理系统。\n"
		"    刚启动本系统时，若有存档，请载入存档，或者进入录入模块录入数据。\n"
		"    录入模块提供添加课程、添加学生、向课程中添加学生或作业的功能。"
		"注意向课程中添加学生只能添加系统中已经录入的学生。"
		"作业成绩的录入会在学生/作业被添加入课程后自动开始。除名称录入外提供完备的错误检查。"
		"本系统支持保存多个课程的记录，每个课程内可以包含多次的作业与多名学生。"
		"课程、学生、作业都有名称和Id两项数据，Id目前仅支持数字（以Id为唯一标识，同类同名称不同Id的项视为不同项）\n\n"
		"    如果需要查询系统内的记录，请使用查询模块。"
		"查询模块提供多重查询、精准查询和查询总计信息功能。"
		"多重查询可以一次查询一组信息或者作业成绩数据，比如系统内所有课程的信息，或某课程内某次作业下的所有作业成绩数据。"
		"精准查询可以查询指定的某一项作业成绩数据。"
		"查询总计信息让你可以方便地查看同属的大量作业成绩数据的统计信息，包括应交量、实交量、上交率、优秀作业量、优秀率，"
		"你可以查询指定课程、指定学生、指定课程下某学生或某项作业的总计信息。\n\n"
		"    如果需要修改数据，请进入修改模块，在这里，你可以修改课程/学生/作业的名称，修改指定学生参加的课程，或删除课程/学生/作业。\n\n"
		"    保存模块为你将数据存档，留待下一次启动系统时载入。\n\n"
		"    关于模块提供本系统的一些信息，并给出使用该系统的简单的指导。\n\n");
	printf("请选择一个选项：1：GPLv2协议全文\t2：返回主选单\n");
	int choice = GetChoice(1, 2);
	if (choice == 2)
		return;
	else {
		printf("Full License Text\n"
			"GNU General Public License, version 2 (GPL-2.0)\n"
			"The GNU General Public License (GPL-2.0)\n"
			"Version 2, June 1991\n"
			"Copyright (C) 1989, 1991 Free Software Foundation, Inc.\n"
			"59 Temple Place, Suite 330, Boston, MA 02111-1307 USA\n"
			"Everyone is permitted to copy and distribute verbatim copies\n"
			"of this license document, but changing it is not allowed.\n"
			"Preamble\n"
			"The licenses for most software are designed to take away your freedom to share and change it. By contrast, the GNU General Public License is intended to guarantee your freedom to share and change free software--to make sure the software is free for all its users. This General Public License applies to most of the Free Software Foundation\'s software and to any other program whose authors commit to using it. (Some other Free Software Foundation software is covered by the GNU Library General Public License instead.) You can apply it to your programs, too.\n"
			"When we speak of free software, we are referring to freedom, not price. Our General Public Licenses are designed to make sure that you have the freedom to distribute copies of free software (and charge for this service if you wish), that you receive source code or can get it if you want it, that you can change the software or use pieces of it in new free programs; and that you know you can do these things.\n"
			"To protect your rights, we need to make restrictions that forbid anyone to deny you these rights or to ask you to surrender the rights. These restrictions translate to certain responsibilities for you if you distribute copies of the software, or if you modify it.\n"
			"For example, if you distribute copies of such a program, whether gratis or for a fee, you must give the recipients all the rights that you have. You must make sure that they, too, receive or can get the source code. And you must show them these terms so they know their rights.\n"
			"We protect your rights with two steps: (1) copyright the software, and (2) offer you this license which gives you legal permission to copy, distribute and/or modify the software.\n"
			"Also, for each author\'s protection and ours, we want to make certain that everyone understands that there is no warranty for this free software. If the software is modified by someone else and passed on, we want its recipients to know that what they have is not the original, so that any problems introduced by others will not reflect on the original authors\' reputations.\n"
			"Finally, any free program is threatened constantly by software patents. We wish to avoid the danger that redistributors of a free program will individually obtain patent licenses, in effect making the program proprietary. To prevent this, we have made it clear that any patent must be licensed for everyone\'s free use or not licensed at all.\n"
			"The precise terms and conditions for copying, distribution and modification follow.\n"
			"TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION\n"
			"0. This License applies to any program or other work which contains a notice placed by the copyright holder saying it may be distributed under the terms of this General Public License. The \"Program\", below, refers to any such program or work, and a \"work based on the Program\" means either the Program or any derivative work under copyright law: that is to say, a work containing the Program or a portion of it, either verbatim or with modifications and/or translated into another language. (Hereinafter, translation is included without limitation in the term \"modification\".) Each licensee is addressed as \"you\".\n"
			"Activities other than copying, distribution and modification are not covered by this License; they are outside its scope. The act of running the Program is not restricted, and the output from the Program is covered only if its contents constitute a work based on the Program (independent of having been made by running the Program). Whether that is true depends on what the Program does.\n"
			"1. You may copy and distribute verbatim copies of the Program\'s source code as you receive it, in any medium, provided that you conspicuously and appropriately publish on each copy an appropriate copyright notice and disclaimer of warranty; keep intact all the notices that refer to this License and to the absence of any warranty; and give any other recipients of the Program a copy of this License along with the Program.\n"
			"You may charge a fee for the physical act of transferring a copy, and you may at your option offer warranty protection in exchange for a fee.\n"
			"2. You may modify your copy or copies of the Program or any portion of it, thus forming a work based on the Program, and copy and distribute such modifications or work under the terms of Section 1 above, provided that you also meet all of these conditions:\n"
			"a) You must cause the modified files to carry prominent notices stating that you changed the files and the date of any change.\n"
			"b) You must cause any work that you distribute or publish, that in whole or in part contains or is derived from the Program or any part thereof, to be licensed as a whole at no charge to all third parties under the terms of this License.\n"
			"c) If the modified program normally reads commands interactively when run, you must cause it, when started running for such interactive use in the most ordinary way, to print or display an announcement including an appropriate copyright notice and a notice that there is no warranty (or else, saying that you provide a warranty) and that users may redistribute the program under these conditions, and telling the user how to view a copy of this License. (Exception: if the Program itself is interactive but does not normally print such an announcement, your work based on the Program is not required to print an announcement.)\n"
			"These requirements apply to the modified work as a whole. If identifiable sections of that work are not derived from the Program, and can be reasonably considered independent and separate works in themselves, then this License, and its terms, do not apply to those sections when you distribute them as separate works. But when you distribute the same sections as part of a whole which is a work based on the Program, the distribution of the whole must be on the terms of this License, whose permissions for other licensees extend to the entire whole, and thus to each and every part regardless of who wrote it.\n"
			"Thus, it is not the intent of this section to claim rights or contest your rights to work written entirely by you; rather, the intent is to exercise the right to control the distribution of derivative or collective works based on the Program.\n"
			"In addition, mere aggregation of another work not based on the Program with the Program (or with a work based on the Program) on a volume of a storage or distribution medium does not bring the other work under the scope of this License.\n"
			"3. You may copy and distribute the Program (or a work based on it, under Section 2) in object code or executable form under the terms of Sections 1 and 2 above provided that you also do one of the following:\n"
			"a) Accompany it with the complete corresponding machine-readable source code, which must be distributed under the terms of Sections 1 and 2 above on a medium customarily used for software interchange; or,\n"
			"b) Accompany it with a written offer, valid for at least three years, to give any third party, for a charge no more than your cost of physically performing source distribution, a complete machine-readable copy of the corresponding source code, to be distributed under the terms of Sections 1 and 2 above on a medium customarily used for software interchange; or,\n"
			"c) Accompany it with the information you received as to the offer to distribute corresponding source code. (This alternative is allowed only for noncommercial distribution and only if you received the program in object code or executable form with such an offer, in accord with Subsection b above.)\n"
			"The source code for a work means the preferred form of the work for making modifications to it. For an executable work, complete source code means all the source code for all modules it contains, plus any associated interface definition files, plus the scripts used to control compilation and installation of the executable. However, as a special exception, the source code distributed need not include anything that is normally distributed (in either source or binary form) with the major components (compiler, kernel, and so on) of the operating system on which the executable runs, unless that component itself accompanies the executable.\n"
			"If distribution of executable or object code is made by offering access to copy from a designated place, then offering equivalent access to copy the source code from the same place counts as distribution of the source code, even though third parties are not compelled to copy the source along with the object code.\n"
			"4. You may not copy, modify, sublicense, or distribute the Program except as expressly provided under this License. Any attempt otherwise to copy, modify, sublicense or distribute the Program is void, and will automatically terminate your rights under this License. However, parties who have received copies, or rights, from you under this License will not have their licenses terminated so long as such parties remain in full compliance.\n"
			"5. You are not required to accept this License, since you have not signed it. However, nothing else grants you permission to modify or distribute the Program or its derivative works. These actions are prohibited by law if you do not accept this License. Therefore, by modifying or distributing the Program (or any work based on the Program), you indicate your acceptance of this License to do so, and all its terms and conditions for copying, distributing or modifying the Program or works based on it.\n"
			"6. Each time you redistribute the Program (or any work based on the Program), the recipient automatically receives a license from the original licensor to copy, distribute or modify the Program subject to these terms and conditions. You may not impose any further restrictions on the recipients\' exercise of the rights granted herein. You are not responsible for enforcing compliance by third parties to this License.\n"
			"7. If, as a consequence of a court judgment or allegation of patent infringement or for any other reason (not limited to patent issues), conditions are imposed on you (whether by court order, agreement or otherwise) that contradict the conditions of this License, they do not excuse you from the conditions of this License. If you cannot distribute so as to satisfy simultaneously your obligations under this License and any other pertinent obligations, then as a consequence you may not distribute the Program at all. For example, if a patent license would not permit royalty-free redistribution of the Program by all those who receive copies directly or indirectly through you, then the only way you could satisfy both it and this License would be to refrain entirely from distribution of the Program.\n"
			"If any portion of this section is held invalid or unenforceable under any particular circumstance, the balance of the section is intended to apply and the section as a whole is intended to apply in other circumstances.\n"
			"It is not the purpose of this section to induce you to infringe any patents or other property right claims or to contest validity of any such claims; this section has the sole purpose of protecting the integrity of the free software distribution system, which is implemented by public license practices. Many people have made generous contributions to the wide range of software distributed through that system in reliance on consistent application of that system; it is up to the author/donor to decide if he or she is willing to distribute software through any other system and a licensee cannot impose that choice.\n"
			"This section is intended to make thoroughly clear what is believed to be a consequence of the rest of this License.\n"
			"8. If the distribution and/or use of the Program is restricted in certain countries either by patents or by copyrighted interfaces, the original copyright holder who places the Program under this License may add an explicit geographical distribution limitation excluding those countries, so that distribution is permitted only in or among countries not thus excluded. In such case, this License incorporates the limitation as if written in the body of this License.\n"
			"9. The Free Software Foundation may publish revised and/or new versions of the General Public License from time to time. Such new versions will be similar in spirit to the present version, but may differ in detail to address new problems or concerns.\n"
			"Each version is given a distinguishing version number. If the Program specifies a version number of this License which applies to it and \"any later version\", you have the option of following the terms and conditions either of that version or of any later version published by the Free Software Foundation. If the Program does not specify a version number of this License, you may choose any version ever published by the Free Software Foundation.\n"
			"10. If you wish to incorporate parts of the Program into other free programs whose distribution conditions are different, write to the author to ask for permission. For software which is copyrighted by the Free Software Foundation, write to the Free Software Foundation; we sometimes make exceptions for this. Our decision will be guided by the two goals of preserving the free status of all derivatives of our free software and of promoting the sharing and reuse of software generally.\n"
			"NO WARRANTY\n"
			"11. BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW. EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM \"AS IS\" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU. SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR CORRECTION.\n"
			"12. IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY AND/OR REDISTRIBUTE THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES, INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED TO LOSS OF DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER PROGRAMS), EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.\n"
			"END OF TERMS AND CONDITIONS\n");
		system("pause");
	}
}