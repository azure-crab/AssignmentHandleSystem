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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SystemAPI.h"
#include "DataStructure.h"
#include "InsideFunctionPrototype.h"
#include "cJSON.h"
#define MAX_JSON_BUFFER_LENGTH SHRT_MAX
#define TESTING 0
//TODO：使用栈来加速检查Id是否合法

#pragma region 系统API
SystemData Initialize(void)
{
	SystemData systemData = (SystemData)malloc(sizeof(struct System_Data));
	if (systemData == NULL)
	{
		printf("Error: fail to initialize system.\n");
		exit(EXIT_FAILURE);
	}
	systemData->headOfCourses  = NULL;
	systemData->headOfStudents = NULL;
	return systemData;
}
void RecordInputMode(SystemData systemData)
{
	long choice;
	for (;;) {
		system("cls");
		printf("录入模块\n\n");
		printf("请选择一个选项：\n"
			"1：载入存档\t\t2：添加课程\n"
			"3：向课程中添加学生\t4：向课程中添加作业\n"
			"5：添加学生\n"
			"0：返回主选单\n"
			"输入数字（0-5）并回车来选择一个选项：");
		choice = GetChoice(0, 5);
		switch (choice) {
		case 1: LoadFile(systemData);				break;
		case 2: AddCourse(systemData);				break;
		case 3: AddStudentIntoCourse(systemData);	break;
		case 4: AddAssignment(systemData);			break;
		case 5: AddStudent(systemData);				break;
		case 0: return;
		}
	}
}
void QueryMode(SystemData systemData)
{
	long choice;
	for (;;) {
		system("cls");
		printf("查询模块\n\n");
		printf("请选择一个选项：\n"
			"1：多重查询\t2：精准查询\t3：查询总计信息\n"
			"0：返回主选单\n"
			"输入数字（0-3）并回车来选择一个选项：");
		choice = GetChoice(0, 3);
		switch (choice) {
		case 1: MultipleQuery(systemData);	break;
		case 2: SpecificQuery(systemData);	break;
		case 3: HandInsQuery(systemData);	break;
		case 0: return;
		}
	}
}
void ModifyMode(SystemData systemData)
{
	long choice;
	for (;;) {
		system("cls");
		printf("修改模块\n\n");
		printf("请选择一个选项：\n"
			"1：修改课程信息\t2：修改作业信息\t3：修改学生信息\n"
			"4：修改一项作业成绩数据\n"
			"0：返回主选单\n"
			"输入数字（0-4）并回车来选择一个选项：");
		choice = GetChoice(0, 4);
		switch (choice) {
		case 1: ModifyCourseInfo(systemData);		break;
		case 2: ModifyAssignmentInfo(systemData);	break;
		case 3: ModifyStudentInfo(systemData);		break;
		case 4: ModifyRecord(systemData);			break;
		case 0: return;
		}
	}
}
void SaveMode(SystemData systemData)
{
	printf("开始存档……\n");	
	SaveMode_Course(systemData->headOfCourses);
	printf("已完成对课程信息数据的存档……\n");
	SaveMode_Student(systemData->headOfStudents);
	printf("已完成对学生信息数据的存档……\n");
	SaveMode_Assignment(systemData->headOfCourses);
	printf("已完成对课程内各次作业数据的存档……\n");
	SaveMode_Record(systemData->headOfCourses);
	printf("存档已完成。\n\n");
	system("pause");
}
#pragma endregion

#pragma region 驱动函数部分
static void LoadFile(SystemData systemData)
{
	if (systemData->headOfCourses != NULL || systemData->headOfStudents != NULL)
	{
		printf("当前系统内已有数据了，这种情况下不支持载入存档。\n");
		system("pause");
		return;
	}
	printf("开始读取存档……\n");
	LoadCoursesData(systemData);
	printf("已完成对课程信息数据的读取……\n");
	LoadStudentsData(systemData);
	printf("已完成对学生信息数据的读取……\n");
	LoadAssignmentData(systemData);
	printf("已完成对课程内各次作业数据的读取……\n");
	LoadRecordData(systemData);
	printf("读取已完成。\n\n");
	system("pause");
}
static void AddCourse(SystemData systemData)
{
	long courseId;
	char courseName[MAX_COURSE_NAME_LENGTH] = { 0 };
	//读取Id并进行重复性检查
	//TODO：改为使用栈来加速检查Id是否合法
	printf("请输入要添加的课程的Id（正整数）并回车，或输入0并回车来取消：");
	for (;;)
	{
		courseId = GetChoice(0, LONG_MAX);
		if (courseId == 0) return;
		if (FindCourseNode(systemData->headOfCourses, courseId)) {
			printf("请不要输入与以下课程重复的Id（正整数）：\n");
			PrintAllCoursesInfo(systemData->headOfCourses);
			printf("请重新输入Id（正整数）并回车：");
		}
		else break;
	}

	//读取课程名称
	printf("请输入要添加的课程的名称并回车。其中不能包含空格，否则空格之后的将被忽略：\n");
	//不必检查输入是否为空，因为scanf会自动跳过空白符，比如换行符
	scanf_s("%s", &courseName, MAX_COURSE_NAME_LENGTH - 1);
	DISCARD_REST_LINE();

	AddCourse_Util(systemData, courseId, courseName);
	printf("已完成课程的添加。\n");
	system("pause");
}
static void AddStudentIntoCourse(SystemData systemData)
{
	printf("请在下列课程中选择你要向其中添加学生的课程：\n");
	CourseNode* course = GetSelectedCourse(systemData->headOfCourses);
	//用户取消了选择
	if (course == NULL) return;

	//TODO:改为利用栈来加快确定学生是否符合要求
	//循环获取需要添加的学生的Id
	for(;;)
	{
		printf("请在下列学生中选择要向该课程中添加的学生：\n");
		PrintStudentsNotInACourse(systemData->headOfStudents, course->studentList);
		printf("输入学生的Id并回车来选择该生，或输入0并回车来取消：");
		long chosenStuId = GetChoice(0, LONG_MAX);
		//判断用户是否选择了取消
		if (chosenStuId == 0) return;

		StudentNode* chosenStudent;
		//判断输入是否合法：未参加该课程的学生
		if ((chosenStudent = FindStudentNode(systemData->headOfStudents, chosenStuId)))
			if (!FindAttendedCourseInStudentInfo(chosenStudent->StudentInfo.courseList,
													course->courseInfo.courseId))
			{
				StudentNodeInCourse* stuInCour = AddStudentIntoCourse_Util(course, chosenStudent);
				AddStudentIntoCourse_RecordInput(course, stuInCour);
				printf("已将学生加入该课程。\n");
			}
			else printf("你输入了一个已参加该课程的学生的Id，此次请求将被忽略\n");
		else printf("你输入的学生的Id在系统中并不存在，此次请求将被忽略\n");
	}
}
static void AddAssignment(SystemData systemData)
{
	printf("请在下列课程中选择你要向其中添加作业的课程：\n");
	CourseNode* course = GetSelectedCourse(systemData->headOfCourses);
	//用户取消了选择
	if (course == NULL) return;

	long assignmentId;
	char assignmentName[MAX_ASSIGNMENT_NAME_LENGTH] = { 0 };
	//TODO:改为使用栈来加速检查Id是否合法
	//读取Id并进行重复性检查
	printf("请输入要添加的作业的Id（正整数）并回车，或输入0并回车来取消：");
	for (;;)
	{
		assignmentId = GetChoice(0, LONG_MAX);
		if (assignmentId == 0) return;
		if (FindAssignmentNode(course->assignmentList, assignmentId)) {
			printf("请不要输入与以下作业重复的Id（正整数）：\n");
			PrintAllAssignmentsInCourse(course->assignmentList);
			printf("请重新输入Id（正整数）并回车，或输入0并回车来取消：");
		}
		else break;
	}

	//读取作业名称
	printf("请输入要添加的作业的名称并回车。其中不能包含空格，否则空格之后的将被忽略：\n");
	scanf_s("%s", &assignmentName, MAX_ASSIGNMENT_NAME_LENGTH - 1);
	DISCARD_REST_LINE();

	AssignmentNode* assignment = AddAssignment_Util(course, assignmentId, assignmentName);
	AddAssignment_RecordInput(course, assignment);
	printf("已将作业加入该课程。\n");
	system("pause");
}
static void AddStudent(SystemData systemData)
{
	long studentId;
	char studentName[MAX_STUDENT_NAME_LENGTH] = { 0 };
	//TODO:改为使用栈来加速检查Id是否合法
	//读取Id并进行重复性检查
	printf("请输入要添加的学生的Id（正整数）并回车，或输入0并回车来取消：");
	for (;;)
	{
		studentId = GetChoice(0, LONG_MAX);
		if (studentId == 0) return;
		if (FindStudentNode(systemData->headOfStudents, studentId)) {
			printf("请不要输入与以下学生重复的Id（正整数）：\n");
			PrintAllStudentsInfo(systemData->headOfStudents);
			printf("请重新输入Id（正整数）：");
		}
		else break;
	}

	//读取学生名称
	printf("请输入要添加的学生的名称并回车。其中不能包含空格，否则空格之后的将被忽略：\n");
	scanf_s("%s", &studentName, MAX_STUDENT_NAME_LENGTH - 1);
	DISCARD_REST_LINE();

	AddStudent_Util(systemData, studentId, studentName);
	printf("已完成学生的添加。\n");
	system("pause");
}
static void MultipleQuery(SystemData systemData) {
	long choice;
	for (;;) {
		printf("请选择一个选项：\n"
			"1：查询所有课程信息\t2：查询所有学生信息\n"
			"3：查询指定课程内所有学生信息\t4：查询指定课程内所有作业信息\t5：查询指定课程内所有作业成绩数据\n"
			"6：查询指定学生在所有课程内的作业成绩数据\t7：查询指定学生在参加的任一课程内的作业成绩数据\n"
			"8：查询指定课程内的指定学生作业成绩数据\t\t9：查询指定课程内指定作业的全部作业成绩数据\n"
			"0：回到查询模块的选单\n"
			"输入数字（0-9）并回车来选择一个选项：");
		choice = GetChoice(0, 9);
		CourseNode* course;
		switch (choice) {
		case 1: 
			printf("以下是系统内所有课程的信息：\n");
			PrintAllCoursesInfo(systemData->headOfCourses);
			system("pause");
			break;
		case 2:
			printf("以下是系统内所有学生的信息：\n");
			PrintAllStudentsInfo(systemData->headOfStudents);
			system("pause");
			break;
		case 3:
			printf("请在下列课程中选择你要查询的课程。\n");
			course = GetSelectedCourse(systemData->headOfCourses);
			if (course == NULL) break;
			PrintAllStudentsInCourse(course->studentList);
			system("pause");
			break;
		case 4:
			printf("请在下列课程中选择你要查询的课程。\n");
			course = GetSelectedCourse(systemData->headOfCourses);
			if (course == NULL) break;
			PrintAllAssignmentsInCourse(course->assignmentList);
			system("pause");
			break;
		case 5:
			MultipleQuery_AllRecordInCourse(systemData->headOfCourses);
			break;
		case 6:
			MultipleQuery_StudentRecordInEveryCourse(systemData->headOfStudents);
			break;
		case 7:
			MultipleQuery_StudentRecordInSpecificCourse(systemData->headOfStudents);
			break;
		case 8:
			MultipleQuery_StudentRecordOfOneCourse(systemData->headOfCourses);
			break;
		case 9:
			MultipleQuery_AssignmentRecordOfOneCourse(systemData->headOfCourses);
			break;
		case 0:
			return;
		}
	}
}
static void SpecificQuery(SystemData systemData)
{
	printf("请在下列课程中选择你要查询的记录对应的课程：\n");
	CourseNode* course = GetSelectedCourse(systemData->headOfCourses);
	if (course == NULL) return;

	printf("请在下列作业中选择你要查询的记录对应的作业：\n");
	AssignmentNode* assignment = GetSelectedAssignment(course->assignmentList);
	if (assignment == NULL) return;

	printf("请在下列学生中选择你要查询的记录对应的学生：\n");
	StudentNodeInCourse* student = GetSelectedStudentInCourse(course->studentList);
	if (student == NULL) return;

	printf("查询到的记录如下：\n");
	printf("学生%s(%ld)在课程\"%s\"(%ld)内作业\"%s\"(%ld)的成绩为：",
		student->studentNode->StudentInfo.studentName, student->studentNode->StudentInfo.studentId,
		course->courseInfo.CourseName, course->courseInfo.courseId,
		assignment->assignmentInfo.assignmentName, assignment->assignmentInfo.assignmentId);
	PrintSpecificRecord(student, assignment);
	system("pause");
}
static void HandInsQuery(SystemData systemData)
{
	long choice;
	for(;;)	{
		printf("请选择一个选项：\n"
			"1：查询指定课程的作业成绩总计信息\n"
			"2：查询指定学生在参加的所有课程的作业成绩总计信息\n"
			"3：查询指定学生在参加的任一课程内的作业成绩总计信息\n"
			"4：查询指定课程内任一学生的作业成绩总计信息\n"
			"5：查询指定课程内任一作业的作业成绩总计信息\n"
			"0：回到查询模块的选单\n"
			"输入数字（0-5）并回车来选择一个选项：");
		choice = GetChoice(0, 5);
		switch (choice) {
		case 1: HandInsInfoQuery_CourseHandIns(systemData->headOfCourses);				break;
		case 2: HandInsInfoQuery_StudentHandIns(systemData->headOfStudents);			break;
		case 3: HandInsInfoQuery_StudentInSpecificCourse(systemData->headOfStudents);	break;
		case 4: HandInsInfoQuery_StudentOfOneCourse(systemData->headOfCourses);			break;
		case 5: HandInsInfoQuery_AssignmentOfOneCourse(systemData->headOfCourses);		break;
		case 0: return;
		}
	}
}
#pragma endregion

#pragma region 辅助函数部分

#pragma region 重新计算作业成绩总计信息
static void RefreshStudentInCourseHandInsInfo(StudentNodeInCourse* studentInCourse)
{
	studentInCourse->handInsInfo.expectedHandInCount = 0;
	studentInCourse->handInsInfo.actualHandInCount   = 0;
	studentInCourse->handInsInfo.goodHandInCount     = 0;
	for (Record* curPtr = studentInCourse->recordEntry; curPtr != NULL;
		curPtr = curPtr->nextAssigment)
	{
		studentInCourse->handInsInfo.expectedHandInCount++;
		if (curPtr->score > 0)
		{
			studentInCourse->handInsInfo.actualHandInCount++;
			if (curPtr->score >= 90)
				studentInCourse->handInsInfo.goodHandInCount++;
		}
	}
}
static void RefreshAssignmentHandInsInfo(AssignmentNode* assignment)
{
	
	assignment->handInsInfo.expectedHandInCount = 0;
	assignment->handInsInfo.actualHandInCount	= 0;
	assignment->handInsInfo.goodHandInCount		= 0;
	for (Record* curPtr = assignment->recordEntry; curPtr != NULL;
		curPtr = curPtr->nextStudent)
	{
		assignment->handInsInfo.expectedHandInCount++;
		if (curPtr->score > 0)
		{
			assignment->handInsInfo.actualHandInCount++;
			if (curPtr->score >= 90)
				assignment->handInsInfo.goodHandInCount++;
		}
	}
}
static void RefreshCourseHandInsInfo(CourseNode* course)
{
	int expectedHandInCount_Student = 0;
	int actualHandInCount_Student	= 0;
	int goodHandInCount_Student		= 0;
	int expectedHandInCount_Assignment = 0;
	int actualHandInCount_Assignment   = 0;
	int goodHandInCount_Assignment     = 0;

	//通过学生结点的总计信息计算
	for (StudentNodeInCourse* student = course->studentList
		; student != NULL; student = student->nextStudent)
	{
		expectedHandInCount_Student += student->handInsInfo.expectedHandInCount;
		actualHandInCount_Student   += student->handInsInfo.actualHandInCount;
		goodHandInCount_Student     += student->handInsInfo.goodHandInCount;
	}

	//通过作业结点的总计信息计算
	for (AssignmentNode* assignment = course->assignmentList; 
		assignment != NULL; assignment = assignment->nextAssigment)
	{
		expectedHandInCount_Assignment += assignment->handInsInfo.expectedHandInCount;
		actualHandInCount_Assignment   += assignment->handInsInfo.actualHandInCount;
		goodHandInCount_Assignment     += assignment->handInsInfo.goodHandInCount;
	}

	//比较两种方法计算出的结果是否相同，相同说明没有问题
	if(expectedHandInCount_Student == expectedHandInCount_Assignment &&
		actualHandInCount_Student  == actualHandInCount_Assignment &&
		goodHandInCount_Student    == goodHandInCount_Assignment)
	{
		course->handInsInfo.expectedHandInCount = expectedHandInCount_Student;
		course->handInsInfo.actualHandInCount   = actualHandInCount_Student;
		course->handInsInfo.goodHandInCount     = goodHandInCount_Student;
	}
	//否则说明各结点的handInsInfo不是最新的，刷新每个结点的handInsInfo
	//同时计算课程正确的handInsInfo
	else
	{
		course->handInsInfo.expectedHandInCount = 0;
		course->handInsInfo.actualHandInCount	= 0;
		course->handInsInfo.goodHandInCount		= 0;
		for (StudentNodeInCourse* student = course->studentList; 
			student != NULL; student = student->nextStudent)
		{
			RefreshStudentInCourseHandInsInfo(student);
		}
		for (AssignmentNode* assignment = course->assignmentList; 
			assignment != NULL; assignment = assignment->nextAssigment)
		{
			RefreshAssignmentHandInsInfo(assignment);
			course->handInsInfo.expectedHandInCount += assignment->handInsInfo.expectedHandInCount;
			course->handInsInfo.actualHandInCount   += assignment->handInsInfo.actualHandInCount;
			course->handInsInfo.goodHandInCount     += assignment->handInsInfo.goodHandInCount;
		}
	}
}
static void RefreshStudentHandInsInfo(StudentNode* student)
{
	student->StudentInfo.handInsInfo.expectedHandInCount = 0;
	student->StudentInfo.handInsInfo.actualHandInCount   = 0;
	student->StudentInfo.handInsInfo.goodHandInCount     = 0;
	for (CourseNodeInStudentInfo* course = student->StudentInfo.courseList;
		course != NULL; course = course->next)
	{
		student->StudentInfo.handInsInfo.expectedHandInCount +=
			FindStudentNodeInCourse(course->courseNode->studentList,
				student->StudentInfo.studentId)->handInsInfo.expectedHandInCount;
		student->StudentInfo.handInsInfo.actualHandInCount += 
			FindStudentNodeInCourse(course->courseNode->studentList,
				student->StudentInfo.studentId)->handInsInfo.actualHandInCount;
		student->StudentInfo.handInsInfo.goodHandInCount += 
			FindStudentNodeInCourse(course->courseNode->studentList,
				student->StudentInfo.studentId)->handInsInfo.goodHandInCount;
	}
}
#pragma endregion 

#pragma region 在链表中根据Id寻找对应结点
static StudentNode* FindStudentNode(StudentNode* headOfStudents,
									long studentId)
{
	StudentNode* curPtr = headOfStudents;
	for (; curPtr != NULL; curPtr = curPtr->next)
		if (curPtr->StudentInfo.studentId == studentId)
			break;
	return curPtr;
}
static CourseNode* FindCourseNode(CourseNode* headOfCourses,
									long courseId)
{
	CourseNode* curPtr = headOfCourses;
	for (; curPtr != NULL; curPtr = curPtr->next)
		if (curPtr->courseInfo.courseId == courseId)
			break;
	return curPtr;
}
static StudentNodeInCourse* FindStudentNodeInCourse(
			StudentNodeInCourse* headOfStudentsInCourse,
			long studentId)
{
	StudentNodeInCourse* curPtr = headOfStudentsInCourse;
	for (; curPtr != NULL; curPtr = curPtr->nextStudent)
		if (curPtr->studentNode->StudentInfo.studentId == studentId)
			break;
	return curPtr;
}
static AssignmentNode* FindAssignmentNode(
			AssignmentNode* headOfAssignments,
			long assignmentId)
{
	AssignmentNode* curPtr = headOfAssignments;
	for (; curPtr != NULL; curPtr = curPtr->nextAssigment)
		if (curPtr->assignmentInfo.assignmentId == assignmentId)
			break;
	return curPtr;
}
static CourseNode* FindAttendedCourseInStudentInfo(
			CourseNodeInStudentInfo* headOfCoursesInStuInfo,
			long courseId)
{
	CourseNodeInStudentInfo* curPtr = headOfCoursesInStuInfo;
	for (; curPtr != NULL; curPtr=curPtr->next)
		if (curPtr->courseNode->courseInfo.courseId == courseId)
			return curPtr->courseNode;
	return NULL;
}
static Record* FindRecordOfSpecificStudent(
			Record* RecordEntryOfAssignment,
			long studentId)
{
	Record* curPtr = RecordEntryOfAssignment;
	for (; curPtr != NULL; curPtr = curPtr->nextStudent)
		if (curPtr->studentId == studentId)
			break;
	return curPtr;
}
static Record* FindRecordOfSpecificAssignment(Record* RecordEntryOfStudent,
											long assignmentId)
{
	Record* curPtr = RecordEntryOfStudent;
	for (; curPtr != NULL; curPtr = curPtr->nextAssigment)
		if (curPtr->assignmentId == assignmentId)
			break;
	return curPtr;
}
#pragma endregion 

#pragma region 输出相关信息后请求输入并在输入合法时返回指针
//TODO:	全部改为使用栈来加快确定输入的Id是否符合要求
static CourseNode* GetSelectedCourse(CourseNode* headOfCourse)
{
	if (headOfCourse == NULL)
	{
		printf("\t无任何数据，本次操作自动取消。\n");
		system("pause");
		return NULL;
	}
	//遍历一遍输出所有课程
	PrintAllCoursesInfo(headOfCourse);
	printf("请输入你要选择的课程的Id并回车来选择该课程，或输入0并回车来取消：");
	
	for (;;)
	{
		long courseId = GetChoice(0, LONG_MAX);
		if (courseId == 0)
			return NULL;
		CourseNode* coPtr = FindCourseNode(headOfCourse, courseId);
		if (coPtr != NULL)
			return coPtr;
		printf("非法输入，请输入下列课程之一的Id并回车，或输入0并回车来取消：\n");
		PrintAllCoursesInfo(headOfCourse);
	}
}
static StudentNode* GetSelectedStudent(StudentNode* headOfStudents)
{
	if (headOfStudents == NULL)
	{
		printf("\t无任何数据，本次操作自动取消。\n");
		system("pause");
		return NULL;
	}
	PrintAllStudentsInfo(headOfStudents);
	printf("请输入你要选择的学生的Id并回车来选择该课程，或输入0并回车来取消：");

	for (;;)
	{
		long studentId = GetChoice(0, LONG_MAX);
		if (studentId == 0)
			return NULL;
		StudentNode* stuPtr = FindStudentNode(headOfStudents, studentId);
		if (stuPtr != NULL)
			return stuPtr;
		printf("非法输入，请输入下列学生之一的Id并回车，或输入0并回车来取消：\n");
		PrintAllStudentsInfo(headOfStudents);
	}
}
static AssignmentNode* GetSelectedAssignment(AssignmentNode* headOfAssignments)
{
	if (headOfAssignments == NULL)
	{
		printf("\t无任何数据，本次操作自动取消。\n");
		system("pause");
		return NULL;
	}
	PrintAllAssignmentsInCourse(headOfAssignments);
	printf("请输入你要选择的作业的Id并回车来选择该课程，或输入0并回车来取消：");

	for (;;)
	{
		long assignmentId = GetChoice(0, LONG_MAX);
		if (assignmentId == 0)
			return NULL;
		AssignmentNode* stuPtr = FindAssignmentNode(headOfAssignments, assignmentId);
		if (stuPtr != NULL)
			return stuPtr;
		printf("非法输入，请输入下列作业之一的Id并回车，或输入0并回车来取消：\n");
		PrintAllAssignmentsInCourse(headOfAssignments);
	}
}
static StudentNodeInCourse* GetSelectedStudentInCourse(
				StudentNodeInCourse* headOfStudentsNodeInCourse)
{
	if (headOfStudentsNodeInCourse == NULL)
	{
		printf("\t无任何数据，本次操作自动取消。\n");
		system("pause");
		return NULL;
	}
	PrintAllStudentsInCourse(headOfStudentsNodeInCourse);
	printf("请输入你要选择的作业的Id并回车来选择该课程，或输入0并回车来取消：");

	for (;;)
	{
		long studentId = GetChoice(0, LONG_MAX);
		if (studentId == 0)
			return NULL;
		StudentNodeInCourse* stuPtr = FindStudentNodeInCourse(
			headOfStudentsNodeInCourse, studentId);
		if (stuPtr != NULL)
			return stuPtr;
		printf("非法输入，请输入下列作业之一的Id并回车，或输入0并回车来取消：\n");
		PrintAllStudentsInCourse(headOfStudentsNodeInCourse);
	}
}
static CourseNode* GetSelectedCourseInStudentInfo(
				CourseNodeInStudentInfo* headOfCoursesInStudentInfo)
{
	if (headOfCoursesInStudentInfo == NULL)
	{
		printf("\t无任何数据，本次操作自动取消。\n");
		system("pause");
		return NULL;
	}
	PrintAllCoursesAStudentAttended(headOfCoursesInStudentInfo);
	printf("请输入你要选择的课程的Id并回车来选择该课程，或输入0并回车来取消：");

	for (;;)
	{
		long courseId = GetChoice(0, LONG_MAX);
		if (courseId == 0)
			return NULL;
		CourseNode* stuPtr = FindAttendedCourseInStudentInfo(
			headOfCoursesInStudentInfo, courseId);
		if (stuPtr != NULL)
			return stuPtr;
		printf("非法输入，请输入下列作业之一的Id并回车，或输入0并回车来取消：\n");
		PrintAllCoursesAStudentAttended(headOfCoursesInStudentInfo);
	}
}
#pragma endregion

#pragma region 其他
long GetChoice(long bottom, long top)
{
	char string[MAXINPUTLENGTH] = { 0 };
	char* checkPtr = NULL;	//接收转换结尾的字符的位置，用于检验输入合法性
	long choice;
	gets_s(string, MAXINPUTLENGTH - 1);
	choice = strtol(string, &checkPtr, 10);
	while (!(bottom <= choice && choice <= top) || *checkPtr != '\0')
	{
		printf("非法输入，请输入正确的数字（%ld-%ld）：", bottom, top);
		gets_s(string, MAXINPUTLENGTH - 1);
		choice = strtol(string, &checkPtr, 10);
	}
	return choice;
}
#pragma endregion 

#pragma endregion

#pragma region 输出模块部分
static Record* PrintSpecificRecord(StudentNodeInCourse* student, 
								AssignmentNode* assignment)
{
	Record* record = FindRecordOfSpecificAssignment(
							student->recordEntry, 
							assignment->assignmentInfo.assignmentId);
	printf("%d\n", record->score);
	return record;
}
static void PrintAllRecordOfSpecificStudentInCourse(Record* recordEntryOfStudent)
{
	if (recordEntryOfStudent == NULL)
	{
		printf("\t\t无任何数据\n");
		return;
	}
	printf("\t学生Id：%ld\n", recordEntryOfStudent->studentId);
	for (Record* curPtr = recordEntryOfStudent;
		curPtr != NULL; curPtr = curPtr->nextAssigment)
	{
		printf("\t\t作业Id：%ld\t成绩：%d\n",
			curPtr->assignmentId, curPtr->score);
	}
}
static void PrintAllRecordOfSpecificAssignment(Record* recordEntryOfAssignment)
{
	if (recordEntryOfAssignment == NULL)
	{
		printf("\t\t无任何数据\n");
		return;
	}
	printf("\t作业Id：%ld\n", recordEntryOfAssignment->assignmentId);
	for (Record* curPtr = recordEntryOfAssignment;
		curPtr != NULL; curPtr = curPtr->nextStudent)
	{
		printf("\t学生Id：%ld\t成绩：%d\n",
			curPtr->studentId, curPtr->score);
	}
}
static void PrintAllStudentsInfo(StudentNode* headOfStudents)
{
	if(headOfStudents == NULL)
	{
		printf("\t\t无任何数据\n");
		return;
	}
	StudentNode* curPtr = headOfStudents;
	for (; curPtr != NULL; curPtr = curPtr->next)
		printf("\t%s(%ld)\n", curPtr->StudentInfo.studentName,
			curPtr->StudentInfo.studentId);
}
static void PrintAllCoursesInfo(CourseNode* headOfCourses)
{
	if (headOfCourses == NULL)
	{
		printf("\t\t无任何数据\n");
		return;
	}
	CourseNode* curPtr = headOfCourses;
	for (; curPtr != NULL; curPtr = curPtr->next)
		printf("\t%s(%ld)\n", curPtr->courseInfo.CourseName,
			curPtr->courseInfo.courseId);
}
static void PrintAllStudentsInCourse(StudentNodeInCourse* headOfStudents)
{
	if (headOfStudents == NULL)
	{
		printf("\t\t无任何数据\n");
		return;
	}
	StudentNodeInCourse* curPtr = headOfStudents;
	for (; curPtr != NULL; curPtr = curPtr->nextStudent)
		printf("\t%s(%ld)\n", curPtr->studentNode->StudentInfo.studentName,
			curPtr->studentNode->StudentInfo.studentId);			
}
static void PrintAllAssignmentsInCourse(AssignmentNode* headOfAssignments)
{
	if (headOfAssignments == NULL)
	{
		printf("\t\t无任何数据\n");
		return;
	}
	AssignmentNode* curPtr = headOfAssignments;
	for (; curPtr != NULL; curPtr = curPtr->nextAssigment)
		printf("\t%s(%ld)\n", curPtr->assignmentInfo.assignmentName,
			curPtr->assignmentInfo.assignmentId);
}
static void PrintAllCoursesAStudentAttended(
	CourseNodeInStudentInfo* headOfCourses)
{
	if (headOfCourses == NULL)
	{
		printf("\t\t无任何数据\n");
		return;
	}
	CourseNodeInStudentInfo* curPtr = headOfCourses;
	for (; curPtr != NULL; curPtr = curPtr->next)
		printf("\t%s(%ld)\n", curPtr->courseNode->courseInfo.CourseName,
			curPtr->courseNode->courseInfo.courseId);
}
static void PrintCoursesAStudentNotAttended(CourseNode* headOfCourses,
						CourseNodeInStudentInfo* headOfCoursesNodeInStudentInfo)
{
	for(CourseNode* curCourseInfo = headOfCourses; 
		curCourseInfo != NULL; curCourseInfo = curCourseInfo->next)
	{
		//若当前课程不在学生已参加课程链表中
		if (!FindAttendedCourseInStudentInfo(headOfCoursesNodeInStudentInfo,
			curCourseInfo->courseInfo.courseId))
			printf("\t%s(%ld)\n", curCourseInfo->courseInfo.CourseName,
				curCourseInfo->courseInfo.courseId);
	}
}
static void PrintStudentsNotInACourse(StudentNode* headOfStudents,
						StudentNodeInCourse* headOfStudentsInCoursesInfo)
{
	for (StudentNode* curStuPtr = headOfStudents; 
		curStuPtr != NULL; curStuPtr = curStuPtr->next)
	{
		//若当前学生不在课程的学生链表中
		if (!FindStudentNodeInCourse(headOfStudentsInCoursesInfo,
			curStuPtr->StudentInfo.studentId))
			printf("\t%s(%ld)\n", curStuPtr->StudentInfo.studentName,
				curStuPtr->StudentInfo.studentId);
	}
}
static void PrintHandInsInfo(const HandInsInfo* handInsInfo)
{
	printf("\t应交作业总数：%d\n"
		"\t实交作业总数：%d\n"
		"\t上交率：%f\n"
		"\t优秀作业总数：%d\n"
		"\t优秀作业率：%f\n",
		handInsInfo->expectedHandInCount,
		handInsInfo->actualHandInCount,
		(float)handInsInfo->actualHandInCount / 
					handInsInfo->expectedHandInCount,
		handInsInfo->goodHandInCount,
		(float)handInsInfo->goodHandInCount / 
					handInsInfo->actualHandInCount);
}
#pragma endregion 

#pragma region 修改模块部分
static void ModifyCourseInfo(SystemData systemData)
{
	for (;;) {
		char courseName[MAX_COURSE_NAME_LENGTH] = { 0 };
		printf("请在下列课程中选择你要修改的课程。\n");
		CourseNode* course = GetSelectedCourse(systemData->headOfCourses);
		//检查用户是否取消了选择
		if (course == NULL)
			return;

		printf("\t课程\"%s\"(%ld)\n", course->courseInfo.CourseName,
			course->courseInfo.courseId);
		printf("请选择一个选项：\t1：修改课程名称\t2：删除课程\t0：取消\n"
				"输入数字（0-2）并回车来选择一个选项：");
		long choice = GetChoice(0, 2);

		switch (choice) {
		case 1:
			printf("请输入新的课程名并回车，名称中不能包含空格，否则空格之后的将被忽略：\n");
			scanf_s("%s", courseName, MAX_COURSE_NAME_LENGTH - 1);
			DISCARD_REST_LINE();
			strcpy_s(course->courseInfo.CourseName, 
							MAX_COURSE_NAME_LENGTH - 1, courseName);
			printf("对相应课程的修改已完成\n");
			system("pause");
			break;
		case 2:
			printf("确定要删除该课程的所有信息？（输入yes并回车来确定）");
			char buffer[MAXINPUTLENGTH] = { 0 };
			gets_s(buffer, MAXINPUTLENGTH - 1);
			if (!strcmp(buffer, "yes"))
				break;
			DeleteCourse(systemData, course);
			printf("已删除相应课程。\n");
			system("pause");
			break;
		case 0:
			return;
		}
	}
}
static void ModifyAssignmentInfo(SystemData systemData)
{
	for (;;) {
		char assignmentName[MAX_ASSIGNMENT_NAME_LENGTH] = { 0 };
		printf("请在下列课程中选择你要修改的作业所在的课程。\n");
		CourseNode* course = GetSelectedCourse(systemData->headOfCourses);
		//检查用户是否取消了选择
		if (course == NULL)
			return;

		printf("请在下列作业中选择你要修改的作业。\n");
		AssignmentNode* assignment = GetSelectedAssignment(course->assignmentList);
		//检查用户是否取消了选择
		if (assignment == NULL)
			return;

		printf("\t课程\"%s\"(%ld)中的作业\"%s\"(%ld)\n", course->courseInfo.CourseName,
			course->courseInfo.courseId, assignment->assignmentInfo.assignmentName,
			assignment->assignmentInfo.assignmentId);
		printf("请选择一个选项：\t1：修改作业名称\t2：删除作业\t0：取消\n"
				"输入数字（0-2）并回车来选择一个选项：");
		long choice = GetChoice(0, 2);

		switch (choice) {
		case 1:
			printf("请输入新的作业名并回车，名称中不能包含空格，否则空格之后的将被忽略：\n");
			scanf_s("%s", assignmentName, MAX_ASSIGNMENT_NAME_LENGTH - 1);
			DISCARD_REST_LINE();
			strcpy_s(assignment->assignmentInfo.assignmentName,
				MAX_ASSIGNMENT_NAME_LENGTH - 1, assignmentName);
			printf("对相应作业的修改已完成\n");
			system("pause");
			break;
		case 2:
			printf("确定要删除该作业的所有信息？（输入yes并回车来确定）");
			char buffer[MAXINPUTLENGTH] = { 0 };
			gets_s(buffer, MAXINPUTLENGTH - 1);
			if (!strcmp(buffer, "yes"))
				break;
			DeleteAssignment(course, assignment);
			printf("已删除相应作业。\n");
			system("pause");
			break;
		case 0:
			return;
		}
	}
}
static void ModifyStudentInfo(SystemData systemData)
{
	for (;;) {
		char studentName[MAX_STUDENT_NAME_LENGTH] = { 0 };
		printf("请在下列学生中选择你要修改的学生。\n");
		StudentNode* student = GetSelectedStudent(systemData->headOfStudents);
		//检查用户是否取消了选择
		if (student == NULL)
			return;

		printf("\t学生%s(%ld)\n", student->StudentInfo.studentName,
			student->StudentInfo.studentId);
		printf("请选择一个选项：\t1：修改学生名称\t2：修改参加的课程\t3：删除学生\t0：取消\n"
				"输入数字（0-3）并回车来选择一个选项：");
		long choice = GetChoice(0, 3);

		switch (choice) {
		case 1:
			printf("请输入新的学生名并回车，名称中不能包含空格，否则空格之后的将被忽略：\n");
			scanf_s("%s", studentName, MAX_STUDENT_NAME_LENGTH - 1);
			DISCARD_REST_LINE();
			strcpy_s(student->StudentInfo.studentName,
						MAX_STUDENT_NAME_LENGTH - 1, studentName);
			printf("对相应学生的修改已完成\n");
			system("pause");
			break;
		case 2:
			ModifyStudentInfo_AttendedCourses(systemData->headOfCourses, student);
			break;
		case 3:
			printf("确定要删除该学生的所有信息？（输入yes来确定）\n");
			char buffer[MAXINPUTLENGTH] = { 0 };
			gets_s(buffer, MAXINPUTLENGTH - 1);
			if (!strcmp(buffer, "yes"))
				break;
			DeleteStudent(systemData, student);
			printf("已删除相应课程。\n");
			system("pause");
			break;
		case 0: 
			return;
		}
	}
}
static void ModifyStudentInfo_AttendedCourses(CourseNode* headOfCourses,
	StudentNode* student)
{
	for (;;)
	{
		printf("学生%s(%ld)参加的课程如下：\n", student->StudentInfo.studentName,
			student->StudentInfo.studentId);
		PrintAllCoursesAStudentAttended(student->StudentInfo.courseList);
		printf("请选择一个选项：\t1：添加（系统内已有）课程\t2：删除课程\t0：取消\n"
				"输入数字（0-2）并回车来选择一个选项：");
		long choice = GetChoice(0, 2);
		switch (choice) {
		case 1:
			for (;;)
			{
				printf("请在下列课程中选择该生要参加的课程。\n");
				PrintCoursesAStudentNotAttended(headOfCourses, 
									student->StudentInfo.courseList);
				printf("输入课程的Id并回车来选择该课程，或输入0并回车来取消：\n");
				long chosenCourseId = GetChoice(0, LONG_MAX);
				//判断用户是否选择了取消
				if (chosenCourseId == 0) break;

				CourseNode* chosenCourse;
				//判断输入是否合法：该学生未参加的课程
				if ((chosenCourse = FindCourseNode(headOfCourses, chosenCourseId)))
					if (!FindStudentNodeInCourse(chosenCourse->studentList,
						student->StudentInfo.studentId))
					{
						StudentNodeInCourse* stuInCour = 
								AddStudentIntoCourse_Util(chosenCourse, student);
						AddStudentIntoCourse_RecordInput(chosenCourse, stuInCour);
					}
					else printf("你输入了一个该学生已参加的课程的Id，此次请求将被忽略\n");
				else printf("你输入的课程的Id在系统中并不存在，此次请求将被忽略\n");
			}
			printf("已完成相关修改。\n");
			system("pause");
			break;
		case 2:
			for(;;)
			{
				printf("请在下列课程中选择该生要退出的课程。\n");
				PrintAllCoursesAStudentAttended(student->StudentInfo.courseList);
				printf("输入课程的Id并回车来选择该课程，或输入0并回车来取消：\n");
				long chosenCourseId = GetChoice(0, LONG_MAX);
				//判断用户是否选择了取消
				if (chosenCourseId == 0) break;

				CourseNode* chosenCourse;
				if ((chosenCourse = FindAttendedCourseInStudentInfo(
					student->StudentInfo.courseList, chosenCourseId)))
					RemoveStudentFromSpecificCourse(student, chosenCourse);
				else printf("你输入了一个该学生未参加的课程的Id，此次请求将被忽略\n");
			}
			printf("已完成相关修改。\n");
			system("pause");
			break;
		case 0:
			return;
		}
	}
}
static void ModifyRecord(SystemData systemData)
{
	for (;;) {
		printf("请在下列课程中选择你要修改的记录所在的课程。\n");
		CourseNode* course = GetSelectedCourse(systemData->headOfCourses);
		//检查用户是否取消了选择
		if (course == NULL)
			return;

		for(;;) {
			printf("请在下列课程中选择你要修改的记录所在的作业。\n");
			AssignmentNode* assignment = GetSelectedAssignment(course->assignmentList);
			//检查用户是否取消了选择
			if (assignment == NULL)
				return;
			
			for(;;) {
				printf("请在下列学生中选择你要修改的记录所对应的学生。\n");
				StudentNodeInCourse* student = 
							GetSelectedStudentInCourse(course->studentList);
				//检查用户是否取消了选择
				if (student == NULL)
					return;

				printf("\t在课程\"%s\"(%ld)中作业\"%s\"(%ld)下，学生%s(%ld)的成绩为（负数表示未交）：",
					course->courseInfo.CourseName, course->courseInfo.courseId,
					assignment->assignmentInfo.assignmentName,
					assignment->assignmentInfo.assignmentId,
					student->studentNode->StudentInfo.studentName,
					student->studentNode->StudentInfo.studentId);
				Record* record = PrintSpecificRecord(student, assignment);
				printf("请输入新的成绩(0-100)并回车，或输入-1表示学生未交：");
				int newScore = GetChoice(-1, 100);
				record->score = newScore;
				RefreshAssignmentHandInsInfo(assignment);
				RefreshStudentInCourseHandInsInfo(student);
				RefreshCourseHandInsInfo(course);
				RefreshStudentHandInsInfo(student->studentNode);
				printf("已修改相应成绩数据。\n");
				system("pause");
			}
		}
	}
}
static void DeleteCourse(SystemData systemData, CourseNode* course)
{
	//若该课程刚好位于链表第一项，直接将其从链表中移除
	if (course == systemData->headOfCourses)
		systemData->headOfCourses = course->next;
	else
	{
		CourseNode* preCourse = systemData->headOfCourses;
		//找到对应课程结点的上一个结点
		for (; preCourse->next != course; preCourse = preCourse->next)
			continue;
		//将课程结点从链表中移除
		preCourse->next = course->next;
	}

	//将链表中各次作业及其记录从多重表中移除
	for (AssignmentNode* curAssignment = course->assignmentList;
		curAssignment != NULL; curAssignment = course->assignmentList)
	{
		course->assignmentList = curAssignment->nextAssigment;
		for (Record* curRecord = curAssignment->recordEntry;
			curRecord != NULL; curRecord = curAssignment->recordEntry)
		{
			curAssignment->recordEntry = curRecord->nextStudent;
			free(curRecord);
		}
		free(curAssignment);
	}

	//将链表中各个学生结点删除并刷新学生结点的courseList及其handInsInfo
	for (StudentNodeInCourse* curStudent = course->studentList;
		curStudent != NULL; curStudent = course->studentList)
	{
		course->studentList = curStudent->nextStudent;
		RemoveCourseFromStudentInfo(curStudent->studentNode, course);
		free(curStudent);
	}

	//回收课程结点本身的内存
	free(course);
}
static void DeleteAssignment(CourseNode* course, AssignmentNode* assignment)
{
	//若该作业刚好位于链表第一项，直接将其从链表中移除
	if (assignment == course->assignmentList)
	{
		course->assignmentList = assignment->nextAssigment;
		//将对应的作业成绩结点从另一方向的链表中移除
		for (StudentNodeInCourse* curStudent = course->studentList;
			curStudent != NULL; curStudent = curStudent->nextStudent)
		{
			curStudent->recordEntry = curStudent->recordEntry->nextAssigment;
		}
	}
	else
	{
		AssignmentNode* preAssignment = course->assignmentList;
		//找到对应作业结点的上一个结点
		for (; preAssignment->nextAssigment != assignment;
			preAssignment = preAssignment->nextAssigment)
			continue;
		//将作业结点从链表中移除
		preAssignment->nextAssigment = assignment->nextAssigment;
		//将对应的作业成绩结点从另一方向的链表中移除
		for (Record* curRecord = preAssignment->recordEntry;
			curRecord != NULL; curRecord = curRecord->nextStudent)
			curRecord->nextAssigment = curRecord->nextAssigment->nextAssigment;
	}

	//回收作业对应的作业成绩结点的内存
	for (Record* curRecord = assignment->recordEntry;
		curRecord != NULL; curRecord = assignment->recordEntry)
	{
		assignment->recordEntry = curRecord->nextStudent;
		free(curRecord);
	}
	//回收作业结点本身的内存
	free(assignment);

	//更新所有学生结点的作业成绩总计信息
	for (StudentNodeInCourse* curStudent = course->studentList;
		curStudent != NULL; curStudent = curStudent->nextStudent)
	{
		RefreshStudentInCourseHandInsInfo(curStudent);
		RefreshStudentHandInsInfo(curStudent->studentNode);
	}
	//更新课程的作业成绩总计信息
	RefreshCourseHandInsInfo(course);
}
static void DeleteStudent(SystemData systemData, StudentNode* student)
{
	//若该学生刚好位于链表第一项，直接将其从链表中移除
	if (student == systemData->headOfStudents)
	{
		systemData->headOfStudents = student->next;
	}
	else
	{
		StudentNode* preStudent = systemData->headOfStudents;
		//找到对应学生结点的上一个结点
		for(; preStudent->next != student; preStudent = preStudent->next)
			continue;
		//将学生结点从链表中移除
		preStudent->next = student->next;
	}
	//删除学生在每个课程内的结点
	for (CourseNodeInStudentInfo* curCourse = student->StudentInfo.courseList;
		curCourse != NULL; curCourse = curCourse->next)
	{
		RemoveStudentFromSpecificCourse(student, curCourse->courseNode);
	}
	free(student);
}
static void RemoveStudentFromSpecificCourse(StudentNode* student, CourseNode* course)
{
	StudentNodeInCourse* stuInCourse;
	//若该学生刚好位于链表第一项，直接将其从链表中移除
	if (student == course->studentList->studentNode)
	{
		stuInCourse = course->studentList;
		course->studentList = stuInCourse->nextStudent;
		//将对应的作业成绩结点从另一方向的链表中移除
		for (AssignmentNode* curAssignment = course->assignmentList;
			curAssignment != NULL; curAssignment = curAssignment->nextAssigment)
			curAssignment->recordEntry = curAssignment->recordEntry->nextStudent;
	}
	else
	{
		//找到对应学生结点的上一个结点
		StudentNodeInCourse* preStuInCourse = course->studentList;
		for (; preStuInCourse->nextStudent->studentNode != student;
			preStuInCourse = preStuInCourse->nextStudent)
			continue;
		stuInCourse = preStuInCourse->nextStudent;
		//将学生结点从链表中移除
		preStuInCourse->nextStudent = preStuInCourse->nextStudent->nextStudent;
		//将对应的作业成绩结点从另一方向的链表中移除
		for (Record* curRecord = preStuInCourse->recordEntry;
			curRecord != NULL; curRecord = curRecord->nextAssigment)
		{
			curRecord->nextStudent = curRecord->nextStudent->nextStudent;
		}
	}

	//回收学生对应的作业成绩结点的内存
	for (Record* curRecord = stuInCourse->recordEntry;
		curRecord != NULL; curRecord = stuInCourse->recordEntry)
	{
		stuInCourse->recordEntry = curRecord->nextAssigment;
		free(curRecord);
	}

	//更新学生结点中的信息
	RemoveCourseFromStudentInfo(stuInCourse->studentNode, course);

	//回收学生在课程中的结点本身的内存
	free(stuInCourse);
}
static void RemoveCourseFromStudentInfo(StudentNode* student, CourseNode* course)
{
	//若该课程刚好位于链表第一项，直接将其从链表中移除并回收内存
	if (course == student->StudentInfo.courseList->courseNode)
	{
		CourseNodeInStudentInfo* tmp = student->StudentInfo.courseList;
		student->StudentInfo.courseList = tmp->next;
		free(tmp);
	}
	else
	{
		//找到对应课程结点的上一个结点
		CourseNodeInStudentInfo* preCourseInStuInfo = student->StudentInfo.courseList;
		for (; preCourseInStuInfo->next->courseNode != course;
			preCourseInStuInfo = preCourseInStuInfo->next)
			continue;
		//将结点从链表中移除并回收其内存
		CourseNodeInStudentInfo* tmp = preCourseInStuInfo->next;
		preCourseInStuInfo->next = tmp->next;
		free(tmp);
	}
	RefreshStudentHandInsInfo(student);
}
#pragma endregion 

#pragma region 查询模块部分

#pragma region 多重查询部分
static void MultipleQuery_AllRecordInCourse(CourseNode* headOfCourses)
{
	printf("请在下列课程中选择你要查询的课程。\n");
	CourseNode* course = GetSelectedCourse(headOfCourses);
	//检查用户是否取消了选择
	if (course == NULL)
		return;

	printf("请选择数据的排列顺序：\t1：按学生顺序输出\t2：按作业顺序输出\t0：取消\n"
	"请输入数字(0-2)并回车来选择一个选项：");
	long choice = GetChoice(0, 2);
	switch (choice) {
	case 1: 
		for (StudentNodeInCourse* student = course->studentList;
			student != NULL; student = student->nextStudent)
			PrintAllRecordOfSpecificStudentInCourse(student->recordEntry);
			system("pause");
		break;
	case 2:
		for (AssignmentNode* assignment = course->assignmentList;
			assignment != NULL; assignment = assignment->nextAssigment)
			PrintAllRecordOfSpecificAssignment(assignment->recordEntry);
			system("pause");
	case 0: return;
	}
}
static void MultipleQuery_StudentRecordInEveryCourse(StudentNode* headOfStudents)
{
	printf("请在下列学生中选择你要查询的学生，或输入0来取消。\n");
	StudentNode* student = GetSelectedStudent(headOfStudents);
	//检查用户是否取消了选择
	if (student == NULL)
		return;

	for (CourseNodeInStudentInfo* CourInStuInfo = student->StudentInfo.courseList;
		CourInStuInfo != NULL; CourInStuInfo = CourInStuInfo->next)
	{
		StudentNodeInCourse* tmpStu = FindStudentNodeInCourse(
			CourInStuInfo->courseNode->studentList, student->StudentInfo.studentId);
		printf("在课程\"%s\"(%ld)中：\n", CourInStuInfo->courseNode->courseInfo.CourseName,
			CourInStuInfo->courseNode->courseInfo.courseId);
		PrintAllRecordOfSpecificStudentInCourse(tmpStu->recordEntry);
		
	}
	system("pause");
}
static void MultipleQuery_StudentRecordInSpecificCourse(StudentNode* headOfStudents)
{
	printf("请在下列学生中选择你要查询的学生。\n");
	StudentNode* student = GetSelectedStudent(headOfStudents);
	//检查用户是否取消了选择
	if (student == NULL)
		return;

	printf("请在下列课程中选择你要查询的课程。\n");
	CourseNode* course = GetSelectedCourseInStudentInfo(student->StudentInfo.courseList);
	//检查用户是否取消了选择
	if (course == NULL)
		return;

	PrintAllRecordOfSpecificStudentInCourse(
		FindStudentNodeInCourse(course->studentList, student->StudentInfo.studentId)
											->recordEntry);
	system("pause");
}
static void MultipleQuery_StudentRecordOfOneCourse(CourseNode* headOfCourses)
{
	printf("请在下列课程中选择你要查询的课程。\n");
	CourseNode* course = GetSelectedCourse(headOfCourses);
	//检查用户是否取消了选择
	if (course == NULL)
		return;

	printf("请在下列学生中选择你要查询的学生。\n");
	StudentNodeInCourse* student = GetSelectedStudentInCourse(course->studentList);
	//检查用户是否取消了选择
	if (student == NULL)
		return;

	PrintAllRecordOfSpecificStudentInCourse(student->recordEntry);
	system("pause");
}
static void MultipleQuery_AssignmentRecordOfOneCourse(CourseNode* headOfCourses)
{
	printf("请在下列课程中选择你要查询的课程。\n");
	CourseNode* course = GetSelectedCourse(headOfCourses);
	//检查用户是否取消了选择
	if (course == NULL)
		return;

	printf("请在下列作业中选择你要查询的作业。\n");
	AssignmentNode* assignment = GetSelectedAssignment(course->assignmentList);
	//检查用户是否取消了选择
	if (assignment == NULL)
		return;

	PrintAllRecordOfSpecificAssignment(assignment->recordEntry);
	system("pause");
}
#pragma endregion 

#pragma region 查询总计信息部分
static void HandInsInfoQuery_CourseHandIns(CourseNode* headOfCourses)
{
	printf("请在下列课程中选择你要查询的课程。\n");
	CourseNode* course = GetSelectedCourse(headOfCourses);
	//检查用户是否取消了选择
	if (course == NULL)
		return;

	PrintHandInsInfo(&(course->handInsInfo));
	system("pause");
}
static void HandInsInfoQuery_StudentHandIns(StudentNode* headOfStudents)
{
	printf("请在下列学生中选择你要查询的学生。\n");
	StudentNode* student = GetSelectedStudent(headOfStudents);
	//检查用户是否取消了选择
	if (student == NULL)
		return;
	
	PrintHandInsInfo(&(student->StudentInfo.handInsInfo));
	system("pause");
}
static void HandInsInfoQuery_StudentInSpecificCourse(StudentNode* headOfStudents)
{
	printf("请在下列学生中选择你要查询的学生。\n");
	StudentNode* student = GetSelectedStudent(headOfStudents);
	//检查用户是否取消了选择
	if (student == NULL)
		return;

	printf("请在下列课程中选择你要查询的课程。\n");
	CourseNode* course = GetSelectedCourseInStudentInfo(student->StudentInfo.courseList);
	//检查用户是否取消了选择
	if (course == NULL)
		return;

	PrintHandInsInfo(&(FindStudentNodeInCourse(course->studentList, 
						student->StudentInfo.studentId)->handInsInfo));
	system("pause");
}
static void HandInsInfoQuery_StudentOfOneCourse(CourseNode* headOfCourses)
{
	printf("请在下列课程中选择你要查询的课程，或输入0来取消。\n");
	CourseNode* course = GetSelectedCourse(headOfCourses);
	//检查用户是否取消了选择
	if (course == NULL)
		return;

	printf("请在下列学生中选择你要查询的学生，或输入0来取消。\n");
	StudentNodeInCourse* student = GetSelectedStudentInCourse(course->studentList);
	//检查用户是否取消了选择
	if (student == NULL)
		return;

	PrintHandInsInfo(&(student->handInsInfo));
	system("pause");
}
static void HandInsInfoQuery_AssignmentOfOneCourse(CourseNode* headOfCourses)
{
	printf("请在下列课程中选择你要查询的课程，或输入0来取消。\n");
	CourseNode* course = GetSelectedCourse(headOfCourses);
	//检查用户是否取消了选择
	if (course == NULL)
		return;

	printf("请在下列作业中选择你要查询的作业，或输入0来取消。\n");
	AssignmentNode* assignment = GetSelectedAssignment(course->assignmentList);
	//检查用户是否取消了选择
	if (assignment == NULL)
		return;

	PrintHandInsInfo(&(assignment->handInsInfo));
	system("pause");
}
#pragma endregion 

#pragma endregion 

#pragma region 录入模块部分

#pragma region 载入存档子函数部分
//注：虽然cJSON中数字使用的是int，但在我的软硬件环境下long和int的范围是一样的，故不会出现问题
//		但本人并不保证在其它软硬件环境下编译后不会出现问题
static void LoadCoursesData(SystemData systemData)
{
	//准备读取文件
	FILE* fp;
	fopen_s(&fp, "Courses.dat", "r");
	char* buffer = (char*)malloc(MAX_JSON_BUFFER_LENGTH * sizeof(char));
	if (buffer == NULL)
	{
		printf("Error: fail to allocate memory in %s.\n", "LoadCoursesData");
		exit(EXIT_FAILURE);
	}
	char* curChar = buffer;
	if (fp == NULL)
	{
		printf("Error: failed to load file: %s.\n", "Courses.dat");
		system("pause");
		exit(EXIT_FAILURE);
	}
	while ((*curChar++ = fgetc(fp)) != EOF)
		continue;
	*--curChar = '\0';
	fclose(fp);

	//分析JSON字符串
	cJSON* json_Root = cJSON_Parse(buffer);
	if (cJSON_GetErrorPtr())
	{
		printf("Error: failed to analyse data from %s, it might have been corrupted.\n",
												"Courses.dat");
		system("pause");
		exit(EXIT_FAILURE);
	}
	const int arraySize = cJSON_GetArraySize(json_Root);
	for (int i = 0; i < arraySize; ++i)
	{
		cJSON* json_CourInfo = cJSON_GetArrayItem(json_Root, i);
		//执行课程结点的添加操作
		AddCourse_Util(systemData,
			cJSON_GetObjectItem(json_CourInfo, "CourId")->valueint,
			cJSON_GetObjectItem(json_CourInfo, "CourName")->valuestring);
	}
	cJSON_Delete(json_Root);
}
static void LoadStudentsData(SystemData systemData)
{
	//准备读取文件
	FILE* fp;
	fopen_s(&fp, "Students.dat", "r");
	char* buffer = (char*)malloc(MAX_JSON_BUFFER_LENGTH * sizeof(char));
	if (buffer == NULL)
	{
		printf("Error: fail to allocate memory in %s.\n", "LoadStudentsData");
		exit(EXIT_FAILURE);
	}
	char* curChar = buffer;
	if (fp == NULL)
	{
		printf("Error: failed to load file: %s.\n", "Students.dat");
		system("pause");
		exit(EXIT_FAILURE);
	}
	while ((*curChar++ = fgetc(fp)) != EOF)
		continue;
	*--curChar = '\0';
	fclose(fp);

	//分析JSON字符串
	cJSON* json_Root = cJSON_Parse(buffer);
	if (cJSON_GetErrorPtr())
	{
		printf("Error: failed to analyse data from %s, it might have been corrupted.\n", 
												"Students.dat");
		system("pause");
		exit(EXIT_FAILURE);
	}
	const int studentCount = cJSON_GetArraySize(json_Root);
	for (int i = 0; i < studentCount; ++i)
	{
		cJSON* json_Student  = cJSON_GetArrayItem (json_Root, i);
		cJSON* json_StuInfo  = cJSON_GetObjectItem(json_Student, "StuInfo");
		cJSON* json_CourList = cJSON_GetObjectItem(json_Student, "CourList");
		//执行学生结点的添加操作
		StudentNode* student = AddStudent_Util(systemData,
			cJSON_GetObjectItem(json_StuInfo, "StuId")->valueint,
			cJSON_GetObjectItem(json_StuInfo, "StuName")->valuestring);

		//向课程中添加学生
		const int courseCount = cJSON_GetArraySize(json_CourList);
		for (int j = 0; j < courseCount; ++j)
		{
			long courseId = cJSON_GetArrayItem(json_CourList, j)->valueint;
			CourseNode* course = FindCourseNode(systemData->headOfCourses, courseId);
			AddStudentIntoCourse_Util(course, student);
		}
	}
	cJSON_Delete(json_Root);
}
static void LoadAssignmentData(SystemData systemData)
{
	//准备读取文件
	FILE* fp;
	fopen_s(&fp, "Assignments.dat", "r");
	char* buffer = (char*)malloc(MAX_JSON_BUFFER_LENGTH * sizeof(char));
	if (buffer == NULL)
	{
		printf("Error: fail to allocate memory in %s.\n", "LoadAssignmentData");
		exit(EXIT_FAILURE);
	}
	char* curChar = buffer;
	if (fp == NULL)
	{
		printf("Error: failed to load file: %s.\n", "Assignments.dat");
		system("pause");
		exit(EXIT_FAILURE);
	}
	while ((*curChar++ = fgetc(fp)) != EOF)
		continue;
	*--curChar = '\0';
	fclose(fp);

	//分析JSON字符串
	cJSON* json_Root = cJSON_Parse(buffer);
	if (cJSON_GetErrorPtr())
	{
		printf("Error: failed to analyse data from %s, it might have been corrupted.\n", 
																		"Assignments.dat");
		system("pause");
		exit(EXIT_FAILURE);
	}
	const int courseCount = cJSON_GetArraySize(json_Root);
	for (int i = 0; i < courseCount; ++i)
	{
		//确定是哪个课程内的作业的信息
		cJSON* json_Course = cJSON_GetArrayItem(json_Root, i);
		long courseId = cJSON_GetObjectItem(json_Course, "CourId")->valueint;
		CourseNode* course = FindCourseNode(systemData->headOfCourses, courseId);

		//获取作业信息数组
		cJSON* json_AssiList = cJSON_GetObjectItem(json_Course, "AssiList");
		int assignmentCount = cJSON_GetArraySize(json_AssiList);

		//向课程中添加作业
		for (int j = 0; j < assignmentCount; ++j)
		{
			cJSON* json_AssiInfo = cJSON_GetArrayItem(json_AssiList, j);
			AddAssignment_Util(course,
				cJSON_GetObjectItem(json_AssiInfo, "AssiId")->valueint,
				cJSON_GetObjectItem(json_AssiInfo, "AssiName")->valuestring);
		}
	}
	cJSON_Delete(json_Root);
}
static void LoadRecordData(SystemData systemData)
{
	//准备读取文件
	FILE* fp;
	fopen_s(&fp, "Record.dat", "r");
	char* buffer = (char*)malloc(MAX_JSON_BUFFER_LENGTH * sizeof(char));
	if (buffer == NULL)
	{
		printf("Error: fail to allocate memory in %s.\n", "LoadRecordData");
		exit(EXIT_FAILURE);
	}
	char* curChar = buffer;
	if (fp == NULL)
	{
		printf("Error: failed to load file: %s.\n", "Record.dat");
		system("pause");
		exit(EXIT_FAILURE);
	}
	while ((*curChar++ = fgetc(fp)) != EOF)
		continue;
	*--curChar = '\0';
	fclose(fp);

	//分析JSON字符串
	cJSON* json_Root = cJSON_Parse(buffer);
	if (cJSON_GetErrorPtr())
	{
		printf("Error: failed to analyse data from %s, it might have been corrupted.\n", 
																		"Record.dat");
		system("pause");
		exit(EXIT_FAILURE);
	}
	int courseCount = cJSON_GetArraySize(json_Root);
	for (int i = 0; i < courseCount; ++i)
	{
		//获取当前课程
		cJSON* json_Course = cJSON_GetArrayItem(json_Root, i);
		long courseId = cJSON_GetObjectItem(json_Course, "CourId")->valueint;
		CourseNode* course = FindCourseNode(systemData->headOfCourses, courseId);
		
		//获取当前课程内的作业列表
		cJSON* json_AssiList = cJSON_GetObjectItem(json_Course, "AssiList");
		int assiCount = cJSON_GetArraySize(json_AssiList);
		for (int j = 0; j < assiCount; ++j)
		{
			//获取当前作业
			cJSON* json_Assi = cJSON_GetArrayItem(json_AssiList, j);
			long assignmentId = cJSON_GetObjectItem(json_Assi, "AssiId")->valueint;
			AssignmentNode* assignment = FindAssignmentNode(course->assignmentList, 
																assignmentId);

			//获取当前每个学生的作业成绩列表
			cJSON* json_StuList = cJSON_GetObjectItem(json_Assi, "StuList");
			int stuCount = cJSON_GetArraySize(json_StuList);
			for (int k = 0; k < stuCount; ++k)
			{
				//获取当前学生
				cJSON* json_Student = cJSON_GetArrayItem(json_StuList, k);
				long studentId = cJSON_GetObjectItem(json_Student, 
													"StuId")->valueint;
				StudentNodeInCourse* student = FindStudentNodeInCourse(
											course->studentList, studentId);

				//构建作业成绩结点
				long score = cJSON_GetObjectItem(json_Student, "Score")->valueint;
				Record* record = (Record*)malloc(sizeof(struct Record_));
				record->assignmentId = assignmentId;
				record->studentId	 = studentId;
				record->score		 = score;

				//插入多重表的对应位置
				record->nextStudent   = assignment->recordEntry;
				record->nextAssigment = student->recordEntry;
				assignment->recordEntry = record;
				student->recordEntry    = record;
			}
			//计算本次作业的作业成绩总计信息
			RefreshAssignmentHandInsInfo(assignment);
		}
		//计算课程内所有学生和课程的作业成绩总计信息
		for (StudentNodeInCourse* student = course->studentList;
			student != NULL; student = student->nextStudent)
			RefreshStudentInCourseHandInsInfo(student);
		RefreshCourseHandInsInfo(course);
	}
	//计算学生在所有课程内的作业成绩总计信息
	for (StudentNode* student = systemData->headOfStudents;
		student != NULL; student = student->next)
		RefreshStudentHandInsInfo(student);

	cJSON_Delete(json_Root);
}
#pragma endregion

#pragma region 各添加操作的子函数部分
static CourseNode* AddCourse_Util(SystemData systemData,
						int courseId, char* courseName)
{
	//构建课程结点
	CourseNode* course = (CourseNode*)
							malloc(sizeof(struct Course_Node));
	if (course == NULL)
	{
		printf("Error: fail to allocate memory in %s.\n", "AddCourse_Util");
		exit(EXIT_FAILURE);
	}
	course->courseInfo.courseId = courseId;
	strcpy_s(course->courseInfo.CourseName, 
				MAX_COURSE_NAME_LENGTH - 1, courseName);
	course->handInsInfo.expectedHandInCount = 0;
	course->handInsInfo.actualHandInCount   = 0;
	course->handInsInfo.goodHandInCount     = 0;
	course->assignmentList = NULL;
	course->studentList	   = NULL;

	//插入课程链表中
	course->next = systemData->headOfCourses;
	systemData->headOfCourses = course;
	return course;
}
static StudentNodeInCourse* AddStudentIntoCourse_Util(
								CourseNode* course, StudentNode* student)
{
	//构建学生结点
	StudentNodeInCourse* stuInCour = (StudentNodeInCourse*)
							malloc(sizeof(struct Student_Node_In_Course));
	if (stuInCour == NULL)
	{
		printf("Error: fail to allocate memory in %s.\n", "AddStudentIntoCourse_Util");
		exit(EXIT_FAILURE);
	}
	stuInCour->studentNode = student;
	stuInCour->recordEntry = NULL;
	stuInCour->handInsInfo.expectedHandInCount = 0;
	stuInCour->handInsInfo.actualHandInCount   = 0;
	stuInCour->handInsInfo.goodHandInCount     = 0;

#if TESTING
	printf("construction of stuInCour completed.\n");
#endif

	//插入链表中
	stuInCour->nextStudent = course->studentList;
	course->studentList = stuInCour;

#if TESTING
	printf("insertion of stuInCour completed.\n");
#endif

	//更新学生结点的参加课程链表
	CourseNodeInStudentInfo* courInStuInfo = (CourseNodeInStudentInfo*)
							malloc(sizeof(struct Course_Node_In_StudentInfo));
	if (courInStuInfo == NULL)
	{
		printf("Error: fail to allocate memory in %s.\n", "AddStudentIntoCourse_Util");
		exit(EXIT_FAILURE);
	}
	courInStuInfo->courseNode = course;

#if TESTING
	printf("construction of courseInStu completed.\n");
#endif

	courInStuInfo->next = student->StudentInfo.courseList;
	student->StudentInfo.courseList = courInStuInfo;

#if TESTING
	printf("insertion of courseInStu completed.\n");
#endif

	return stuInCour;
}
static AssignmentNode* AddAssignment_Util(CourseNode* course,
							int assignmentId, char* assignmentName)
{
	//构建作业结点
	AssignmentNode* assignment = (AssignmentNode*)
									malloc(sizeof(struct Assignment_Node));
	if (assignment == NULL)
	{
		printf("Error: fail to allocate memory in %s.\n", "AddAssignment_Util");
		exit(EXIT_FAILURE);
	}
	assignment->assignmentInfo.assignmentId = assignmentId;
	strcpy_s(assignment->assignmentInfo.assignmentName, 
							MAX_ASSIGNMENT_NAME_LENGTH - 1, assignmentName);
	assignment->recordEntry = NULL;
	assignment->handInsInfo.expectedHandInCount = 0;
	assignment->handInsInfo.actualHandInCount   = 0;
	assignment->handInsInfo.goodHandInCount     = 0;

	//插入链表中
	assignment->nextAssigment = course->assignmentList;
	course->assignmentList = assignment;
	return assignment;
}
static StudentNode* AddStudent_Util(SystemData systemData,
							int studentId, char* studentName)
{
	//构建学生结点
	StudentNode* student = (StudentNode*)
								malloc(sizeof(struct Student_Node));
	if (student == NULL)
	{
		printf("Error: fail to allocate memory in %s.\n", "AddStudent_Util");
		exit(EXIT_FAILURE);
	}
	student->StudentInfo.studentId = studentId;
	strcpy_s(student->StudentInfo.studentName, 
				MAX_STUDENT_NAME_LENGTH - 1, studentName);
	student->StudentInfo.courseList = NULL;
	student->StudentInfo.handInsInfo.expectedHandInCount = 0;
	student->StudentInfo.handInsInfo.actualHandInCount   = 0;
	student->StudentInfo.handInsInfo.goodHandInCount     = 0;

	//插入链表中
	student->next = systemData->headOfStudents;
	systemData->headOfStudents = student;

	return student;
}
static void AddAssignment_RecordInput(CourseNode* course,
								AssignmentNode* assignment)
{
	//若该课程中暂时还没有学生，则不需要录入成绩数据
	if (course->studentList == NULL)
		return;

	//否则需要录入每一个学生的作业成绩
	printf("下面开始录入各个学生本次作业的成绩。\n");
	for (StudentNodeInCourse* curStu = course->studentList; 
		curStu != NULL; curStu = curStu->nextStudent)
	{
		//输出提示信息并获取作业成绩
		printf("学生%s(%ld)在本次作业的成绩是（0-100,-1代表未交）：", 
			curStu->studentNode->StudentInfo.studentName,
			curStu->studentNode->StudentInfo.studentId);

		//构建作业成绩结点
		Record* record = (Record*)malloc(sizeof(struct Record_));
		if (record == NULL)
		{
			printf("Error: fail to allocate memory in %s.\n", "AddAssignment_RecordInput");
			exit(EXIT_FAILURE);
		}
		record->studentId = curStu->studentNode->StudentInfo.studentId;
		record->assignmentId = assignment->assignmentInfo.assignmentId;
		record->score = GetChoice(-1, 100);;

		//插入多重表中
		record->nextStudent = assignment->recordEntry;
		record->nextAssigment =   curStu->recordEntry;
		assignment->recordEntry = record;
		curStu->recordEntry     = record;

		RefreshStudentInCourseHandInsInfo(curStu);
		RefreshStudentHandInsInfo(curStu->studentNode);
	}
	RefreshAssignmentHandInsInfo(assignment);
	RefreshCourseHandInsInfo(course);
}
static void AddStudentIntoCourse_RecordInput(CourseNode* course,
					StudentNodeInCourse* stuNodeInCour)
{
	//若该课程中暂时还没有作业，则不需要录入成绩数据
	if (course->assignmentList == NULL)
		return;

	//否则需要录入该生每一次的作业成绩
	printf("下面开始录入该生每次作业的成绩。\n");
	for (AssignmentNode* curAssi = course->assignmentList;
		curAssi != NULL; curAssi = curAssi->nextAssigment)
	{
		//输出提示信息并获取作业成绩
		printf("该生在作业\"%s\"(%ld)的成绩是（0-100,-1代表未交）：",
			curAssi->assignmentInfo.assignmentName,
			curAssi->assignmentInfo.assignmentId);
		
		//构建作业成绩结点
		Record* record = (Record*)malloc(sizeof(struct Record_));
		if (record == NULL)
		{
			printf("Error: fail to allocate memory in %s.\n", "AddStudentIntoCourse_RecordInput");
			exit(EXIT_FAILURE);
		}
		record->studentId = stuNodeInCour->studentNode->StudentInfo.studentId;
		record->assignmentId = curAssi->assignmentInfo.assignmentId;
		record->score = GetChoice(-1, 100);

		//插入多重表中
		record->nextStudent   = curAssi->recordEntry;
		record->nextAssigment = stuNodeInCour->recordEntry;
		curAssi->recordEntry       = record;
		stuNodeInCour->recordEntry = record;

		RefreshAssignmentHandInsInfo(curAssi);
	}
	RefreshStudentInCourseHandInsInfo(stuNodeInCour);
	RefreshStudentHandInsInfo(stuNodeInCour->studentNode);
	RefreshCourseHandInsInfo(course);
}
#pragma endregion 

#pragma endregion 

#pragma region 保存模块子函数
static void SaveMode_Course(CourseNode* headOfCourses)
{
	//课程信息列表
	cJSON* json_Root = cJSON_CreateArray();

	//遍历课程链表，将每一个课程的信息转化为JSON格式然后插入root中
	for (CourseNode* curCour = headOfCourses; curCour != NULL;
		curCour = curCour->next)
	{
		//构建当前课程的JSON对象
		cJSON* json_Cour = cJSON_CreateObject();
		cJSON_AddNumberToObject(json_Cour, "CourId",
			curCour->courseInfo.courseId);
		cJSON_AddStringToObject(json_Cour, "CourName",
			curCour->courseInfo.CourseName);

		//将当前课程插入root中
		cJSON_AddItemToArray(json_Root, json_Cour);
	}

	//完成文件写入
	FILE* fp;
	fopen_s(&fp, "Courses.dat", "w");
	if (fp == NULL)
	{
		printf("Error: fail to open %s, in function %s", "Courses.dat", "SaveMode_Course");
		exit(EXIT_FAILURE);
	}
	char* buffer = cJSON_Print(json_Root);
	fputs(buffer, fp);

	//释放空间
	cJSON_Delete(json_Root);
	free(buffer);
	fclose(fp);
}
static void SaveMode_Student(StudentNode* headOfStudents)
{
	//学生信息列表
	cJSON* json_Root = cJSON_CreateArray();

	//遍历学生链表，将每一个课程的信息转化为JSON格式然后插入学生列表中
	for (StudentNode* curStu = headOfStudents; curStu != NULL;
		curStu = curStu->next)
	{
		//当前学生的JSON对象
		cJSON* json_Stu = cJSON_CreateObject();
		
		//构建该生的信息的JSON对象
		cJSON* json_StuInfo = cJSON_CreateObject();
		cJSON_AddNumberToObject(json_StuInfo, "StuId", 
							curStu->StudentInfo.studentId);
		cJSON_AddStringToObject(json_StuInfo, "StuName", 
							curStu->StudentInfo.studentName);

		//构建该生的参加课程列表JSON对象
		cJSON* json_StuCourList = cJSON_CreateArray();
		for (CourseNodeInStudentInfo* curCour = curStu->StudentInfo.courseList;
			curCour != NULL; curCour = curCour->next)
		{
			//将当前课程的Id添加到课程列表里
			cJSON_AddItemToArray(json_StuCourList,
				cJSON_CreateNumber(curCour->courseNode->courseInfo.courseId));
		}

		//将该生信息和该生参加课程链表添加到该生的JSON对象里
		cJSON_AddItemToObject(json_Stu, "StuInfo", json_StuInfo);
		cJSON_AddItemToObject(json_Stu, "CourList", json_StuCourList);

		//将当前学生加入学生列表中
		cJSON_AddItemToArray(json_Root, json_Stu);
	}

	//完成文件写入
	FILE* fp;
	fopen_s(&fp, "Students.dat", "w");
	if (fp == NULL)
	{
		printf("Error: fail to open %s, in function %s", "Students.dat", "SaveMode_Student");
		exit(EXIT_FAILURE);
	}
	char* buffer = cJSON_Print(json_Root);
	fputs(buffer, fp);

	//释放空间
	cJSON_Delete(json_Root);
	free(buffer);
	fclose(fp);
}
static void SaveMode_Assignment(CourseNode* headOfCourses)
{
	//每个课程的作业信息列表
	cJSON* json_Root = cJSON_CreateArray();

	//遍历课程链表，将每一个课程及其作业信息转化为JSON格式然后插入root中
	for (CourseNode* curCour = headOfCourses; curCour != NULL;
		curCour = curCour->next)
	{
		//当前课程的JSON对象
		cJSON* json_Cour = cJSON_CreateObject();
		cJSON_AddNumberToObject(json_Cour, "CourId",	
								curCour->courseInfo.courseId);

		//课程内包含的作业列表
		cJSON* json_AssiList = cJSON_CreateArray();
		for (AssignmentNode* curAssi = curCour->assignmentList;
			curAssi != NULL; curAssi = curAssi->nextAssigment)
		{
			//当前作业的JSON对象
			cJSON* json_Assi = cJSON_CreateObject();
			cJSON_AddNumberToObject(json_Assi, "AssiId", 
						curAssi->assignmentInfo.assignmentId);
			cJSON_AddStringToObject(json_Assi, "AssiName",
						curAssi->assignmentInfo.assignmentName);

			//加入作业列表中
			cJSON_AddItemToArray(json_AssiList, json_Assi);
		}

		//将作业列表加入课程对象中
		cJSON_AddItemToObject(json_Cour, "AssiList", json_AssiList);

		//将课程对象加入课程列表中
		cJSON_AddItemToArray(json_Root, json_Cour);
	}

	//完成文件写入
	FILE* fp;
	fopen_s(&fp, "Assignments.dat", "w");
	if (fp == NULL)
	{
		printf("Error: fail to open %s, in function %s", "Assignments.dat", "SaveMode_Assignment");
		exit(EXIT_FAILURE);
	}
	char* buffer = cJSON_Print(json_Root);
	fputs(buffer, fp);

	//释放空间
	cJSON_Delete(json_Root);
	free(buffer);
	fclose(fp);
}
static void SaveMode_Record(CourseNode* headOfCourses)
{
	//每个课程的作业成绩信息列表
	cJSON* json_Root = cJSON_CreateArray();

	//遍历课程链表，构建课程的作业成绩信息的JSON对象并插入列表中
	for (CourseNode* curCour = headOfCourses; curCour != NULL;
		curCour = curCour->next)
	{
		//课程的作业成绩信息的JSON对象
		cJSON* json_Cour = cJSON_CreateObject();
		cJSON_AddNumberToObject(json_Cour, "CourId",
								curCour->courseInfo.courseId);
		
		//当前课程的作业JSON对象组成的JSON数组
		cJSON* json_AssiList = cJSON_CreateArray();
		//遍历作业链表，构建作业的作业成绩信息的JSON对象并插入列表中
		for (AssignmentNode* curAssi = curCour->assignmentList;
			curAssi != NULL; curAssi = curAssi->nextAssigment)
		{
			//当前作业的作业成绩信息JSON对象
			cJSON* json_Assi = cJSON_CreateObject();
			cJSON_AddNumberToObject(json_Assi, "AssiId",
							curAssi->assignmentInfo.assignmentId);

			//当前作业对应的一系列作业成绩对象组成的JSON数组
			cJSON* json_RecList = cJSON_CreateArray();
			//遍历作业成绩记录链表，构建每个记录的结点并插入列表中
			for (Record* curRec = curAssi->recordEntry;
				curRec != NULL; curRec = curRec->nextStudent)
			{
				cJSON* json_Rec = cJSON_CreateObject();
				cJSON_AddNumberToObject(json_Rec, "StuId", curRec->studentId);
				cJSON_AddNumberToObject(json_Rec, "Score", curRec->score);

				//将本次作业成绩插入作业成绩列表中
				cJSON_AddItemToArray(json_RecList, json_Rec);
			}
			//将作业成绩列表插入作业的JSON对象中
			cJSON_AddItemToObject(json_Assi, "StuList", json_RecList);

			//将作业对象插入作业列表中
			cJSON_AddItemToArray(json_AssiList, json_Assi);
		}
		//将作业列表插入课程的JSON对象中
		cJSON_AddItemToObject(json_Cour, "AssiList", json_AssiList);

		//将课程对象插入课程列表中
		cJSON_AddItemToArray(json_Root, json_Cour);
	}

	//完成文件写入
	FILE* fp;
	fopen_s(&fp, "Record.dat", "w");
	if (fp == NULL)
	{
		printf("Error: fail to open %s, in function %s", "Record.dat", "SaveMode_Record");
		exit(EXIT_FAILURE);
	}
	char* buffer = cJSON_Print(json_Root);
	fputs(buffer, fp);

	//释放空间
	cJSON_Delete(json_Root);
	free(buffer);	
	fclose(fp);
}
#pragma endregion