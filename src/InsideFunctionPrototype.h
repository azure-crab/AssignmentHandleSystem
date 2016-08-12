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

//本头文件内包含内部函数的原型以及一些宏定义
#pragma once
#include "systemapi.h"
#include "DataStructure.h"

//用于清除该行剩余输入
#define DISCARD_REST_LINE() while(getchar()!='\n') continue

#pragma region 录入模块部分

#pragma region 载入存档及其子函数部分
//载入存档的驱动函数
//接收参数：指向CourseNode的指针的地址和指向StudentNode的指针的地址
static void  LoadFile			(SystemData systemData);

//载入课程数据并构建课程链表
static void  LoadCoursesData	(SystemData systemData);

//载入学生数据并构建学生链表
static void  LoadStudentsData	(SystemData systemData);

//载入课程作业数据并构建每次作业的链表
static void  LoadAssignmentData	(SystemData systemData);

//载入作业成绩记录数据并完成多重表的构建
static void  LoadRecordData		(SystemData systemData);
#pragma endregion

#pragma region 添加课程及其子函数部分
//添加课程的驱动函数
//接收参数：课程链表和学生链表
//包括查错
static void AddCourse(SystemData systemData);

//创建课程并插入课程链表中
//返回指向对应课程结点的指针
static CourseNode* AddCourse_Util(SystemData systemData, 
									int courseId, char* courseName);

#pragma endregion

#pragma region 向课程中添加学生
//向课程中添加学生的驱动程序
static void AddStudentIntoCourse(SystemData systemData);

//创建学生结点并插入课程结点的学生链表中
//并修改学生信息节点中参加课程的记录
//返回指向对应学生结点的指针
static StudentNodeInCourse* AddStudentIntoCourse_Util(CourseNode* course,
													StudentNode* student);
//创建对应的作业成绩记录结点并插入多重表中
static void AddStudentIntoCourse_RecordInput(CourseNode* course,
					StudentNodeInCourse* studentNodeInCourse);
#pragma endregion

#pragma region 向课程中添加作业
//添加作业的驱动函数
//包括查错
static void AddAssignment(SystemData systemData);
//创建作业结点并插入课程的作业链表中
//返回指向对应作业结点的指针
static AssignmentNode* AddAssignment_Util(CourseNode* course,
							int assignmentId, char* assignmentName);
//创建对应的作业成绩记录结点并插入多重表中
static void AddAssignment_RecordInput(CourseNode* course,
							AssignmentNode* assignment);
#pragma endregion

#pragma region 添加学生及其子函数部分
//添加学生的驱动程序
//包括查错
static void AddStudent(SystemData systemData);

//创建学生结点并插入学生信息链表
//返回指向对应学生结点的指针
//不进行学生作业成绩录入
static StudentNode* AddStudent_Util(SystemData systemData, 
							int studentId, char* studentName);


#pragma endregion

#pragma endregion 

#pragma region 输出模块部分

#pragma region 作业成绩部分
//输出单个成绩数据
static Record* PrintSpecificRecord(StudentNodeInCourse* student,
								AssignmentNode* assignment);

//输出指定学生的作业成绩链表内的所有成绩数据
static void PrintAllRecordOfSpecificStudentInCourse(Record* RecordEntryOfStudent);

//输出指定作业的作业成绩链表内的所有成绩数据
static void PrintAllRecordOfSpecificAssignment(Record* RecordEntryOfAssignment);

#pragma endregion 

#pragma region 各项信息部分
//输出所有学生的信息
static void PrintAllStudentsInfo(StudentNode* headOfStudents);

//输出所有课程的信息
static void PrintAllCoursesInfo(CourseNode* headOfCourses);

//输出指定课程内所有学生信息
static void PrintAllStudentsInCourse(StudentNodeInCourse* headOfStudents);

//输出指定课程内所有作业信息
static void PrintAllAssignmentsInCourse(AssignmentNode* headOfAssignments);

//输出指定学生参加的所有课程信息
static void PrintAllCoursesAStudentAttended(
				CourseNodeInStudentInfo* headOfCoursesInStudentInfo);

//输出指定学生没有参加的课程的信息
static void PrintCoursesAStudentNotAttended(CourseNode* headOfCourses, 
				CourseNodeInStudentInfo* headOfCoursesNodeInStudentInfo);

//输出不在指定课程内的学生的信息
static void PrintStudentsNotInACourse(StudentNode* headOfStudents,
						StudentNodeInCourse* headOfStudentsInCoursesInfo);

//输出指定的作业成绩总计信息
static void PrintHandInsInfo(const HandInsInfo* handInsInfo);
#pragma endregion 

#pragma endregion 

#pragma region 查询模块部分

#pragma region 多重查询部分
//多重查询部分驱动函数
static void MultipleQuery(SystemData systemdata);

//查询指定课程内所有作业成绩数据
//进行课程信息输出，查询的课程Id的查错工作
//询问主序，最后根据主序调用输出函数输出
static void MultipleQuery_AllRecordInCourse(CourseNode* headOfCourses);

//查询指定学生在所有参加课程内的作业成绩数据
//进行学生信息输出，查询的学生Id的查错工作
static void MultipleQuery_StudentRecordInEveryCourse(StudentNode* headOfStudents);

//查询指定学生在参加的任一课程内的作业成绩数据
//相关信息输出，Id查错
static void MultipleQuery_StudentRecordInSpecificCourse(StudentNode* headOfStudents);

//查询指定课程内任一学生的作业成绩数据
//相关信息输出，Id查错
static void MultipleQuery_StudentRecordOfOneCourse(CourseNode* headOfCourses);

//查询指定课程内任一作业的全部作业成绩数据
static void MultipleQuery_AssignmentRecordOfOneCourse(CourseNode* headOfCourses);
#pragma endregion 

#pragma region 精确查询部分
//精确查询的函数
//输出相关信息，查错，调用输出函数
static void SpecificQuery(SystemData systemdata);
#pragma endregion 

#pragma region 查询总计信息部分
//查询总计信息的驱动函数
static void HandInsQuery(SystemData systemData);

//查询指定课程的总计信息
static void HandInsInfoQuery_CourseHandIns(CourseNode* headOfCourses);

//查询指定学生在所有课程的总计信息
static void HandInsInfoQuery_StudentHandIns(StudentNode* headOfStudents);

//查询指定学生在参加的任一课程内的总计信息
static void HandInsInfoQuery_StudentInSpecificCourse(StudentNode* headOfStudents);

//查询指定的课程中任一学生的总计信息
static void HandInsInfoQuery_StudentOfOneCourse(CourseNode* headOfCourses);

//查询指定的课程中任一作业的总计信息
static void HandInsInfoQuery_AssignmentOfOneCourse(CourseNode* headOfCourses);
#pragma endregion 

#pragma endregion 

#pragma region 修改模块部分
//修改课程信息
static void ModifyCourseInfo(SystemData systemData);

//修改作业信息
static void ModifyAssignmentInfo(SystemData systemData);

//修改学生信息
static void ModifyStudentInfo(SystemData systemData);

//修改学生参加课程信息
static void ModifyStudentInfo_AttendedCourses(CourseNode* headOfCourses,
												StudentNode* student);

//修改一项作业成绩信息
static void ModifyRecord(SystemData systemData);

#pragma region 删除函数
//删除指定课程
static void DeleteCourse(SystemData systemData, CourseNode* course);

//删除指定作业
static void DeleteAssignment(CourseNode* course, AssignmentNode* assignment);

//删除指定学生
static void DeleteStudent(SystemData systemData, StudentNode* student);

//将学生从某课程中移除
//并更新学生信息
static void RemoveStudentFromSpecificCourse(StudentNode* student, CourseNode* course);

//将课程移出某学生的参加课程链表
//仅删除对应的课程结点，不修改课程中的学生链表
//刷新学生作业成绩总计信息
static void RemoveCourseFromStudentInfo(StudentNode* student, CourseNode* course);
#pragma endregion 
#pragma endregion 

#pragma region 保存模块部分
//保存课程信息
static void SaveMode_Course(CourseNode* headOfCourses);

//保存学生信息
static void SaveMode_Student(StudentNode* headOfStudents);

//保存作业信息
static void SaveMode_Assignment(CourseNode* headOfCourses);

//保存作业成绩数据
static void SaveMode_Record(CourseNode* headOfCourses);
#pragma endregion 

#pragma region 辅助函数部分

#pragma region 重新计算作业成绩总计信息
//接收参数：指向需要重新计算的结点的指针

//对应课程中学生结点
static void RefreshStudentInCourseHandInsInfo(
			StudentNodeInCourse* student_in_co_node_in_course);

//对应课程中作业结点
static void RefreshAssignmentHandInsInfo(
			AssignmentNode* assignment);

//对应课程结点
//注意：调用时请保证学生和作业结点的作业成绩总计信息都是最新且正确的
//错误检查：将利用学生结点和作业结点分别计算总计信息并比对
//			若不同则调用函数将它们全部重新计算
static void RefreshCourseHandInsInfo(CourseNode* course);

//对应学生结点
//注意：调用时请保证学生在所有课程中的结点的
//		作业成绩总计信息都是最新且正确的
static void RefreshStudentHandInsInfo(StudentNode* student);
#pragma endregion

#pragma region 在链表中根据Id寻找对应结点
//参数：链表，Id
//返回：未找到则返回NULL

//对应学生信息链表
static StudentNode* FindStudentNode(StudentNode* headOfStudents,
									long studentId);

//对应课程链表
static CourseNode*  FindCourseNode(CourseNode* headOfCourses,
									long courseId);

//对应课程结点中学生链表
static StudentNodeInCourse* FindStudentNodeInCourse(
				StudentNodeInCourse* headOfStudentsInCourse,
				long studentId);

//对应课程结点中作业链表
static AssignmentNode* FindAssignmentNode(
				AssignmentNode* headOfAssignments,
				long assignmentId);

//对应学生信息中已参加课程链表
static CourseNode* FindAttendedCourseInStudentInfo(
				CourseNodeInStudentInfo* headOfCoursesInStuInfo,
				long courseId);

//对应作业结点的记录入口，沿学生方向遍历
static Record* FindRecordOfSpecificStudent(
				Record* RecordEntryOfAssignment,
				long studentId);

//对应学生结点的记录入口，沿作业方向遍历
static Record* FindRecordOfSpecificAssignment(Record* RecordEntryOfStudent,
											long assignmentId);
#pragma endregion

#pragma region 输出相关信息后请求输入并在输入合法时返回指针
//返回值：若用户取消则返回NULL

//对于所有课程
static CourseNode* GetSelectedCourse(CourseNode* headOfCourses);

//对于所有学生
static StudentNode* GetSelectedStudent(StudentNode* headOfStudents);

//对于课程内所有作业
static AssignmentNode* GetSelectedAssignment(AssignmentNode* headOfAssignments);

//对于课程内所有学生
static StudentNodeInCourse* GetSelectedStudentInCourse(
								StudentNodeInCourse* headOfStudentsNodeInCourse);

//对于学生参加的所有课程
static CourseNode* GetSelectedCourseInStudentInfo(
							CourseNodeInStudentInfo* headOfCoursesInStudentInfo);

#pragma endregion

#pragma region 其他
//读取用户的选择
//检查合法性并在非法时重新请求输入
//参数：选择序号的最大值和最小值
//返回值：用户选择的项的序号
long GetChoice(long bottom, long top);
#pragma endregion

#pragma endregion