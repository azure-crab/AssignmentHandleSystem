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

//本头文件内包含系统所用的各种数据结构的定义
#pragma once
#define	MAX_COURSE_NAME_LENGTH		50
#define MAX_ASSIGNMENT_NAME_LENGTH	200
#define MAX_STUDENT_NAME_LENGTH		50
#define MAXINPUTLENGTH				129

//先将所有结构不完全声明并typedef，保证后续声明不会出现无法解析的标识符
struct	Course_Info;
struct	Course_Node;
struct	Course_Node_In_StudentInfo;
struct	HandIns_Info;
struct	Assignment_Info;
struct	Assignment_Node;
struct	Student_Info;
struct	Student_Node_In_Course;
struct	Student_Node;
struct	Record_;
typedef struct Course_Info					CourseInfo;
typedef struct Course_Node					CourseNode;
typedef struct Course_Node_In_StudentInfo	CourseNodeInStudentInfo;
typedef struct HandIns_Info 				HandInsInfo;
typedef struct Assignment_Info 				AssignmentInfo;
typedef struct Assignment_Node				AssignmentNode;
typedef struct Student_Info					StudentInfo;
typedef struct Student_Node_In_Course 		StudentNodeInCourse;
typedef struct Student_Node 				StudentNode;
typedef struct Record_						Record;

//系统信息集合
struct System_Data {
	CourseNode*	 headOfCourses;
	StudentNode* headOfStudents;
};

//作业成绩总计信息
struct HandIns_Info {
	int		expectedHandInCount;
	int		actualHandInCount;
	int		goodHandInCount;
};
//单次作业信息
struct Assignment_Info {
	long	assignmentId;
	char	assignmentName[MAX_ASSIGNMENT_NAME_LENGTH];
};

//课程信息
struct Course_Info {
	long	courseId;
	char	CourseName[MAX_COURSE_NAME_LENGTH];
};
//课程结点
struct Course_Node {
	CourseInfo 				courseInfo;
	HandInsInfo				handInsInfo;
	CourseNode*				next;
	AssignmentNode* 		assignmentList;
	StudentNodeInCourse*	studentList;
};
//课程在学生参加课程链表中的节点
struct Course_Node_In_StudentInfo {
	CourseNode*					courseNode;
	CourseNodeInStudentInfo*	next;
};

//单次作业信息结点
struct Assignment_Node {
	AssignmentInfo 	assignmentInfo;
	HandInsInfo 	handInsInfo;
	AssignmentNode*	nextAssigment;
	Record*			recordEntry;
};

//学生信息
struct Student_Info {
	long						studentId;
	char						studentName[MAX_STUDENT_NAME_LENGTH];
	HandInsInfo					handInsInfo;
	CourseNodeInStudentInfo*	courseList;
};
//学生在某课程中的结点
struct Student_Node_In_Course {
	StudentNode* 			studentNode;
	HandInsInfo 			handInsInfo;
	StudentNodeInCourse*	nextStudent;
	Record*					recordEntry;
};
//学生在学生总链表中的结点
struct Student_Node {
	StudentInfo 	StudentInfo;
	StudentNode*	next;
};

//作业成绩记录
struct Record_ {
	long		assignmentId;
	long		studentId;
	int 		score;
	Record*		nextStudent;
	Record*		nextAssigment;
};